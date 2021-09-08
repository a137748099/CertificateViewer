
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QIcon>

//! [0]
class TreeItem
{
public:
    explicit TreeItem(const QList<QVariant> &data, const QList<QVariant> &clickData, TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

    QVariant clickData(int column) const;
    QIcon icon() const;
    void setIcon(const QIcon &icon);

private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    QList<QVariant> m_clickData;
    TreeItem *m_parentItem;
    QIcon m_icon;
};
//! [0]

#endif // TREEITEM_H
