#ifndef SM_MODEL6PROXY_H
#define SM_MODEL6PROXY_H

#include <QAbstractProxyModel>

class SM_Model6Proxy : public QAbstractProxyModel
{
    Q_OBJECT
public:
    explicit SM_Model6Proxy(QObject *parent = 0);
    virtual ~SM_Model6Proxy();

    virtual void setSourceModel(QAbstractItemModel *);

    virtual QModelIndex mapFromSource(const QModelIndex &) const;
    virtual QModelIndex mapToSource(const QModelIndex &) const;

    virtual QModelIndex	parent(const QModelIndex &) const;
    virtual QModelIndex index(int, int, const QModelIndex & p = QModelIndex()) const;

    virtual int	rowCount(const QModelIndex & p = QModelIndex()) const;
    virtual int	columnCount(const QModelIndex & p = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;

private slots:
    void source_rowsAboutToBeInserted(QModelIndex, int, int);
    void source_rowsAboutToBeRemoved(QModelIndex, int, int);
    void source_rowsInserted(QModelIndex, int, int);
    void source_rowsRemoved(QModelIndex, int, int);
    void source_dataChanged(QModelIndex, QModelIndex);
    void source_modelReset();

private:

signals:

public slots:

};

#endif // SM_MODEL6PROXY_H
