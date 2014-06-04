#include "tetrismodel.h"
#include <QTableView>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QMessageBox>
#include <stdexcept>
#include <QHeaderView>

TetrisModel::TetrisModel(int columnsCount, int rowsCount, QObject *parent):
    QAbstractItemModel(parent),
    m_rowsCount(rowsCount),
    m_columnsCount(columnsCount),
    m_score(0),
    m_timer(NULL)
{

    for (int i=0; i < rowCount(); ++i) {
        QVector<int> row;
        for (int j=0; j < columnCount(); ++j)
            row << 0;
        m_matrix << row;
    }
    QTableView *view = static_cast<QTableView *>(parent);
    view->installEventFilter(this);
    view->setItemDelegate(new TetrisDelegate(view));
    view->setFocus();
    //view->setStyleSheet("background-color: gray;");
    m_currentShape << QModelIndex()
                   << QModelIndex()
                   << QModelIndex()
                   << QModelIndex();
    m_typeCurrentShape = NullShape;
    m_typeNextShape = NullShape;
    view->verticalHeader()->setDefaultSectionSize(view->size().height()/(m_rowsCount+1));
    view->horizontalHeader()->setDefaultSectionSize(view->size().height()/(m_rowsCount+1));
}

TetrisModel::~TetrisModel()
{
    m_matrix.clear();
}

QVariant TetrisModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QVariant v;
    if (role  == Qt::DisplayRole) {
        if (index.column() < m_columnsCount ||
           (index.row() > 7 && index.column() > m_columnsCount+2))
            v = m_matrix[index.row()][index.column()];
        else if (index.column() == m_columnsCount ||
                (index.row() == 4 && index.column() > m_columnsCount))
            v = 5;
        else if (index.row() == 1 && index.column() > m_columnsCount+2)
        {
            QString score = tr("Score:");
            int i = index.column()-(m_columnsCount+2);
            if (score.size() > i-1)
                v = score.at(i-1);
        }
        else if (index.row() == 2 && index.column() > m_columnsCount+2)
        {
            QString score = QString::number(m_score);
            int i = index.column()-(m_columnsCount+2);
            if (score.size() > i-1)
                v = score.at(i-1);
        }
        else if (index.row() == 6 && index.column() > m_columnsCount+2)
        {
            QString score = tr("Next:");
            int i = index.column()-(m_columnsCount+2);
            if (score.size() > i-1)
                v = score.at(i-1);
        }
    }
    return (!v.toInt()) ? QVariant() : v;
}

bool TetrisModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()) {
        m_matrix[index.row()][index.column()] = value.toInt();
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags TetrisModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QVariant TetrisModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

QModelIndex TetrisModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    return createIndex(row, column);
}

QModelIndex TetrisModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

int TetrisModel::rowCount(const QModelIndex &parent) const
{
    return m_rowsCount;
}

int TetrisModel::columnCount(const QModelIndex &parent) const
{
    return m_columnsCount + 10;
}

bool TetrisModel::insertRows(int position, int rows, const QModelIndex &index)
{
    beginInsertRows(index, position, position+rows-1);
    QVector<int> row;
    for (int i=0; i < m_columnsCount; ++i)
        row << 0;
    m_matrix.insert(position, row);
    endInsertRows();
    return true;
}

bool TetrisModel::removeRows(int position, int rows, const QModelIndex &index)
{
    beginRemoveRows(index, position, position+rows-1);
    m_matrix.remove(position);
    endRemoveRows();
    return true;
}

bool TetrisModel::isValidShape(QVector<QModelIndex> _shape) const
{
    foreach (QModelIndex index, _shape) {
        if (index.column() >= m_columnsCount ||
            index.row() >= m_rowsCount ||
            index.column() < 0)
            return false;
    }
    return true;
}

QVector<QModelIndex> TetrisModel::createTShape(QModelIndex _center, int _state, bool _ignoreValidCheck) const
{
    QVector<QModelIndex> newShape;
    if (_state == LeftState)
        newShape << _center
                 << createIndex(_center.row()+1,_center.column())
                 << createIndex(_center.row()+1,_center.column()-1)
                 << createIndex(_center.row()+1,_center.column()+1);
    else if (_state == UpState)
        newShape << _center
                 << createIndex(_center.row()-1,_center.column()-1)
                 << createIndex(_center.row()+1,_center.column()-1)
                 << createIndex(_center.row(),_center.column()-1);
    else if (_state == RightState)
        newShape << _center
                 << createIndex(_center.row()-1,_center.column())
                 << createIndex(_center.row()-1,_center.column()+1)
                 << createIndex(_center.row()-1,_center.column()-1);
    else if (_state == DownState)
        newShape << _center
                 << createIndex(_center.row(),_center.column()+1)
                 << createIndex(_center.row()+1,_center.column()+1)
                 << createIndex(_center.row()-1,_center.column()+1);
    if (!_ignoreValidCheck && !isValidShape(newShape))
        throw std::runtime_error("invalid shape");
    return newShape;
}

