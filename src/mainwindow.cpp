#include "mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QImageReader>
#include <QScrollBar>
#include <QSplitter>
#include <QWheelEvent>

#include "imageviewer.h"
#include "ui_mainwindow.h"

namespace {
const double kScaleFactorStep = 1.1;
}  // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  auto main_spliter = new QSplitter(Qt::Horizontal);
  main_spliter->addWidget(viewer_ = new ImageViewer(this));
  main_spliter->addWidget(ui_->listView);

  int w = main_spliter->width();
  main_spliter->setSizes({6 * w / 7, w / 7});
  setCentralWidget(main_spliter);

  connect(viewer_, &ImageViewer::PixelTrack, this, &MainWindow::ShowPixel);
  ui_->listView->setModel(&images_list_model_);

  connect(ui_->listView->horizontalScrollBar(), &QScrollBar::sliderPressed,
          &images_list_model_, &ImagesListModel::ActivateSoftLoading);
  connect(ui_->listView->horizontalScrollBar(), &QScrollBar::sliderReleased,
          this, &MainWindow::UpdateView);

  lb_disply_img_info_ = new QLabel(this);
  statusBar()->addPermanentWidget(lb_disply_img_info_);
  lb_disply_img_info_->setText("name: None");

  ui_->actionZoom_In->setEnabled(false);
  ui_->actionZoom_Out->setEnabled(false);
  ui_->actionFit_Width->setEnabled(false);
  ui_->actionScale_100->setEnabled(false);
  ui_->actionNext->setEnabled(false);
  ui_->actionBack->setEnabled(false);
}

void MainWindow::UpdateView() {
  images_list_model_.DeactivateSoftLoading();
  ui_->listView->reset();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::on_actionOpen_Image_triggered() {
  QString image_file =
      QFileDialog::getOpenFileName(this, "Image", last_img_path_);
  if (image_file.isEmpty()) return;
  LoadImage(image_file);
}

void MainWindow::ShowPixel(int i, int j, double scf) {
  statusBar()->showMessage(
      QString("Pixel [%1,%2] | Zoom [%3\%]").arg(i).arg(j).arg(scf * 100.0));
}

void MainWindow::LoadImage(const QString &image_path, const bool reload) {
  if (viewer_->ImagePtr()) {
    delete viewer_->ImagePtr();
    viewer_->Init();
  }

  QImageReader reader(image_path);
  if (reader.canRead()) {
    viewer_->AttachImagePtr(new QImage(image_path));
    last_img_path_ = image_path;
    if (reload) {
      images_list_model_.Init(image_path);
      current_image_index_ = 0;
    }
  } else {
    QMessageBox::critical(this, "Error open file:", image_path);
    return;
  }
  if (!ui_->actionZoom_In->isEnabled()) {
    ui_->actionZoom_In->setEnabled(true);
    ui_->actionZoom_Out->setEnabled(true);
    ui_->actionFit_Width->setEnabled(true);
    ui_->actionScale_100->setEnabled(true);
    ui_->actionNext->setEnabled(true);
    ui_->actionBack->setEnabled(true);
  }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index) {}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
  if (index.isValid()) {
    lb_disply_img_info_->setText("Name: " +
                                 images_list_model_.GetFileName(index.row()));
    LoadImage(images_list_model_.GetImagePath(index.row()), false);
    current_image_index_ = index.row();
  }
}

void MainWindow::on_actionNext_triggered() {
  current_image_index_ =
      (current_image_index_ + 1) % images_list_model_.rowCount();
  LoadImage(images_list_model_.GetImagePath(current_image_index_), false);
  QModelIndex index = images_list_model_.index(current_image_index_);
  ui_->listView->setCurrentIndex(index);
  lb_disply_img_info_->setText("Name: " +
                               images_list_model_.GetFileName(index.row()));
}

void MainWindow::on_actionBack_triggered() {
  if (current_image_index_ == 0) {
    current_image_index_ = images_list_model_.rowCount();
  }
  current_image_index_ =
      (current_image_index_ - 1) % images_list_model_.rowCount();
  LoadImage(images_list_model_.GetImagePath(current_image_index_), false);
  QModelIndex index = images_list_model_.index(current_image_index_);
  ui_->listView->setCurrentIndex(index);
  lb_disply_img_info_->setText("Name: " +
                               images_list_model_.GetFileName(index.row()));
}

void MainWindow::on_actionZoom_In_triggered() {
  viewer_->SetScf(std::min(viewer_->GetScale() * kScaleFactorStep, 360.0));
}

void MainWindow::on_actionZoom_Out_triggered() {
  viewer_->SetScf(std::max(viewer_->GetScale() / kScaleFactorStep, 1.0 / 360));
}

void MainWindow::on_actionFit_Width_triggered() { viewer_->FixWidth(); }

void MainWindow::on_actionScale_100_triggered() { viewer_->SetScf(1.0); }
