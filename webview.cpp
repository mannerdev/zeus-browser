#include "adblock/adblockdialog.h"
#include "adblock/adblockmanager.h"
#include "adblock/adblockpage.h"

#include "browserapplication.h"
#include "browsermainwindow.h"
#include "bookmarks.h"
#include "cookiejar.h"
#include "downloadmanager.h"
#include "networkaccessmanager.h"
#include "tabwidget.h"
#include "webview.h"


#include <QtGui/QClipboard>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtGui/QMouseEvent>

#include <QWebHitTestResult>

#ifndef QT_NO_UITOOLS
#include <QtUiTools/QUiLoader>
#endif  //QT_NO_UITOOLS

#include <QtCore/QDebug>
#include <QtCore/QBuffer>

WebPage::WebPage(QObject *parent)
    : QWebPage(parent)
    , m_keyboardModifiers(Qt::NoModifier)
    , m_pressedButtons(Qt::NoButton)
    , m_openInNewTab(false)
{
    setNetworkAccessManager(BrowserApplication::networkAccessManager());
    connect(this, SIGNAL(unsupportedContent(QNetworkReply*)),
            this, SLOT(handleUnsupportedContent(QNetworkReply*)));
}

BrowserMainWindow *WebPage::mainWindow()
{
    QObject *w = this->parent();
    while (w) {
        if (BrowserMainWindow *mw = qobject_cast<BrowserMainWindow*>(w))
            return mw;
        w = w->parent();
    }
    return BrowserApplication::instance()->mainWindow();
}

bool WebPage::acceptNavigationRequest(QWebFrame *frame, const QNetworkRequest &request, NavigationType type)
{
    // ctrl open in new tab
    // ctrl-shift open in new tab and select
    // ctrl-alt open in new window
    if (type == QWebPage::NavigationTypeLinkClicked
        && (m_keyboardModifiers & Qt::ControlModifier
            || m_pressedButtons == Qt::MidButton)) {
        bool newWindow = (m_keyboardModifiers & Qt::AltModifier);
        WebView *webView;
        if (newWindow) {
            BrowserApplication::instance()->newMainWindow();
            BrowserMainWindow *newMainWindow = BrowserApplication::instance()->mainWindow();
            webView = newMainWindow->currentTab();
            newMainWindow->raise();
            newMainWindow->activateWindow();
            webView->setFocus();
        } else {
            bool selectNewTab = (m_keyboardModifiers & Qt::ShiftModifier);
            webView = mainWindow()->tabWidget()->newTab(selectNewTab);
        }
        webView->load(request);
        m_keyboardModifiers = Qt::NoModifier;
        m_pressedButtons = Qt::NoButton;
        return false;
    }
    if (frame == mainFrame()) {
        m_loadingUrl = request.url();
        emit loadingUrl(m_loadingUrl);
    }
    return QWebPage::acceptNavigationRequest(frame, request, type);
}

QWebPage *WebPage::createWindow(QWebPage::WebWindowType type)
{
    Q_UNUSED(type);
    if (m_keyboardModifiers & Qt::ControlModifier || m_pressedButtons == Qt::MidButton)
        m_openInNewTab = true;
    if (m_openInNewTab) {
        m_openInNewTab = false;
        return mainWindow()->tabWidget()->newTab()->page();
    }
    BrowserApplication::instance()->newMainWindow();
    BrowserMainWindow *mainWindow = BrowserApplication::instance()->mainWindow();

    return mainWindow->currentTab()->page();
}

#if !defined(QT_NO_UITOOLS)
QObject *WebPage::createPlugin(const QString &classId, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues)
{
    Q_UNUSED(url);
    Q_UNUSED(paramNames);
    Q_UNUSED(paramValues);
    QUiLoader loader;
    return loader.createWidget(classId, view());
}
#endif // !defined(QT_NO_UITOOLS)

void WebPage::handleUnsupportedContent(QNetworkReply *reply)
{
    QString errorString = reply->errorString();

    if (m_loadingUrl != reply->url()) {
        // sub resource of this page
        qWarning() << "Resource" << reply->url().toEncoded() << "has unknown Content-Type, will be ignored.";
        reply->deleteLater();
        return;
    }

    if (reply->error() == QNetworkReply::NoError && !reply->header(QNetworkRequest::ContentTypeHeader).isValid()) {
        errorString = "Unknown Content-Type";
    }

    QFile file(QLatin1String(":/notfound.html"));
    bool isOpened = file.open(QIODevice::ReadOnly);
    Q_ASSERT(isOpened);
    Q_UNUSED(isOpened)

    QString title = tr("Error loading page: %1").arg(reply->url().toString());
    QString html = QString(QLatin1String(file.readAll()))
                        .arg(title)
                        .arg(errorString)
                        .arg(reply->url().toString());

    QBuffer imageBuffer;
    imageBuffer.open(QBuffer::ReadWrite);
    QIcon icon = view()->style()->standardIcon(QStyle::SP_MessageBoxWarning, 0, view());
    QPixmap pixmap = icon.pixmap(QSize(32,32));
    if (pixmap.save(&imageBuffer, "PNG")) {
        html.replace(QLatin1String("IMAGE_BINARY_DATA_HERE"),
                     QString(QLatin1String(imageBuffer.buffer().toBase64())));
    }

    QList<QWebFrame*> frames;
    frames.append(mainFrame());
    while (!frames.isEmpty()) {
        QWebFrame *frame = frames.takeFirst();
        if (frame->url() == reply->url()) {
            frame->setHtml(html, reply->url());
            return;
        }
        QList<QWebFrame *> children = frame->childFrames();
        foreach(QWebFrame *frame, children)
            frames.append(frame);
    }
    if (m_loadingUrl == reply->url()) {
        mainFrame()->setHtml(html, reply->url());
    }
}

