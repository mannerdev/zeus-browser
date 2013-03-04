/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#include "adblockblockednetworkreply.h"

#include "adblockrule.h"

#include <qnetworkrequest.h>
#include <qtimer.h>

AdBlockBlockedNetworkReply::AdBlockBlockedNetworkReply(const QNetworkRequest &request, const AdBlockRule *rule, QObject *parent)
    : QNetworkReply(parent)
{
    setOperation(QNetworkAccessManager::GetOperation);
    setRequest(request);
    setUrl(request.url());
    setError(QNetworkReply::ContentAccessDenied, tr("Blocked by AdBlockRule: %1").arg(rule->filter()));
    QTimer::singleShot(0, this, SLOT(delayedFinished()));
}

qint64 AdBlockBlockedNetworkReply::readData(char *data, qint64 maxSize)
{
    Q_UNUSED(data);
    Q_UNUSED(maxSize);
    return -1;
}

void AdBlockBlockedNetworkReply::delayedFinished()
{
    emit error(QNetworkReply::ContentAccessDenied);
    emit finished();
}

