#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QImage>
#include <QScreen>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.showMaximized();
  //w.setGeometry(QApplication::screens()[0]->geometry());
  const QStringList args = QApplication::arguments();
  if (args.size() > 1) {
    w.LoadImage(args[1]);
  }
  return QApplication::exec();
}
