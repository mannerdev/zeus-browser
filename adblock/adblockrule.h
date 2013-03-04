/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKRULE_H
#define ADBLOCKRULE_H

#include <qstringlist.h>

class QUrl;
class QRegExp;
class AdBlockRule
{

public:
    AdBlockRule(const QString &filter = QString());

    QString filter() const;
    void setFilter(const QString &filter);

    bool isCSSRule() const { return m_cssRule; }
    bool networkMatch(const QString &encodedUrl) const;

    bool isException() const;
    void setException(bool exception);

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QString regExpPattern() const;
    void setPattern(const QString &pattern, bool isRegExp);

private:
    QString m_filter;

    bool m_cssRule;
    bool m_exception;
    bool m_enabled;
    QRegExp m_regExp;
    QStringList m_options;
};

#endif // ADBLOCKRULE_H

