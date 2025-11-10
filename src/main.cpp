#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setStyle("Fusion");


  MainWindow w;
  const QStringList args = QApplication::arguments();
  if (args.size() > 1) {
    w.loadImage(args[1], true);
  }
  w.showMaximized();
  return QApplication::exec();
}
