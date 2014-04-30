#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(_R("Загрузка расписания"));

    SharedObjSingleton::instance();

    QObject::connect(ui->changeFileButton, SIGNAL(clicked()), this, SLOT(setFilePath()));
    QObject::connect(ui->parseButton, SIGNAL(clicked()), this, SLOT(parseFile()));   
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::setFilePath()
{
    QString filePath = QFileDialog::getOpenFileName(0, _R("Файл расписания"), "", "*.docx");
    ui->filePathEdit->setText(filePath);

}

void
MainWindow::parseFile()
{
    QString filePath = ui->filePathEdit->text();
    if (filePath == "")
    {
        QMessageBox::information(0, _R("Ошибка"), _R("Файл не выбран"));
        return;
    }
    QZipReader zip(filePath);

    if (zip.exists())
    {    
        try {
            QByteArray content = zip.fileData("word/document.xml");
            DocumentParser parser(content);
            QVector<Row> rows = parser.getRows();

            QTableView *table = new QTableView();
            TableModel *model = new TableModel(rows);
            table->setModel(model);
            table->setItemDelegateForRow(0, new TableHeaderDelegate);
            table->resizeColumnsToContents();
            table->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
            table->horizontalHeader()->hide();
            table->verticalHeader()->hide();
            QVBoxLayout *layout = (QVBoxLayout *) ui->tableLayout;
            clearTable();
            layout->addWidget(table);

            QPushButton *clearButton = new QPushButton();
            clearButton->setObjectName("clearButton");
            clearButton->setText(_R("Очистить"));
            QObject::connect(clearButton, SIGNAL(clicked()), this, SLOT(clearTable()));
            QPushButton *saveButton = new QPushButton();
            saveButton->setObjectName("saveButton");
            saveButton->setText(_R("Сохранить"));
            QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveTable()));

            QStringList users = getUsers();
            QComboBox *cb = new QComboBox();
            cb->addItems(users);
            cb->setObjectName("userComboBox");
            layout = (QVBoxLayout *) ui->infoLayout;
            layout->addWidget(cb);

            layout = (QVBoxLayout *) ui->buttonLayout;
            layout->addWidget(saveButton);
            layout->addWidget(clearButton);

        }
        catch(ParserEx)
        {
            QMessageBox::information(0, _R("Ошибка"), _R("Документ поврежден"));
        }
    }
    zip.close();
}

void
MainWindow::saveTable()
{
    QVBoxLayout *layout = (QVBoxLayout *) ui->infoLayout;
    QComboBox *cb = (QComboBox *) layout->takeAt(0)->widget();

    Yb::Session session(Yb::theSchema::instance(), SharedObjSingleton::instance().getEngine());
    qDebug() << "Name: " << cb->currentText();
    Domain::User user = Yb::query<Domain::User>(session)
            .filter_by(Domain::User::c.name == cb->currentText()).one();

    {
        QStringList categories;
        QVBoxLayout *layout = (QVBoxLayout *) ui->tableLayout;
        QTableView *table = (QTableView *) layout->itemAt(0)->widget();
        QAbstractItemModel *model = table->model();
        for (int i = 0; i < model->columnCount(); ++i)
        {
            QModelIndex index = model->index(0, i, QModelIndex());
            categories << index.data().toString();
        }
        SharedObjSingleton::instance().setCategories(categories);
    }

    ScheduleManager scheduleManager(&user);
    scheduleManager.convertToScheduleObj();
    QString msg = _R("Расписание сохранено для пользователя: ");
    msg += user.name.value();
    QMessageBox::information(0, _R("Радость"), msg);
}

void
MainWindow::clearTable()
{
    QVBoxLayout *layout = (QVBoxLayout *) ui->tableLayout;
    QLayoutItem *item = layout->takeAt(0);
    if (item)
    {
        QWidget *widget = item->widget();
        delete item;
        layout->removeWidget(widget);
        delete widget;
    }

    layout = (QVBoxLayout *) ui->buttonLayout;
    QLayoutItem *clearButton = layout->takeAt(1);
    if (clearButton)
    {
        QWidget *widget = clearButton->widget();
        delete clearButton;
        layout->removeWidget(widget);
        delete widget;
    }

    QLayoutItem *saveButton = layout->takeAt(1);
    if (saveButton)
    {
        QWidget *widget = saveButton->widget();
        delete saveButton;
        layout->removeWidget(widget);
        delete widget;
    }

    /*
    layout = (QVBoxLayout *) ui->infoLayout;
    QLayoutItem *users = layout->itemAt(0);
    if (users)
    {
        QWidget *widget = users->widget();
        delete users;
        layout->removeWidget(widget);
        delete widget;
    }
    */
}

QStringList
MainWindow::getUsers()
{
    QStringList userList;

    Yb::Session session(Yb::theSchema::instance(), SharedObjSingleton::instance().getEngine());
    Yb::DomainResultSet<Domain::User> users = Yb::query<Domain::User>(session).all();
    Yb::DomainResultSet<Domain::User>::iterator ibegin = users.begin(), iend = users.end();
    for (; ibegin != iend; ++ibegin)
    {
        userList << ibegin->name.value();
    }
    return userList;
}

