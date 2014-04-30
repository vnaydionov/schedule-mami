#ifndef TETRISMODEL_H
#define TETRISMODEL_H

#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QVector>
#include <QModelIndex>

enum Value {
    None = 0,
    Red,
    Green,
    Yellow,
    Blue
};

enum Shape {
    TShape = 0,
    LineShape,
    LineWithTallShape,
    CubeShape,
    NullShape
};

enum PlaceState {
    YesFreePlace = 1,
    NotFreePlace
};

enum ShapeState {
    LeftState = 0,
    UpState,
    RightState,
    DownState,
    RandomState
};

class QTimer;

class TetrisModel : public QAbstractItemModel
{
    Q_OBJECT
    friend class TetrisDelegate;
public:
    explicit TetrisModel(int columnsCount, int rowsCount, QObject *parent=0);
    ~TetrisModel();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

    bool eventFilter(QObject *object, QEvent *event);

    void startNewGame();

private slots:
    void shapeMovingDown();
    void shapeMovingLeft();
    void shapeMovingRight();

private:
    int m_score;
    int m_rowsCount;
    int m_columnsCount;
    QVector< QVector<int> > m_matrix;
    QVector<QModelIndex> m_currentShape;
    int m_typeCurrentShape;
    int m_shapeState;

    QVector<QModelIndex> m_nextShape;
    int m_typeNextShape;
    int m_nextShapeState;

    QTimer *m_timer;


    bool isValidShape(QVector<QModelIndex> _shape) const;
    bool isFreePlace(const QList<QModelIndex> &place) const;
    bool isFreePlaceForNextDownStep() const;
    bool isFreePlaceForNextRightStep() const;
    bool isFreePlaceForNextLeftStep() const;
    void createShape(QModelIndex _center, int _state = RandomState);
    QVector<QModelIndex> createTShape(QModelIndex _center, int _state, bool _ignoreValidCheck=false) const;
    QVector<QModelIndex> createLineShape(QModelIndex _center, int _state, bool _ignoreValidCheck=false) const;
    QVector<QModelIndex> createLineWithTallShape(QModelIndex _center, int _state, bool _ignoreValidCheck=false) const;
    QVector<QModelIndex> createCubeShape(QModelIndex _center) const;
    void rotateShape();
    void rotateTShape();
    void rotateLineShape();
    void rotateLineWithTallShape();
    QVector<int> checkFullRows();
    void Yeaaaah(const QVector<int> &_fullRows);
    void clear();
};

class TetrisDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    TetrisDelegate(QObject *parent = 0);
    void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // TETRISMODEL_H
