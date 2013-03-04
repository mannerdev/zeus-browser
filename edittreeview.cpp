#include "edittreeview.h"

#include <QtGui/QKeyEvent>

EditTreeView::EditTreeView(QWidget *parent)
    : QTreeView(parent)
{
}

void EditTreeView::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Delete
        || event->key() == Qt::Key_Backspace)
        && model()) {
        removeOne();
    } else {
        QAbstractItemView::keyPressEvent(event);
    }
}

void EditTreeView::removeOne()
{
    if (!model())
        return;
    QModelIndex ci = currentIndex();
    int row = ci.row();
    model()->removeRow(row, ci.parent());
}

void EditTreeView::removeAll()
{
    if (!model())
        return;
    model()->removeRows(0, model()->rowCount(rootIndex()), rootIndex());
}

