#include "mainwindow.h"
#include <QApplication>
#include <QImage>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    const QStringList args = QApplication::arguments();
    if (args.size()>1){
        w.loadImage( args[1] );
    }
    w.showMaximized();
    

    return QApplication::exec();
}
