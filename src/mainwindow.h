#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageslistmodel.h"
#include <QLabel>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ImageViewer;
class CropWidget;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  bool loadImage(const QString &image_path, bool reload);

protected:
  void keyPressEvent(QKeyEvent *event) override final;

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
  void on_actionCrop_triggered();
  void on_actionSave_triggered();
  void showInfo(const QString &msg);
  void on_actionSave_as_triggered();
  void updateImageChanged(bool val);

  void on_actionRotate_L_triggered();

  void on_actionRotate_R_triggered();

  void on_actionFlip_H_triggered();

  void on_actionFlip_V_triggered();

private:
  void doCrop();
  bool m_imageWasModified;
  Ui::MainWindow *ui;
  ImageViewer *m_viewer;
  ImagesListModel m_imagesListModel;
  QString m_lastImgPath;
  int m_currentImageIndex;
  QLabel *m_labelDisplyImgInfo;
  CropWidget *m_cropWidget;
};

#endif // MAINWINDOW_H
