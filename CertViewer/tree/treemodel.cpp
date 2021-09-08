
#include "treeitem.h"
#include "treemodel.h"

#include <QStringList>
#include <QDebug>


//! [0]
TreeModel::TreeModel(const QStringList &dataList, const QStringList &clickDataList, const QString &title,  QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData = {title};
    QList<QVariant> rootClickData;
    rootItem = new TreeItem(rootData, rootClickData);

    for(int i = 0; i < dataList.size(); ++i) {
        m_dataMap.insert(dataList.at(i).trimmed(), clickDataList.at(i));
    }
    setupModelData(dataList, rootItem);

//    setupModelData(data.split(QString("\n")), rootItem);
}
//! [0]

//! [1]
TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModel::columnCount(const QModelIndex &parent) const
{
//    if (parent.isValid())
//        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
//    else
//        return rootItem->columnCount();
    return 1;
}

QVariant TreeModel::clickData(const QModelIndex &index)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->clickData(index.column());
}

void TreeModel::setIcon(const QModelIndex &index, const QIcon &icon)
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if(item->childCount() <= 0)
        return;
    item->setIcon(icon);
}
//! [2]

//! [3]
QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    else if(role == Qt::DecorationRole) {
        return item->icon();
    }
    else
        return QVariant();


}
//! [3]

//! [4]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//! [5]

//! [6]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//! [8]

void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            QList<QVariant> clickData;
            for (int column = 0; column < columnStrings.count(); ++column) {
                QString data = columnStrings[column];
                columnData << data;
//                qDebug() << __FUNCTION__ << "song" << m_dataMap.value(data) <<data;
                clickData << m_dataMap.value(data);
            }

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            TreeItem *treeItem = new TreeItem(columnData, clickData, parents.last());
//            treeItem->setIcon(QIcon(":/resource/folder_closed.png"));
            parents.last()->appendChild(treeItem);
        }

        ++number;
    }
    qDebug() << __FUNCTION__ << "song";
}
