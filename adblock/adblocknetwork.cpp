/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#include "adblocknetwork.h"

#include "adblockblockednetworkreply.h"
#include "adblockmanager.h"
#include "adblocksubscription.h"

#include <qdebug.h>

// #define ADBLOCKNETWORK_DEBUG

AdBlockNetwork::AdBlockNetwork(QObject *parent)
    : QObject(parent)
{
}

QNetworkReply *AdBlockNetwork::block(const QNetworkRequest &request)
{
    QUrl url = request.url();

    if (url.scheme() == QLatin1String("data"))
        return 0;

    AdBlockManager *manager = AdBlockManager::instance();
    if (!manager->isEnabled())
        return 0;

    QString urlString = QString::fromUtf8(url.toEncoded());
    const AdBlockRule *blockedRule = 0;
    const AdBlockSubscription *blockingSubscription = 0;

    QList<AdBlockSubscription*> subscriptions = manager->subscriptions();
    foreach (AdBlockSubscription *subscription, subscriptions) {
        if (subscription->allow(urlString))
            return 0;

        if (const AdBlockRule *rule = subscription->block(urlString)) {
            blockedRule = rule;
            blockingSubscription = subscription;
            break;
        }
    }

    if (blockedRule) {
#if defined(ADBLOCKNETWORK_DEBUG)
        qDebug() << "AdBlockNetwork::" << __FUNCTION__ << "rule:" << blockedRule->filter() << "subscription:" << blockingSubscription->title() << url;
#endif
       AdBlockBlockedNetworkReply *reply = new AdBlockBlockedNetworkReply(request, blockedRule, this);
        return reply;
    }
    return 0;
}

