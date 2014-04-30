#include <QtGui/QApplication>
#include <QtGui>
#include "App.h"
#include "mainwindow.h"
#include <QSplashScreen>
#include <QThread>
#include "ScheduleEditor.h"

using namespace std;

QPointer<QApplication> app;

class Delay: public QThread {
public:
    static void sleep(unsigned long sec) {
        QThread::sleep(sec);
    }
};

int
main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(ImageResourses);
    app = new QApplication(argc, argv);
    theApp::instance();
    //QSplashScreen splash(QPixmap("images/splash_screen.jpeg"));

   // splash.show();
    //splash.showMessage(" ", Qt::AlignLeft | Qt::AlignTop, Qt::black);
    //qApp->processEvents();
    //ScheduleEditor *d = new ScheduleEditor();
    //d->show();
    app->setAttribute(Qt::AA_DontShowIconsInMenus, false);

    MainWindow w;

    //Delay::sleep(1);
    //w.setEnabled(false);
    w.show();
    //splash.finish(&w);
    //w.call_auth_dialog();
    return app->exec();
}
