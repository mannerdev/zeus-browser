/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKNETWORK_H
#define ADBLOCKNETWORK_H

#include <qobject.h>

class QNetworkRequest;
class QNetworkReply;
class AdBlockNetwork : public QObject
{
    Q_OBJECT

public:
    AdBlockNetwork(QObject *parent = 0);

    QNetworkReply *block(const QNetworkRequest &request);

};

#endif // ADBLOCKNETWORK_H

