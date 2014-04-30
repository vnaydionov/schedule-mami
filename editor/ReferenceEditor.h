#ifndef ReferenceEditor_H
#define ReferenceEditor_H

#include <QDialog>

namespace Ui {
class ReferenceEditor;
}

class ReferenceEditor : public QDialog
{
    Q_OBJECT
    
public:
    explicit ReferenceEditor(QWidget *parent = 0);
    ~ReferenceEditor();
    
private:
    Ui::ReferenceEditor *ui;
    void fillTView();
private slots:
    void on_CloseBt_clicked();
    void on_ReferenceCb_currentIndexChanged(int index);
    void on_pushButton_clicked();
    void on_xmlConfigoxCb_clicked();
};

#endif // ReferenceEditor_H
