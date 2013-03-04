/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKBLOCKEDNETWORKREPLY_H
#define ADBLOCKBLOCKEDNETWORKREPLY_H

#include <qnetworkreply.h>

class AdBlockRule;
class AdBlockBlockedNetworkReply : public QNetworkReply
{
    Q_OBJECT

public:
    AdBlockBlockedNetworkReply(const QNetworkRequest &request, const AdBlockRule *rule, QObject *parent = 0);
    void abort() {};

protected:
    qint64 readData(char *data, qint64 maxSize);

private slots:
    void delayedFinished();

};

#endif // ADBLOCKBLOCKEDNETWORKREPLY_H

