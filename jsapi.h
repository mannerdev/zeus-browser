#ifndef JSAPI_H
#define JSAPI_H

#include <QObject>
#include <QString>

class JsAPI : QObject
{
    Q_OBJECT
public slots:
    void addTop8(const QString &url, int id );
};

#endif // JSAPI_H
