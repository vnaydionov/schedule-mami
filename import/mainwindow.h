#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <qzipreader_p.h>
#include <QFile>
#include <QMap>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include "documentparser.h"
#include <QTableView>
#include "tablemodel.h"
#include <zlib.h>
#include <QByteArray>
#include "delegate.h"
#include "widget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <orm/MetaData.h>
#include "domain/User.h"
#include "sharedobj.h"
#include "schedulemanager.h"

#define _R QString::fromUtf8

namespace Ui {



class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    QStringList getUsers();
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;

public slots:
    void setFilePath();
    void parseFile();
    void clearTable();
    void saveTable();
};

#endif // MAINWINDOW_H
