#ifndef BROWSERAPPLICATION_H
#define BROWSERAPPLICATION_H

#include <QApplication>

#include <QtCore/QUrl>
#include <QtCore/QPointer>

#include <QtGui/QIcon>

QT_BEGIN_NAMESPACE
class QLocalServer;
QT_END_NAMESPACE

class BookmarksManager;
class BrowserMainWindow;
class CookieJar;
class DownloadManager;
class HistoryManager;
class NetworkAccessManager;
class BrowserApplication : public QApplication
{
    Q_OBJECT

public:
    BrowserApplication(int &argc, char **argv);
    ~BrowserApplication();
    static BrowserApplication *instance();
    void loadSettings();

    bool isTheOnlyBrowser() const;
    BrowserMainWindow *mainWindow();
    QList<BrowserMainWindow*> mainWindows();
    QIcon icon(const QUrl &url) const;

    void saveSession();
    bool canRestoreSession() const;

    static HistoryManager *historyManager();
    static CookieJar *cookieJar();
    static DownloadManager *downloadManager();
    static NetworkAccessManager *networkAccessManager();
    static BookmarksManager *bookmarksManager();

    static QString dataFilePath(const QString &fileName);

#if defined(Q_WS_MAC)
    bool event(QEvent *event);
#endif

public slots:
    BrowserMainWindow *newMainWindow();
    void restoreLastSession();
#if defined(Q_WS_MAC)
    void lastWindowClosed();
    void quitBrowser();
#endif

private slots:
    void postLaunch();
    void openUrl(const QUrl &url);
    void newLocalSocketConnection();

private:
    void clean();
    void installTranslator(const QString &name);

    static HistoryManager *s_historyManager;
    static DownloadManager *s_downloadManager;
    static NetworkAccessManager *s_networkAccessManager;
    static BookmarksManager *s_bookmarksManager;

    QList<QPointer<BrowserMainWindow> > m_mainWindows;
    QLocalServer *m_localServer;
    QByteArray m_lastSession;
    mutable QIcon m_defaultIcon;
};

#endif // BROWSERAPPLICATION_H

