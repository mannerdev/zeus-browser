/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKMANAGER_H
#define ADBLOCKMANAGER_H

#include <qobject.h>

#include <qpointer.h>

class QUrl;
class AutoSaver;
class AdBlockDialog;
class AdBlockNetwork;
class AdBlockPage;
class AdBlockSubscription;
class AdBlockManager : public QObject
{
    Q_OBJECT

signals:
    void rulesChanged();

public:
    AdBlockManager(QObject *parent = 0);
    ~AdBlockManager();

    void load();

    static AdBlockManager *instance();
    bool isEnabled() const;

    QList<AdBlockSubscription*> subscriptions() const;
    void removeSubscription(AdBlockSubscription *subscription);
    void addSubscription(AdBlockSubscription *subscription);

    AdBlockNetwork *network();
    AdBlockPage *page();
    AdBlockSubscription *customRules();

public slots:
    void setEnabled(bool enabled);
    AdBlockDialog *showDialog();

private slots:
    void save();

private:
    static QUrl customSubscriptionUrl();
    static AdBlockManager *s_adBlockManager;

    bool m_loaded;
    bool m_enabled;
    AutoSaver *m_saveTimer;
    QPointer<AdBlockDialog> m_adBlockDialog;
    AdBlockNetwork *m_adBlockNetwork;
    AdBlockPage *m_adBlockPage;
    QList<AdBlockSubscription*> m_subscriptions;

};

#endif // ADBLOCKMANAGER_H

