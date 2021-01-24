#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setStyle("Fusion");

  // I like dark themes
  QPalette newPalette;
  newPalette.setColor(QPalette::Window, QColor(37, 37, 37));
  newPalette.setColor(QPalette::WindowText, QColor(212, 212, 212));
  newPalette.setColor(QPalette::Base, QColor(60, 60, 60));
  newPalette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
  newPalette.setColor(QPalette::PlaceholderText, QColor(127, 127, 127));
  newPalette.setColor(QPalette::Text, QColor(212, 212, 212));
  newPalette.setColor(QPalette::Button, QColor(45, 45, 45));
  newPalette.setColor(QPalette::ButtonText, QColor(212, 212, 212));
  newPalette.setColor(QPalette::BrightText, QColor(240, 240, 240));
  newPalette.setColor(QPalette::Highlight, QColor(38, 79, 120));
  newPalette.setColor(QPalette::HighlightedText, QColor(240, 240, 240));
  newPalette.setColor(QPalette::Light, QColor(60, 60, 60));
  newPalette.setColor(QPalette::Midlight, QColor(52, 52, 52));
  newPalette.setColor(QPalette::Dark, QColor(30, 30, 30));
  newPalette.setColor(QPalette::Mid, QColor(37, 37, 37));
  newPalette.setColor(QPalette::Shadow, QColor(0, 0, 0));
  newPalette.setColor(QPalette::Disabled, QPalette::Text,
                      QColor(127, 127, 127));
  a.setPalette(newPalette);

  MainWindow w;
  const QStringList args = QApplication::arguments();
  if (args.size() > 1) {
    w.loadImage(args[1], true);
  }
  w.showMaximized();

//  QImage img(101,50,QImage::Format_ARGB32);
//  int dex=0;
//  for (int i=0; i<50; ++i){
//      for (int j=0; j<101; ++j){
//          img.setPixel(j,i,dex%2 ? qRgb(255,0,0) : qRgb(0,0,1));
//          dex+=1;
//      }
//  }
//  img.save("OK.png");
  return QApplication::exec();


}
