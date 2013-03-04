/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKSUBSCRIPTION_H
#define ADBLOCKSUBSCRIPTION_H

#include <qobject.h>

#include "adblockrule.h"

#include <qlist.h>
#include <qdatetime.h>

class QNetworkReply;
class QUrl;
class AdBlockSubscription : public QObject
{
    Q_OBJECT

signals:
    void changed();
    void rulesChanged();

public:
    AdBlockSubscription(const QUrl &url, QObject *parent = 0);
    QUrl url() const;

    bool isEnabled() const;
    void setEnabled(bool enabled);

    QString title() const;
    void setTitle(const QString &title);

    QUrl location() const;
    void setLocation(const QUrl &url);

    void updateNow();
    QDateTime lastUpdate() const;

    void saveRules();

    const AdBlockRule *allow(const QString &urlString) const;
    const AdBlockRule *block(const QString &urlString) const;
    QList<const AdBlockRule*> pageRules() const;

    QList<AdBlockRule> allRules() const;
    void addRule(const AdBlockRule &rule);
    void removeRule(int offset);
    void replaceRule(const AdBlockRule &rule, int offset);

private slots:
    void rulesDownloaded();

private:
    void populateCache();
    QString rulesFileName() const;
    void parseUrl(const QUrl &url);
    void loadRules();

    QByteArray m_url;

    QString m_title;
    QByteArray m_location;
    QDateTime m_lastUpdate;
    bool m_enabled;

    QNetworkReply *m_downloading;
    QList<AdBlockRule> m_rules;

    // sorted list
    QList<const AdBlockRule*> m_networkExceptionRules;
    QList<const AdBlockRule*> m_networkBlockRules;
    QList<const AdBlockRule*> m_pageRules;
};

#endif // ADBLOCKSUBSCRIPTION_H

