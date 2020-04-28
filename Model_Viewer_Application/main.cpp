#include <QApplication>
#include "mainwindow.h"
#include <QSplashScreen>
#include <QProgressBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap("splashscreen.png");
    QSplashScreen splash(pixmap);

    splash.show();
    a.processEvents();
    MainWindow w;
    splash.finish(&w);
    w.show();
    return a.exec();
}
