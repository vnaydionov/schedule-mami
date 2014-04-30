#ifndef SCHEDULEEDITOR_H
#define SCHEDULEEDITOR_H

#include <QDialog>
#include <QMap>
#include "domain/Schedule.h"

namespace Ui {
class ScheduleEditor;
}

class QComboBox;

class ScheduleEditor : public QDialog
{
    Q_OBJECT
    enum Mode {
        NormalMode = 0,
        FunnyMode
    };

public:
    explicit ScheduleEditor(QWidget *parent = 0);
    ~ScheduleEditor();
    
private:
    void save_table_width();
    void closeEvent(QCloseEvent * event);
    Ui::ScheduleEditor *ui;
    QDialog *m_filterDialog;
    QMap<qint32, QString> group_indexes;
    QComboBox *m_filterCb;
    QComboBox *m_filterValueCb;
    QPair<int, int> m_filter;
    Mode m_mode;

private slots:
    void on_LoadScheduleBt_clicked();
    void on_FiltersCb_currentIndexChanged(int index);
    void on_RefreshBt_clicked();
    void on_SaveToPDF_Bt_clicked();
    void on_LoadFromDocxBt_clicked();
    void on_FilterBt_clicked();
    void slotAcceptFilter();
    void on_DictEditorBt_clicked();
    void on_SaveToPdfBt_clicked();
    void on_FilterCh_clicked(bool checked);
    void switchMode();
};

#endif // SCHEDULEEDITOR_H
