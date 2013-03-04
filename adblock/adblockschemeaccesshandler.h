/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKSCHEMEACCESSHANDLER_H
#define ADBLOCKSCHEMEACCESSHANDLER_H

#include "schemeaccesshandler.h"

class AdBlockSchemeAccessHandler : public SchemeAccessHandler
{
public:
    AdBlockSchemeAccessHandler(QObject *parent = 0);

    virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0);
};

#endif // ADBLOCKSCHEMEACCESSHANDLER_H
