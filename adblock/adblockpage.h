/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKPAGE_H
#define ADBLOCKPAGE_H

#include <qobject.h>

class AdBlockRule;
class QWebPage;
class AdBlockPage : public QObject
{
    Q_OBJECT

public:
    AdBlockPage(QObject *parent = 0);

    void applyRulesToPage(QWebPage *page);

private:
    void checkRule(const AdBlockRule *rule, QWebPage *page, const QString &host);
};

#endif // ADBLOCKPAGE_H