WebView::WebView(QWidget* parent)
    : QWebView(parent)
    , m_progress(0)
    , m_page(new WebPage(this))
{
    setPage(m_page);
    connect(page(), SIGNAL(statusBarMessage(QString)),
            SLOT(setStatusBarText(QString)));
    connect(this, SIGNAL(loadProgress(int)),
            this, SLOT(setProgress(int)));
    connect(this, SIGNAL(loadFinished(bool)),
            this, SLOT(loadFinished()));
    connect(page(), SIGNAL(loadingUrl(QUrl)),
            this, SIGNAL(urlChanged(QUrl)));
    connect(page(), SIGNAL(downloadRequested(QNetworkRequest)),
            this, SLOT(downloadRequested(QNetworkRequest)));
    page()->setForwardUnsupportedContent(true);

}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu(this);
    QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());

    if (!r.linkUrl().isEmpty()) {
         qDebug() << "Normal Click";
        menu->addAction(pageAction(QWebPage::OpenLinkInNewWindow));
        menu->addAction(tr("Open in New Tab"), this, SLOT(openLinkInNewTab()));
        menu->addSeparator();
        menu->addAction(pageAction(QWebPage::DownloadLinkToDisk));
        // Add link to bookmarks...
        menu->addSeparator();
        menu->addAction(pageAction(QWebPage::CopyLinkToClipboard));
        menu->addAction(tr("Block Image"), this, SLOT(blockImage()))->setData(r.imageUrl().toString());
        if (page()->settings()->testAttribute(QWebSettings::DeveloperExtrasEnabled))
            menu->addAction(pageAction(QWebPage::InspectElement));
        menu->exec(mapToGlobal(event->pos()));
        return;
    }
    delete menu;
    QWebView::contextMenuEvent(event);
}

void WebView::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15;
        setTextSizeMultiplier(textSizeMultiplier() + numSteps * 0.1);
        event->accept();
        return;
    }
    QWebView::wheelEvent(event);
}

void WebView::openLinkInNewTab()
{
    m_page->m_openInNewTab = true;
    pageAction(QWebPage::OpenLinkInNewWindow)->trigger();
}

void WebView::setProgress(int progress)
{
    m_progress = progress;
}

void WebView::loadFinished()
{
    if (100 != m_progress) {
        qWarning() << "Received finished signal while progress is still:" << progress()
                   << "Url:" << url();
    }
    m_progress = 0;

    AdBlockManager::instance()->page()->applyRulesToPage(page());

    //BrowserApplication::instance()->autoFillManager()->fill(page());
}

void WebView::loadUrl(const QUrl &url)
{
    if(url.toString() == "about:home") {

        QFile file("://home.html");
        bool isOpened = file.open(QIODevice::ReadOnly);
        Q_ASSERT(isOpened);
        Q_UNUSED(isOpened)
        page()->mainFrame()->addToJavaScriptWindowObject("js", &js);
        BookmarksManager *manager =  BrowserApplication::bookmarksManager();

        QList<QString> url;

        url = manager->bookmarksTop();

        QString html = QString(QLatin1String(file.readAll()))
                .arg((url.count() >= 1)? "<a href="+url[0]+">"+url[1]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 3)? "<a href="+url[2]+">"+url[3]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 5)? "<a href="+url[4]+">"+url[5]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 7)? "<a href="+url[6]+">"+url[7]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 9)? "<a href="+url[8]+">"+url[9]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 11)? "<a href="+url[10]+">"+url[11]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 13)? "<a href="+url[12]+">"+url[13]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>")
                .arg((url.count() >= 15)? "<a href="+url[14]+">"+url[15]+"</a>" : "<a class='none' onclick='addDialog()'>+</a>");

        page()->mainFrame()->setHtml(html);

        m_initialUrl = "about:home";

        return;
    }

    m_initialUrl = url;
    load(url);
}

QString WebView::lastStatusBarText() const
{
    return m_statusBarText;
}

QUrl WebView::url() const
{
    QUrl url = QWebView::url();
    if (!url.isEmpty())
        return url;

    return m_initialUrl;
}

void WebView::mousePressEvent(QMouseEvent *event)
{
    m_page->m_pressedButtons = event->buttons();
    m_page->m_keyboardModifiers = event->modifiers();
    QWebView::mousePressEvent(event);
}

void WebView::mouseReleaseEvent(QMouseEvent *event)
{
    QWebView::mouseReleaseEvent(event);
    if (!event->isAccepted() && (m_page->m_pressedButtons & Qt::MidButton)) {
        QUrl url(QApplication::clipboard()->text(QClipboard::Selection));
        if (!url.isEmpty() && url.isValid() && !url.scheme().isEmpty()) {
            setUrl(url);
        }
    }
}

void WebView::setStatusBarText(const QString &string)
{
    m_statusBarText = string;
}

void WebView::downloadRequested(const QNetworkRequest &request)
{
    BrowserApplication::downloadManager()->download(request);
}

void WebView::blockImage()
{
    if (QAction *action = qobject_cast<QAction*>(sender())) {
        QString imageUrl = action->data().toString();
        AdBlockDialog *dialog = AdBlockManager::instance()->showDialog();
        dialog->addCustomRule(imageUrl);
    }
}

