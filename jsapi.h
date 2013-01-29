#ifndef JSAPI_H
#define JSAPI_H

#include <QObject>
#include <QString>
#include <QtWebKit>

class JsAPI : public QObject {
       Q_OBJECT

public Q_SLOTS:
    void addTop8(const QString &url, const QString &title);
    void test();
};

#endif // JSAPI_H
