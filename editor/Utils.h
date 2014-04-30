#ifndef UTILS_H
#define UTILS_H

#include <QPushButton>
#include <QDebug>

class QString;
class QListWidget;
class QTableView;

class SchedulePrinter {
    static QString getUnionListOfValues(QListWidget *lst);
    static QString getTextFromCellWidget(const QWidget *cellWidget, int column);
public:
    static void printToPDF(const QTableView *view);
    static void print(const QTableView *view);
};

class CommandButton: public QPushButton {
    Q_OBJECT
private:
    int m_row;
public:
    CommandButton(int _row, QWidget *parent=0):
        QPushButton(parent),
        m_row(_row)
    { }
    void setRow(int _row) { m_row = _row; }
    int getRow() { return m_row; }
public slots:
    void slotIncreaseRowValue(int _row) {
        if (m_row >= _row)
            ++m_row;
    }
    void slotReduceRowValue(int _row) {
        if (m_row > _row)
            --m_row;
    }
};

#endif // UTILS_H
