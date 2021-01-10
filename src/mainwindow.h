#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageslistmodel.h"
#include <QLabel>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ImageViewer;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  void loadImage(const QString &image_path, bool reload);

private slots:
  void on_actionOpenImage_triggered();
  void showPixel(int i, int j, double scf);
  void on_listView_clicked(const QModelIndex &index);
  void on_actionNext_triggered();
  void on_actionBack_triggered();
  void updateView();
  void on_actionZoomIn_triggered();
  void on_actionZoomOut_triggered();
  void on_actionFitWidth_triggered();
  void on_actionScale100_triggered();

private:
  Ui::MainWindow *ui;
  ImageViewer *m_viewer;
  ImagesListModel m_imagesListModel;
  QString m_lastImgPath;
  int m_currentImageIndex;
  QLabel *m_labelDisplyImgInfo;
};

#endif // MAINWINDOW_H
