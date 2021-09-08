
#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

//! [0]
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(const QStringList &dataList, const QStringList &clickDataList, const QString &title, QObject *parent = 0);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant clickData(const QModelIndex &index);
    void setIcon(const QModelIndex &index, const QIcon &icon);

private:
    void setupModelData(const QStringList &lines, TreeItem *parent);

    TreeItem *rootItem;
    QMap<QString, QString> m_dataMap;
};
//! [0]

#endif // TREEMODEL_H