QVector<QModelIndex> TetrisModel::createLineShape(QModelIndex _center, int _state, bool _ignoreValidCheck) const
{
    QVector<QModelIndex> newShape;
    if (_state == LeftState || _state == RightState)
        newShape << _center
                 << createIndex(_center.row(), _center.column()-1)
                 << createIndex(_center.row(), _center.column()+1)
                 << createIndex(_center.row(), _center.column()+2);
    else if (_state == UpState || _state == DownState)
        newShape << _center
                 << createIndex(_center.row()-1,_center.column())
                 << createIndex(_center.row()+1,_center.column())
                 << createIndex(_center.row()+2,_center.column());
    if (!_ignoreValidCheck && !isValidShape(newShape))
        throw std::runtime_error("invalid shape");
    return newShape;
}

QVector<QModelIndex> TetrisModel::createLineWithTallShape(QModelIndex _center, int _state, bool _ignoreValidCheck) const
{
    QVector<QModelIndex> newShape;
    if (_state == LeftState)
        newShape << _center
                 << createIndex(_center.row(), _center.column()-1)
                 << createIndex(_center.row(), _center.column()+1)
                 << createIndex(_center.row()-1, _center.column()+1);
    else if (_state == UpState)
        newShape << _center
                 << createIndex(_center.row()-1,_center.column())
                 << createIndex(_center.row()+1,_center.column())
                 << createIndex(_center.row()+1,_center.column()+1);
    else if (_state == RightState)
        newShape << _center
                 << createIndex(_center.row(),_center.column()-1)
                 << createIndex(_center.row(),_center.column()+1)
                 << createIndex(_center.row()+1,_center.column()-1);
    else if (_state == DownState)
        newShape << _center
                 << createIndex(_center.row()+1,_center.column())
                 << createIndex(_center.row()-1,_center.column())
                 << createIndex(_center.row()-1,_center.column()-1);

    if (!_ignoreValidCheck && !isValidShape(newShape))
        throw std::runtime_error("invalid shape");
    return newShape;
}

QVector<QModelIndex> TetrisModel::createCubeShape(QModelIndex _center) const
{
    QVector<QModelIndex> newShape;
    newShape << _center
             << createIndex(_center.row(),_center.column()+1)
             << createIndex(_center.row()+1,_center.column())
             << createIndex(_center.row()+1,_center.column()+1);
    return newShape;
}

void TetrisModel::rotateTShape()
{
    QVector <QModelIndex> tmp = m_currentShape;

    try {
        if (m_shapeState == LeftState) {
            m_currentShape = createTShape(m_currentShape.first(), UpState);
            m_shapeState = UpState;
        } else if (m_shapeState == UpState) {
            m_currentShape = createTShape(m_currentShape.first(), RightState);
            m_shapeState = RightState;
        } else if (m_shapeState == RightState) {
            m_currentShape = createTShape(m_currentShape.first(), DownState);
            m_shapeState = DownState;
        } else if (m_shapeState == DownState) {
            m_currentShape = createTShape(m_currentShape.first(), LeftState);
            m_shapeState = LeftState;
        }

        foreach (QModelIndex index, tmp)
            setData(index, QVariant(0));
        foreach (QModelIndex index, m_currentShape)
        setData(index, QVariant(1));
    } catch (const std::runtime_error &err) { }
}

void TetrisModel::rotateLineShape()
{
    QVector <QModelIndex> tmp = m_currentShape;

    try {
        if (m_shapeState == LeftState || m_shapeState == RightState) {
            m_currentShape = createLineShape(m_currentShape.first(), UpState);
            m_shapeState = UpState;
        }
        else if (m_shapeState == UpState || m_shapeState == DownState) {
            m_currentShape = createLineShape(m_currentShape.first(), LeftState);
            m_shapeState = LeftState;
        }

        foreach (QModelIndex index, tmp)
            setData(index, QVariant(0));
        foreach (QModelIndex index, m_currentShape)
            setData(index, QVariant(1));
    } catch (const std::runtime_error &err) { }
 }

