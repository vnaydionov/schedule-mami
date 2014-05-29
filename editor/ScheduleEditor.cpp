#include "ScheduleEditor.h"
#include "ui_ScheduleEditor.h"
#include "ReferenceEditor.h"
#include <QtGui>
#include <QDebug>
#include "SettingsEditor.h"
#include "domain/StudGroup.h"
#include "domain/User.h"
#include "App.h"
#include "domain/ScheduleGroup.h"
#include "DocxParser.h"
#include "Utils.h"
#include "ScheduleModel.h"
#include <tetrismodel.h>

using namespace Yb;
using namespace Domain;

extern std::auto_ptr<QApplication> app;

ScheduleEditor::ScheduleEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ScheduleEditor),
    m_mode(NormalMode)
{
    ui->setupUi(this);
    ui->ScheduleView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ScheduleModel *model = new ScheduleModel(ui->ScheduleView);
    ui->ScheduleView->setModel(model);
    ScheduleDelegate *delegate = new ScheduleDelegate(ui->ScheduleView);
    ui->ScheduleView->setItemDelegate(delegate);
    m_filter = QPair<int,int>(-1, -1);
    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence("Ctrl+G"));
    shortcut->setContext(Qt::ApplicationShortcut);
    connect(shortcut, SIGNAL(activated()), this, SLOT(switchMode()));

}

ScheduleEditor::~ScheduleEditor()
{
    delete ui;
}

void
ScheduleEditor::save_table_width()
{
    if (ui->ScheduleView->model()->columnCount())
    {
        Yb::ElementTree::ElementPtr tbl_config = theApp::instance().get_table_config()->find_first("T_SCHEDULE");
        tbl_config->find_first("DOW")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(1));
        tbl_config->find_first("TIME")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(2));
        tbl_config->find_first("SUBJECT")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(3));
        tbl_config->find_first("GROUPS")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(4));
        tbl_config->find_first("DATE")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(5));
        tbl_config->find_first("PLACE")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(6));
        tbl_config->find_first("TEACHER")->attrib_["width"] = QString::number(ui->ScheduleView->columnWidth(7));
        theApp::instance().save_table_widget_config();
    }
}

void
ScheduleEditor::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    if (m_mode == NormalMode)
        save_table_width();
}

void ScheduleEditor::switchMode()
{
    QAbstractItemModel *model = ui->ScheduleView->model();
    QAbstractItemDelegate *delegate = ui->ScheduleView->itemDelegate();
    if (m_mode == NormalMode) {
        TetrisModel *tModel = new TetrisModel(8, 20, ui->ScheduleView);
        ui->ScheduleView->setModel(tModel);
        tModel->startNewGame();
        m_mode = FunnyMode;
    } else {
        ScheduleModel *model = new ScheduleModel(ui->ScheduleView);
        ui->ScheduleView->setModel(model);
        ScheduleDelegate *delegate = new ScheduleDelegate(ui->ScheduleView);
        ui->ScheduleView->setItemDelegate(delegate);
        m_filter = QPair<int,int>(-1, -1);
        m_mode = NormalMode;
        on_LoadScheduleBt_clicked();
    }
    delete model;
    delete delegate;

}

void ScheduleEditor::on_LoadScheduleBt_clicked()
{
    if (m_mode != NormalMode)
        switchMode();
    Yb::Session *session = new Yb::Session(Yb::theSchema(), theApp::instance().get_engine());
    QVector<ScheduleHolder> objects;
    std::auto_ptr<Yb::Session> s = theApp::instance().new_session();
    if (ui->FilterCh->isChecked() && m_filter.first == 0) { // Groups
        Yb::DomainResultSet<ScheduleGroup> rs = Yb::query<ScheduleGroup>(*session)
                .filter_by(ScheduleGroup::c.group_id == m_filter.second)
                .all();
        Yb::DomainResultSet<ScheduleGroup>::iterator it = rs.begin();
        for (; it != rs.end(); ++it)
            objects.push_back(it->schedule);
    } else if (ui->FilterCh->isChecked() && m_filter.first == 1) { // Teachers
        Yb::DomainResultSet<Schedule> rs = Yb::query<Schedule>(*session)
                .filter_by(Schedule::c.receiver_id ==  m_filter.second)
                .all();
        Yb::DomainResultSet<Domain::Schedule>::iterator it = rs.begin();
        for (; it != rs.end(); ++it)
            objects.push_back(Domain::ScheduleHolder(*session, it->id));
    } else {
        Yb::DomainResultSet<Schedule> rs = Yb::query<Schedule>(*session).all();
        Yb::DomainResultSet<Domain::Schedule>::iterator it = rs.begin();
        for (; it != rs.end(); ++it)
            objects.push_back(Domain::ScheduleHolder(*session, it->id));
    }
    static_cast<ScheduleModel *>(ui->ScheduleView->model())->fill(objects);
    ui->FilterCh->setEnabled(true);
}

