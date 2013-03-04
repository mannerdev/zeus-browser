#ifndef SCHEMEACCESSHANDLER_H
#define SCHEMEACCESSHANDLER_H

#include <qobject.h>

#include <qnetworkaccessmanager.h>

class QNetworkReply;
class SchemeAccessHandler : public QObject
{
public:
    SchemeAccessHandler(QObject *parent = 0);

    virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0) = 0;
};

#endif // SCHEMEACCESSHANDLER_H
