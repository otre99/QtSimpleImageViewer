#include "mainwindow.h"
#include "imageviewer.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QMessageBox>
#include <QScrollBar>
#include <QSplitter>
#include <QWheelEvent>

namespace {
const double kScaleFactorStep = 1.1;
} // namespace

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  auto main_spliter = new QSplitter(Qt::Horizontal);
  main_spliter->addWidget(m_viewer = new ImageViewer(this));
  main_spliter->addWidget(ui->listView);

  int w = main_spliter->width();
  main_spliter->setSizes({6 * w / 7, w / 7});
  setCentralWidget(main_spliter);

  connect(m_viewer, &ImageViewer::pixelTrack, this, &MainWindow::showPixel);
  ui->listView->setModel(&m_imagesListModel);

  connect(ui->listView->horizontalScrollBar(), &QScrollBar::sliderPressed,
          &m_imagesListModel, &ImagesListModel::activateSoftLoading);
  connect(ui->listView->horizontalScrollBar(), &QScrollBar::sliderReleased,
          this, &MainWindow::updateView);

  m_labelDisplyImgInfo = new QLabel(this);
  statusBar()->addPermanentWidget(m_labelDisplyImgInfo);
  m_labelDisplyImgInfo->setText("name: None");

  ui->actionZoomIn->setEnabled(false);
  ui->actionZoomOut->setEnabled(false);
  ui->actionFitWidth->setEnabled(false);
  ui->actionScale100->setEnabled(false);
  ui->actionNext->setEnabled(false);
  ui->actionBack->setEnabled(false);
}

void MainWindow::updateView() {
  m_imagesListModel.deactivateSoftLoading();
  ui->listView->reset();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionOpenImage_triggered() {
  QString imageFile =
      QFileDialog::getOpenFileName(this, "Image", m_lastImgPath);
  if (imageFile.isEmpty())
    return;
  loadImage(imageFile, true);
}

void MainWindow::showPixel(int i, int j, double scf) {
  statusBar()->showMessage(
      QString("Pixel [%1,%2] | Zoom [%3\%]").arg(i).arg(j).arg(scf * 100.0));
}

void MainWindow::loadImage(const QString &image_path, const bool reload) {
  if (m_viewer->imagePtr()) {
    delete m_viewer->imagePtr();
    m_viewer->init();
  }

  QImageReader reader(image_path);
  if (reader.canRead()) {
    m_viewer->attachImagePtr(new QImage(image_path));
    m_lastImgPath = image_path;
    if (reload) {
      m_imagesListModel.init(image_path);
      m_currentImageIndex =
          m_imagesListModel.indexOf(QFileInfo(image_path).fileName());
      on_actionScale100_triggered();
    }
  } else {
    QMessageBox::critical(this, "Error open file:", image_path);
    return;
  }
  if (!ui->actionZoomIn->isEnabled()) {
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionFitWidth->setEnabled(true);
    ui->actionScale100->setEnabled(true);
    ui->actionNext->setEnabled(true);
    ui->actionBack->setEnabled(true);
  }
}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
  if (index.isValid() && m_currentImageIndex != index.row()) {
    m_labelDisplyImgInfo->setText("Name: " +
                                  m_imagesListModel.fileName(index.row()));
    loadImage(m_imagesListModel.imagePath(index.row()), false);
    m_currentImageIndex = index.row();
  }
}

void MainWindow::on_actionNext_triggered() {
  m_currentImageIndex =
      (m_currentImageIndex + 1) % m_imagesListModel.rowCount();
  loadImage(m_imagesListModel.imagePath(m_currentImageIndex), false);
  QModelIndex index = m_imagesListModel.index(m_currentImageIndex);
  ui->listView->setCurrentIndex(index);
  m_labelDisplyImgInfo->setText("Name: " +
                                m_imagesListModel.fileName(index.row()));
}

void MainWindow::on_actionBack_triggered() {
  if (m_currentImageIndex == 0) {
    m_currentImageIndex = m_imagesListModel.rowCount();
  }
  m_currentImageIndex =
      (m_currentImageIndex - 1) % m_imagesListModel.rowCount();
  loadImage(m_imagesListModel.imagePath(m_currentImageIndex), false);
  QModelIndex index = m_imagesListModel.index(m_currentImageIndex);
  ui->listView->setCurrentIndex(index);
  m_labelDisplyImgInfo->setText("Name: " +
                                m_imagesListModel.fileName(index.row()));
}

void MainWindow::on_actionZoomIn_triggered() {
  m_viewer->setScf(std::min(m_viewer->scale() * kScaleFactorStep, 360.0));
}

void MainWindow::on_actionZoomOut_triggered() {
  m_viewer->setScf(std::max(m_viewer->scale() / kScaleFactorStep, 1.0 / 360));
}

void MainWindow::on_actionFitWidth_triggered() { m_viewer->fixWidth(); }

void MainWindow::on_actionScale100_triggered() { m_viewer->setScf(1.0); }
