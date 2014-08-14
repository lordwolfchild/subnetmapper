#include "sm_model6proxy.h"

SM_Model6Proxy::SM_Model6Proxy(QObject *parent) :
    QAbstractProxyModel(parent)
{
    qDebug("Instance of SM_Model6Proxy constructed.");
}

SM_Model6Proxy::~SM_Model6Proxy()
{
}

void SM_Model6Proxy::setSourceModel(QAbstractItemModel * m)
{
    QAbstractProxyModel::setSourceModel(m);

    connect(m,  SIGNAL(rowsAboutToBeInserted(QModelIndex, int, int)),
            this, SLOT(s_rowsAboutToBeInserted(QModelIndex, int, int)));
    connect(m,  SIGNAL(rowsInserted(QModelIndex, int, int)),
            this, SLOT(s_rowsInserted(QModelIndex, int, int)));

    connect(m,  SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
            this, SLOT(s_rowsAboutToBeRemoved(QModelIndex, int, int)));
    connect(m,  SIGNAL(rowsRemoved(QModelIndex, int, int)),
            this, SLOT(s_rowsRemoved(QModelIndex, int, int)));

    connect(m,  SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this, SLOT(s_dataChanged(QModelIndex,QModelIndex)));
    connect(m,  SIGNAL(modelReset()),
            this, SLOT(s_modelReset()));

    connect(m,    SIGNAL(layoutAboutToBeChanged()),
            this, SIGNAL(layoutAboutToBeChanged()));
    connect(m,    SIGNAL(layoutChanged()),
            this, SIGNAL(layoutChanged()));

    connect(m,  SIGNAL(destroyed(QObject *)),
            this, SLOT(s_destroyed(QObject *)));

    qDebug("Count of new Model proxied: %d",m->rowCount());


}

QModelIndex SM_Model6Proxy::mapFromSource(const QModelIndex & source) const
{
    return source;
}

QModelIndex SM_Model6Proxy::mapToSource(const QModelIndex & proxy) const
{
    return proxy;
}

QModelIndex	SM_Model6Proxy::parent(const QModelIndex & p) const
{
    return sourceModel()->parent(p);
}

QModelIndex SM_Model6Proxy::index(int row, int column, const QModelIndex & p) const
{
    return sourceModel()->index(row, column, p);
}

int	SM_Model6Proxy::rowCount(const QModelIndex & p) const
{
    return sourceModel()->rowCount(p);
}

int	SM_Model6Proxy::columnCount(const QModelIndex & p) const
{
    if (p.isValid()) return 0;
    if (!sourceModel()) return 0;
    return sourceModel()->columnCount();
}

void SM_Model6Proxy::source_rowsAboutToBeInserted(QModelIndex p, int from, int to)
{


}

void SM_Model6Proxy::source_rowsInserted(QModelIndex p, int, int)
{
    if (!p.isValid())
        return;

}


void SM_Model6Proxy::source_rowsAboutToBeRemoved(QModelIndex p, int from, int to)
{

}

void SM_Model6Proxy::source_rowsRemoved(QModelIndex p, int, int)
{
}

void SM_Model6Proxy::source_dataChanged(QModelIndex tl, QModelIndex br)
{
    QModelIndex p_tl = mapFromSource(tl);
    QModelIndex p_br = mapFromSource(br);
    emit dataChanged(p_tl, p_br);
}

QVariant SM_Model6Proxy::data(const QModelIndex &index, int role) const
{
    return sourceModel()->data(index,role);
}

void SM_Model6Proxy::source_modelReset()
{
    resetInternalData();
}
