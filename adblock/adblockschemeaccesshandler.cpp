/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#include "adblockschemeaccesshandler.h"

#include "adblockmanager.h"
#include "adblocksubscription.h"
#include "adblockdialog.h"

#include <qnetworkrequest.h>
#include <qmessagebox.h>

AdBlockSchemeAccessHandler::AdBlockSchemeAccessHandler(QObject *parent)
    : SchemeAccessHandler(parent)
{
}

QNetworkReply *AdBlockSchemeAccessHandler::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    Q_UNUSED(outgoingData);
    if (op != QNetworkAccessManager::GetOperation)
        return 0;

    if (request.url().path() != QLatin1String("subscribe"))
        return 0;

    AdBlockSubscription *subscription = new AdBlockSubscription(request.url(), AdBlockManager::instance());

    QMessageBox::StandardButton result = QMessageBox::question(0
            , tr("Subscribe?")
            , tr("Subscribe to this AdBlock subscription?\n%1").arg(subscription->title())
            , QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::No) {
        delete subscription;
    } else {
        AdBlockManager::instance()->addSubscription(subscription);
        AdBlockDialog *dialog = AdBlockManager::instance()->showDialog();
        QAbstractItemModel *model = dialog->treeView->model();
        dialog->treeView->setCurrentIndex(model->index(model->rowCount() -1, 0));
        dialog->setFocus();
    }
    return 0;
}