void ScheduleEditor::on_FiltersCb_currentIndexChanged(int index)
{
    std::auto_ptr<Session> session = theApp::instance().new_session();
    m_filterValueCb->clear();
    if (index == 0) { // Groups
        DomainResultSet<StudGroup> rs = Yb::query<StudGroup>(*session).all();
        DomainResultSet<StudGroup>::iterator it = rs.begin(),
                                             it_end = rs.end();
        if (it == it_end) {
            QMessageBox::information(this, "Information",
                    "Table with student groups is empty.. Please, "
                    "fill this table in reference editor and refresh this dialog.",
                    QMessageBox::Ok);
        }
        for (; it != it_end; ++it)
            m_filterValueCb->addItem(it->name_code, (QVariant)it->id);
    } else if (index == 1) {
        DomainResultSet<User> rs = Yb::query<User>(*session).all();
        DomainResultSet<User>::iterator it = rs.begin(),
                                             it_end = rs.end();
        if (it == it_end) {
            QMessageBox::information(this, "Information",
                    "Table with teachers is empty.. Please, "
                    "fill this table in reference editor and refresh this dialog.",
                    QMessageBox::Ok);
        }
        for (int i=0; it != it_end; ++it, ++i)
            m_filterValueCb->addItem(it->name, (QVariant)it->id);
    }
}

void ScheduleEditor::on_RefreshBt_clicked()
{
    static_cast<ScheduleModel *>(ui->ScheduleView->model())->epicCommit();
    QMessageBox::information(0, trUtf8("Информация"),
            trUtf8("Данные успешно сохранены."),
            QMessageBox::Ok);
}

void ScheduleEditor::on_SaveToPDF_Bt_clicked()
{
    SchedulePrinter::printToPDF(ui->ScheduleView);
}

void ScheduleEditor::on_LoadFromDocxBt_clicked()
{
    static_cast<ScheduleModel *>(ui->ScheduleView->model())->fill(DocxParser::pasreDocument("schedule.docx"));
}

void ScheduleEditor::on_FilterBt_clicked()
{
    m_filterDialog = new QDialog();
    m_filterDialog->setToolTip(trUtf8("Добавление фильтра"));
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *headerLayout = new QHBoxLayout();
    QLabel *header = new QLabel(trUtf8("Выберите столбец и значение фильтра:"));
    headerLayout->addWidget(header);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *filterLb = new QLabel(trUtf8("Столбец:"));
    m_filterCb = new QComboBox();
    QLabel *filterValueLb = new QLabel(trUtf8("Значение:"));
    m_filterValueCb = new QComboBox();

    connect(m_filterCb, SIGNAL(currentIndexChanged(int)), SLOT(on_FiltersCb_currentIndexChanged(int)));
    m_filterCb->addItems(QStringList() << trUtf8("Группа") << trUtf8("Преподаватель"));
    m_filterCb->setCurrentIndex(0);

    filterLayout->addWidget(filterLb);
    filterLayout->addWidget(m_filterCb);
    filterLayout->addWidget(filterValueLb);
    filterLayout->addWidget(m_filterValueCb);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btOk = new QPushButton(trUtf8("Применить"));
    connect(btOk, SIGNAL(clicked()), SLOT(slotAcceptFilter()));

    QPushButton *btCancel = new QPushButton(trUtf8("Отмена"));
    connect(btCancel, SIGNAL(clicked()), m_filterDialog, SLOT(close()));

    buttonsLayout->addWidget(btOk);
    buttonsLayout->addWidget(btCancel);

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addLayout(buttonsLayout);

    m_filterDialog->setLayout(mainLayout);
    m_filterDialog->show();
}

void ScheduleEditor::slotAcceptFilter()
{
    m_filter = QPair<int, int>(m_filterCb->currentIndex(), m_filterValueCb->itemData(m_filterValueCb->currentIndex()).toInt());
    on_LoadScheduleBt_clicked();
    QString text = trUtf8("Текущий фильтр: ") + m_filterCb->currentText() + " -> " + m_filterValueCb->currentText();
    ui->CurrentFilterLb->setText(text);
    m_filterDialog->close();
}

void ScheduleEditor::on_DictEditorBt_clicked()
{
    ReferenceEditor *re = new ReferenceEditor(this);
    re->show();
}

void ScheduleEditor::on_SaveToPdfBt_clicked()
{
    SchedulePrinter::printToPDF(static_cast<QTableView *>(parent()));
}

void ScheduleEditor::on_FilterCh_clicked(bool checked)
{
    if (checked) {
        ui->FilterBt->setEnabled(true);
        ui->CurrentFilterLb->setEnabled(true);
    } else {
        ui->FilterBt->setEnabled(false);
        ui->CurrentFilterLb->setEnabled(false);
    }
    on_LoadScheduleBt_clicked();
}