void TetrisModel::rotateLineWithTallShape()
{
    QVector <QModelIndex> tmp = m_currentShape;

    try {
        if (m_shapeState == LeftState) {
            m_currentShape = createLineWithTallShape(m_currentShape.first(), UpState);
            m_shapeState = UpState;
        } else if (m_shapeState == UpState) {
            m_currentShape = createLineWithTallShape(m_currentShape.first(), RightState);
            m_shapeState = RightState;
        } else if (m_shapeState == RightState) {
            m_currentShape = createLineWithTallShape(m_currentShape.first(), DownState);
            m_shapeState = DownState;
        } else if (m_shapeState == DownState) {
            m_currentShape = createLineWithTallShape(m_currentShape.first(), LeftState);
            m_shapeState = LeftState;
        }

        foreach (QModelIndex index, tmp)
            setData(index, QVariant(0));
        foreach (QModelIndex index, m_currentShape)
            setData(index, QVariant(1));
    } catch (const std::runtime_error &err) { }
}

void TetrisModel::rotateShape()
{
    switch (m_typeCurrentShape) {
        case TShape:
            rotateTShape();
            break;
        case LineShape:
            rotateLineShape();
            break;
        case LineWithTallShape:
            rotateLineWithTallShape();
            break;
        default:
            return;
    }
}

void TetrisModel::createShape(QModelIndex _center, int _state)
{
   if (m_typeNextShape == NullShape) {
        if (RandomState == _state)
            _state = rand()%5;
        m_shapeState = _state;
        m_typeCurrentShape = rand()%5;
    } else {
        m_shapeState = m_nextShapeState;
        m_typeCurrentShape = m_typeNextShape;
    }

    m_nextShapeState = rand()%5;
    m_typeNextShape = rand()%5;

    for (int i=7; i < rowCount(); ++i)
         for (int j=m_columnsCount+2; j < columnCount(); ++j)
            setData(createIndex(i, j), 0);

    QModelIndex legendCenter = createIndex(10, m_columnsCount+5);

    switch (m_typeNextShape) {
        case TShape:
            m_nextShape = createTShape(legendCenter, m_nextShapeState, true);
            break;
        case LineShape:
            m_nextShape = createLineShape(legendCenter, m_nextShapeState, true);
            break;
        case LineWithTallShape:
            m_nextShape = createLineWithTallShape(legendCenter, m_nextShapeState, true);
            break;
        case CubeShape:
            m_nextShape = createCubeShape(legendCenter);
            break;
        default:
            return;
    }
    foreach (QModelIndex i, m_nextShape)
        setData(i, 5);

    switch (m_typeCurrentShape) {
        case TShape:
            m_currentShape = createTShape(_center, m_shapeState);
            break;
        case LineShape:
            m_currentShape = createLineShape(_center, m_shapeState);
            break;
        case LineWithTallShape:
            m_currentShape = createLineWithTallShape(_center, m_shapeState);
            break;
        case CubeShape:
            m_currentShape = createCubeShape(_center);
            break;
        default:
            return;
    }
    bool gameOver = false;
    foreach (QModelIndex index, m_currentShape) {
        if (data(index).toInt() > 0 && !gameOver) {
            delete m_timer;
            m_timer = NULL;
            //disconnect(m_timer, 0, 0, 0);
            gameOver = true;
        }
    }

    if (gameOver) {
        QMessageBox::information(0, "Bams",
                                "Game over..\n Score: " + QString::number(m_score),
                                QMessageBox::Ok);
        return;
    }

    foreach (QModelIndex index, m_currentShape)
        setData(index, QVariant(1));

    if (!m_timer) {
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), SLOT(shapeMovingDown()));
        m_timer->start(500);
    }
}

bool TetrisModel::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Right) {
            shapeMovingRight();
            return true;
        }
        if (ke->key() == Qt::Key_Left) {
            shapeMovingLeft();
            return true;
        }
        if (ke->key() == Qt::Key_Down) {
            shapeMovingDown();
            return true;
        }
        if (ke->key() == Qt::Key_Space) {
            rotateShape();
            return true;
        }

    }
    return false;
}

void TetrisModel::startNewGame()
{
    clear();
    createShape(createIndex(0, m_columnsCount/2-1));
}

void TetrisModel::clear()
{
    m_currentShape.clear();
    m_score = 0;
    for (int i=0; i < rowCount(); ++i) {
        for (int j=0; j < m_columnsCount; ++j)
            setData(createIndex(i, j), 0);
    }
}

bool TetrisModel::isFreePlaceForNextDownStep() const
{
    QHash<int, QModelIndex> downIndexes;
    foreach (QModelIndex index, m_currentShape) {
        if (!downIndexes.contains(index.column()) && index.row()+1 < rowCount())
            downIndexes[index.column()] = createIndex(index.row()+1, index.column());
        else if (downIndexes[index.column()].row() < index.row()+1 && index.row()+1 < rowCount())
            downIndexes[index.column()] = createIndex(index.row()+1, index.column());
    }
    return isFreePlace(downIndexes.values());
}

