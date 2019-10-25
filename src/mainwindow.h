#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "imageslistmodel.h"
#include <QMainWindow>
#include <QSlider>
#include <QLabel>

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
  void ShowPixel(int i, int j);
  void on_scfSlider_valueChanged(int value);
  void on_goTo100_clicked();
  void on_tbFull_clicked();
  void on_listView_doubleClicked(const QModelIndex &index);
  void on_listView_clicked(const QModelIndex &index);
  void on_actionNext_triggered();
  void on_actionBack_triggered();
  void UpdateView();

private:
  Ui::MainWindow *ui_;
  ImageViewer *viewer_;
  ImagesListModel images_list_model_;
  QString last_img_path_;
  int current_image_index_;
  QLabel *lb_disply_img_info_;
};

#endif // MAINWINDOW_H
