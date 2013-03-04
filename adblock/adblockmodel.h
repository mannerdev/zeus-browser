/*
 * Copyright (c) 2009, Benjamin C. Meyer <ben@meyerhome.net>
 */

#ifndef ADBLOCKMODEL_H
#define ADBLOCKMODEL_H

#include <qabstractitemmodel.h>

class AdBlockRule;
class AdBlockSubscription;
class AdBlockManager;
class AdBlockModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    AdBlockModel(QObject *parent = 0);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const;

    const AdBlockRule rule(const QModelIndex &index) const;
    AdBlockSubscription *subscription(const QModelIndex &index) const;
    QModelIndex index(AdBlockSubscription *subscription);

private slots:
    void rulesChanged();

private:
    AdBlockManager *m_manager;
};

#endif // ADBLOCKMODEL_H

