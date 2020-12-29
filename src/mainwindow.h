#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QSlider>

#include "imageslistmodel.h"

namespace Ui {
class MainWindow;
}

class ImageViewer;

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;
  void LoadImage(const QString &image_path, bool reload = true);

 private slots:
  void on_actionOpen_Image_triggered();
  void ShowPixel(int i, int j, double scf);
  void on_listView_doubleClicked(const QModelIndex &index);
  void on_listView_clicked(const QModelIndex &index);
  void on_actionNext_triggered();
  void on_actionBack_triggered();
  void UpdateView();
  void on_actionZoom_In_triggered();
  void on_actionZoom_Out_triggered();
  void on_actionFit_Width_triggered();
  void on_actionScale_100_triggered();

 private:
  Ui::MainWindow *ui_;
  ImageViewer *viewer_;
  ImagesListModel images_list_model_;
  QString last_img_path_;
  int current_image_index_;
  QLabel *lb_disply_img_info_;
};

#endif  // MAINWINDOW_H
