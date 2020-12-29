#include <QApplication>
#include <QDebug>
#include <QTimer>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.showMaximized();
  const QStringList args = QApplication::arguments();
  if (args.size() > 1) {
    //    QTimer::singleShot(1, &w, [&w, &args]() { w.LoadImage(args[1]); });
    w.LoadImage(args[1]);
  }
  return QApplication::exec();
}
