#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {

//NOTE: I observed a drastic performance degradation when using `wayland` in kubuntu 26.04, so I force the use of `xcb` platform plugin on Linux if not already set. 
//      This is a workaround and may not be ideal for all environments. The  QT_QPA_PLATFORM still can be changed by the user by setting it before running the application 
//      if they want to use a different platform plugin, e.g.: `QT_QPA_PLATFORM=wayland ./TMEDesktopViewer`.
#ifdef Q_OS_LINUX
  if (qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")) {
    qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
  }
#endif

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
