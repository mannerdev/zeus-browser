/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKDIALOG_H
#define ADBLOCKDIALOG_H

#include <qdialog.h>
#include "ui_adblockdialog.h"

class AdBlockModel;
class TreeSortFilterProxyModel;
class AdBlockDialog : public QDialog, public Ui_AdBlockDialog
{
    Q_OBJECT

public:
    AdBlockDialog(QWidget *parent = 0);

public slots:
    void addCustomRule(const QString &rule = QString());

private slots:
    void learnAboutWritingFilters();
    void aboutToShowActionMenu();
    void updateSubscription();
    void browseSubscriptions();
    void removeSubscription();

private:
    AdBlockModel *m_adBlockModel;
    TreeSortFilterProxyModel *m_proxyModel;

};

#endif // ADBLOCKDIALOG_H

