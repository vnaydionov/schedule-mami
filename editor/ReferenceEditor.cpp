#include "ReferenceEditor.h"
#include "ui_ReferenceEditor.h"
#include "domain/StudGroup.h"
#include "domain/Subject.h"
#include "domain/Activity.h"
#include "domain/Place.h"
#include "domain/User.h"
#include "App.h"
#include <QDebug>
#include <QMessageBox>
#include "DictionaryModel.h"
#include "XmlConfigModel.h"

using namespace Domain;

ReferenceEditor::ReferenceEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReferenceEditor)
{
    ui->setupUi(this);
    ui->ReferenceCb->setItemData(0, QVariant("T_SUBJECT"), Qt::UserRole);
    ui->ReferenceCb->setItemData(1, QVariant("T_ACTIVITY"), Qt::UserRole);
    ui->ReferenceCb->setItemData(2, QVariant("T_PLACE"), Qt::UserRole);
    ui->ReferenceCb->setItemData(3, QVariant("T_USER"), Qt::UserRole);
    ui->ReferenceCb->setItemData(4, QVariant("T_STUD_GROUP"), Qt::UserRole);
    ui->ReferenceCb->setCurrentIndex(0);
    fillTView();
}

ReferenceEditor::~ReferenceEditor()
{
    delete ui;
}

void ReferenceEditor::on_CloseBt_clicked()
{
    this->close();
}

void ReferenceEditor::fillTView()
{
    QAbstractItemModel *cur_model = ui->dictionaryTView->model();
    if (cur_model)
        delete cur_model;
    if (ui->xmlConfigoxCb->isChecked())
    {
        XmlConfigModel *model = new XmlConfigModel( ui->ReferenceCb->itemData(ui->ReferenceCb->currentIndex(), Qt::UserRole).toString(), ui->dictionaryTView );
        ui->dictionaryTView->setModel(model);
        ui->dictionaryTView->setItemDelegate(new XmlConfigDelegate(ui->dictionaryTView));
        ui->pushButton->setEnabled(false);
    }
    else
    {
        DictionaryModel *model = new DictionaryModel( ui->ReferenceCb->itemData(ui->ReferenceCb->currentIndex(), Qt::UserRole).toString(), ui->dictionaryTView );
        ui->dictionaryTView->setModel(model);
        ui->dictionaryTView->setItemDelegate(new DictionaryDelegate(ui->dictionaryTView));
        ui->pushButton->setEnabled(true);
    }
}

void ReferenceEditor::on_ReferenceCb_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    fillTView();
}

void ReferenceEditor::on_pushButton_clicked()
{
    if (!ui->xmlConfigoxCb->isChecked())
        static_cast<DictionaryModel *>(ui->dictionaryTView->model())->epicCommit();
}

void ReferenceEditor::on_xmlConfigoxCb_clicked()
{
    if (ui->xmlConfigoxCb->isChecked())
        ui->pushButton->setEnabled(false);
    else
        ui->pushButton->setEnabled(true);
    fillTView();
}
