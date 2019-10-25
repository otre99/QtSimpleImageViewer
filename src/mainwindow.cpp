#include "mainwindow.h"
#include "imageviewer.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QImageReader>
#include <QScrollBar>
#include <QSplitter>

namespace {
const int kScaleValue100 = 100;
const double kScaleFactorStep = 1.0 / kScaleValue100;
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  auto main_spliter = new QSplitter(Qt::Horizontal);
  main_spliter->addWidget(viewer_ = new ImageViewer(this));
  main_spliter->addWidget(ui_->listView);

  int w = main_spliter->width();
  main_spliter->setSizes({6 * w / 7, w / 7});
  setCentralWidget(main_spliter);

  connect(viewer_, SIGNAL(PixelTrack(int, int)), SLOT(ShowPixel(int, int)));
  ui_->mainToolBar->addWidget(ui_->scfSlider);
  ui_->mainToolBar->addWidget(ui_->goTo100);
  ui_->mainToolBar->addWidget(ui_->tbFull);
  ui_->mainToolBar->setEnabled(false);
  ui_->listView->setModel(&images_list_model_);

  connect(ui_->listView->horizontalScrollBar(), &QScrollBar::sliderPressed,
          &images_list_model_, &ImagesListModel::ActivateSoftLoading);
  connect(ui_->listView->horizontalScrollBar(), &QScrollBar::sliderReleased,
          this, &MainWindow::UpdateView);

    lb_disply_img_info_ = new QLabel(this);
    statusBar()->addPermanentWidget(lb_disply_img_info_);
    lb_disply_img_info_->setText("name: None");
}

void MainWindow::UpdateView() {
  images_list_model_.DeactivateSoftLoading();
  ui_->listView->reset();
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::on_actionOpen_Image_triggered() {
  QString image_file =
      QFileDialog::getOpenFileName(this, "Image", last_img_path_);
  if (image_file.isEmpty())
    return;
  LoadImage(image_file);
}

void MainWindow::ShowPixel(int i, int j) {
  statusBar()->showMessage(QString("[%1,%2]").arg(i).arg(j));
}

void MainWindow::LoadImage(const QString &image_path, const bool reload) {
  if (viewer_->ImagePtr()) {
    delete viewer_->ImagePtr();
    viewer_->Init();
  }

  QImageReader reader(image_path);
  if (reader.canRead()) {
    viewer_->AttachImagePtr(new QImage(image_path));
    ui_->mainToolBar->setEnabled(true);
    last_img_path_ = image_path;
    ui_->scfSlider->setValue(
        static_cast<int>(viewer_->GetScale() / kScaleFactorStep));
    if (reload) {
      images_list_model_.Init(image_path);
      current_image_index_ = 0;
    }
  } else
    ui_->mainToolBar->setEnabled(false);
}

void MainWindow::on_scfSlider_valueChanged(int value) {
  viewer_->SetScf(value * kScaleFactorStep);
}

void MainWindow::on_goTo100_clicked() {
  ui_->scfSlider->setValue(kScaleValue100);
}

void MainWindow::on_tbFull_clicked() {
  if (isFullScreen()) {
    showNormal();
    ui_->tbFull->setText("Full");
  } else {
    showFullScreen();
    ui_->tbFull->setText("Normal");
  }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index) {}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
  if (index.isValid()) {


    lb_disply_img_info_->setText("Name: "+images_list_model_.GetFileName(index.row()));
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
  lb_disply_img_info_->setText("Name: "+images_list_model_.GetFileName(index.row()));
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
  lb_disply_img_info_->setText("Name: "+images_list_model_.GetFileName(index.row()));
}