bool TetrisModel::isFreePlaceForNextRightStep() const
{
    QHash<int, QModelIndex> rightIndexes;
    foreach (QModelIndex index, m_currentShape) {
        if (!rightIndexes.contains(index.row()) && index.column()+1 < m_columnsCount)
            rightIndexes[index.row()] = createIndex(index.row(), index.column()+1);
        else if (rightIndexes[index.row()].column() < index.column()+1 && index.column()+1 < m_columnsCount)
            rightIndexes[index.row()] = createIndex(index.row(), index.column()+1);
    }
    return isFreePlace(rightIndexes.values());
}

bool TetrisModel::isFreePlaceForNextLeftStep() const
{
    QHash<int, QModelIndex> leftIndexes;
    foreach (QModelIndex index, m_currentShape) {
        if (!leftIndexes.contains(index.row()) && index.column()-1 >= 0)
            leftIndexes[index.row()] = createIndex(index.row(), index.column()-1);
        else if (leftIndexes[index.row()].column() > index.column()-1 && index.column()-1 >= 0)
            leftIndexes[index.row()] = createIndex(index.row(), index.column()-1);
    }
    return isFreePlace(leftIndexes.values());
}

bool TetrisModel::isFreePlace(const QList<QModelIndex> &place) const
{
    if (place.isEmpty())
        return false;
    foreach (QModelIndex index, place) {
        if (index.data().toInt() > 0 ||
            index.column() >= m_columnsCount ||
            index.column() < 0 ||
            index.row() >= rowCount())
            return false;
    }
    return true;
}

void TetrisModel::Yeaaaah(const QVector<int> &_fullRows)
{
    foreach (int row, _fullRows) {
        removeRows(row, 1);
        insertRows(0, 1);
    }
    m_score += _fullRows.size() * m_columnsCount;
}

QVector<int> TetrisModel::checkFullRows()
{
    QVector<int> fullRows;
    for (int i=0; i < m_rowsCount; ++i) {
        bool isFullRow = true;
        for (int j=0; j < m_columnsCount; ++j) {
            if (data(createIndex(i, j)).toInt() == 0)
                isFullRow = false;
        }
        if (isFullRow)
            fullRows << i;
    }
    return fullRows;
}

void TetrisModel::shapeMovingDown()
{
    if (!isFreePlaceForNextDownStep()) {
        QVector<int> fullRows = checkFullRows();
        if (!fullRows.isEmpty())
            Yeaaaah(fullRows);
        createShape(createIndex(0, m_columnsCount/2-1));
    }
    else {
        foreach (QModelIndex index, m_currentShape)
            setData(index, QVariant(0));

        QVector<QModelIndex> newShape;
        foreach (QModelIndex index, m_currentShape) {
            newShape << createIndex(index.row()+1, index.column());
            setData(newShape.last(), QVariant(1));
        }
        m_currentShape = newShape;
    }
}

void TetrisModel::shapeMovingLeft()
{
    if (!isFreePlaceForNextLeftStep())
        return;
    foreach (QModelIndex index, m_currentShape)
        setData(index, QVariant(0));

    QVector<QModelIndex> newShape;
    foreach (QModelIndex index, m_currentShape) {
        newShape << createIndex(index.row(), index.column()-1);
        setData(newShape.last(), QVariant(1));
    }
    m_currentShape = newShape;
}

void TetrisModel::shapeMovingRight()
{
    if (!isFreePlaceForNextRightStep())
        return;
    foreach (QModelIndex index, m_currentShape)
        setData(index, QVariant(0));

    QVector<QModelIndex> newShape;
    foreach (QModelIndex index, m_currentShape) {
        newShape << createIndex(index.row(), index.column()+1);
        setData(newShape.last(), QVariant(1));
    }
    m_currentShape = newShape;
}


/////////////////////////////

TetrisDelegate::TetrisDelegate(QObject *parent):
    QStyledItemDelegate (parent)
{}

void TetrisDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    opt.text = "";
    QRect rct(option.rect.topLeft(), option.rect.size());
    if (index.data() == 1) {
        painter->fillRect(rct, Qt::green);
        painter->drawRect(rct);
    } else if (index.data() == 5) {
        painter->fillRect(rct, Qt::gray);
        painter->drawRect(rct);
    } else {
        QRect rct(option.rect.topLeft(), option.rect.size());
        QString text = index.data().toString().trimmed();
        painter->drawText(rct, Qt::AlignCenter, text);
    }

}


