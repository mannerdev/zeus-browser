/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#include "adblockdialog.h"

#include "adblockmodel.h"
#include "adblockmanager.h"
#include "adblocksubscription.h"
#include "treesortfilterproxymodel.h"

#include <qdesktopservices.h>
#include <qmenu.h>
#include <qurl.h>

#include <qdebug.h>

AdBlockDialog::AdBlockDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    m_adBlockModel = new AdBlockModel(this);
    m_proxyModel = new TreeSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_adBlockModel);
    treeView->setModel(m_proxyModel);
    connect(search, SIGNAL(textChanged(QString)),
            m_proxyModel, SLOT(setFilterFixedString(QString)));

    AdBlockManager *manager = AdBlockManager::instance();
    adblockCheckBox->setChecked(manager->isEnabled());
    connect(adblockCheckBox, SIGNAL(toggled(bool)),
            AdBlockManager::instance(), SLOT(setEnabled(bool)));

    QMenu *menu = new QMenu(this);
    connect(menu, SIGNAL(aboutToShow()),
            this, SLOT(aboutToShowActionMenu()));
    actionToolButton->setMenu(menu);
    actionToolButton->setIcon(QIcon(QLatin1String(":128x128/run.png")));
    actionToolButton->setPopupMode(QToolButton::InstantPopup);

    AdBlockSubscription *subscription = manager->customRules();
    QModelIndex subscriptionIndex = m_adBlockModel->index(subscription);
    treeView->expand(m_proxyModel->mapFromSource(subscriptionIndex));
}

void AdBlockDialog::aboutToShowActionMenu()
{
    QMenu *menu = actionToolButton->menu();
    menu->clear();

    QAction *addRule = menu->addAction(tr("Add Custom Rule"));
    connect(addRule, SIGNAL(triggered()), this, SLOT(addCustomRule()));

    QAction *learnRule = menu->addAction(tr("Learn more about writing rules..."));
    connect(learnRule, SIGNAL(triggered()), this, SLOT(learnAboutWritingFilters()));

    menu->addSeparator();

    QModelIndex idx = m_proxyModel->mapToSource(treeView->currentIndex());

    QAction *updateSubscription = menu->addAction(tr("Update Subscription"));
    connect(updateSubscription, SIGNAL(triggered()), this, SLOT(updateSubscription()));
    if (!idx.isValid())
        updateSubscription->setEnabled(false);

    QAction *addSubscription = menu->addAction(tr("Browse Subscriptions..."));
    connect(addSubscription, SIGNAL(triggered()), this, SLOT(browseSubscriptions()));

    menu->addSeparator();

    QAction *removeSubscription = menu->addAction(tr("Remove Subscription"));
    connect(removeSubscription, SIGNAL(triggered()), this, SLOT(removeSubscription()));
    if (!idx.isValid())
        removeSubscription->setEnabled(false);
}

void AdBlockDialog::addCustomRule(const QString &rule)
{
    AdBlockManager *manager = AdBlockManager::instance();
    AdBlockSubscription *subscription = manager->customRules();
    Q_ASSERT(subscription);
    subscription->addRule(AdBlockRule(rule));
    // reset the model
    qApp->processEvents();

    QModelIndex parent = m_adBlockModel->index(subscription);
    int x = m_adBlockModel->rowCount(parent);
    QModelIndex ruleIndex = m_adBlockModel->index(x - 1, 0, parent);
    treeView->expand(m_proxyModel->mapFromSource(parent));
    treeView->edit(m_proxyModel->mapFromSource(ruleIndex));
}

void AdBlockDialog::updateSubscription()
{
    QModelIndex idx = m_proxyModel->mapToSource(treeView->currentIndex());
    if (!idx.isValid())
        return;
    if (idx.parent().isValid())
        idx = idx.parent();
    AdBlockSubscription *subscription = (AdBlockSubscription*)m_adBlockModel->subscription(idx);
    subscription->updateNow();
}

void AdBlockDialog::browseSubscriptions()
{
    QUrl url(QLatin1String("http://adblockplus.org/en/subscriptions"));
    QDesktopServices::openUrl(url);
}

void AdBlockDialog::learnAboutWritingFilters()
{
    QUrl url(QLatin1String("http://adblockplus.org/en/filters"));
    QDesktopServices::openUrl(url);
}

void AdBlockDialog::removeSubscription()
{
    QModelIndex idx = m_proxyModel->mapToSource(treeView->currentIndex());
    if (!idx.isValid())
        return;
    if (idx.parent().isValid())
        idx = idx.parent();
    AdBlockSubscription *subscription = (AdBlockSubscription*)m_adBlockModel->subscription(idx);
    AdBlockManager::instance()->removeSubscription(subscription);
}

