/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#include "adblockpage.h"

#include "adblockmanager.h"
#include "adblocksubscription.h"
#include "adblockrule.h"

#if QT_VERSION >= 0x040600
#include <qwebelement.h>
#endif
#include <qwebpage.h>
#include <qwebframe.h>

#include <qdebug.h>

// #define ADBLOCKPAGE_DEBUG

AdBlockPage::AdBlockPage(QObject *parent)
    : QObject(parent)
{
}

void AdBlockPage::checkRule(const AdBlockRule *rule, QWebPage *page, const QString &host)
{
    if (!rule->isEnabled())
        return;

    QString filter = rule->filter();
    int offset = filter.indexOf(QLatin1String("##"));
    if (offset == -1)
        return;

    QString selectorQuery;
    if (offset > 0) {
        QString domainRules = filter.mid(0, offset);
        selectorQuery = filter.mid(offset + 2);
        QStringList domains = domainRules.split(QLatin1Char(','));

        bool match = false;
        foreach (const QString &domain, domains) {
            bool reverse = (domain[0] == QLatin1Char('~'));
            if (reverse) {
                QString xdomain = domain.mid(1);
                if (host.endsWith(xdomain))
                    return;
                match = true;
            }
            if (host.endsWith(domain))
                match = true;
        }
        if (!match)
            return;
    }

    if (offset == 0)
        selectorQuery = filter.mid(2);

    Q_UNUSED(page);
#if QT_VERSION >= 0x040600
    QWebElement document = page->mainFrame()->documentElement();
    QWebElementCollection elements = document.findAll(selectorQuery);
#if defined(ADBLOCKPAGE_DEBUG)
    if (elements.count() != 0)
        qDebug() << "AdBlockPage::" << __FUNCTION__ << "blocking" << elements.count() << "items" << selectorQuery << elements.count() << "rule:" << rule->filter();
#endif
    foreach (QWebElement element, elements) {
        element.setStyleProperty(QLatin1String("visibility"), QLatin1String("hidden"));
        element.removeFromDocument();
    }

#endif
}

void AdBlockPage::applyRulesToPage(QWebPage *page)
{
    if (!page || !page->mainFrame())
        return;
    AdBlockManager *manager = AdBlockManager::instance();
    if (!manager->isEnabled())
        return;
#if QT_VERSION >= 0x040600
    QString host = page->mainFrame()->url().host();
    QList<AdBlockSubscription*> subscriptions = manager->subscriptions();
    foreach (AdBlockSubscription *subscription, subscriptions) {
        QList<const AdBlockRule*> rules = subscription->pageRules();
        foreach (const AdBlockRule *rule, rules) {
            checkRule(rule, page, host);
        }
    }
#endif
}

