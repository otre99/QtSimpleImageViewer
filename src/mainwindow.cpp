#include "mainwindow.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QKeyEvent>
#include <QMessageBox>
#include <QScrollBar>
#include <QSplitter>
#include <QWheelEvent>
#include <QClipboard>
#include <QMimeData>

#include "cropwidget.h"
#include "imageviewer.h"
#include "ui_mainwindow.h"
#include "image_utils/image_utils.h"


namespace {
const double kScaleFactorStep = 1.1;
}  // namespace

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
  ui->actionSave_as->setEnabled(false);
  ui->actionFlip_H->setEnabled(false);
  ui->actionFlip_V->setEnabled(false);
  ui->actionRotate_L->setEnabled(false);
  ui->actionRotate_R->setEnabled(false);
  ui->actionCrop->setEnabled(false);
  ui->actionAuto_Crop->setEnabled(false);
  ui->actionCopy_To_Clipboard->setEnabled(false);
  ui->actionCopy_path_to_Clipboard->setEnabled(false);

  ui->actionSave->setEnabled(m_imageWasModified = false);

  m_cropWidget = new CropWidget(m_viewer->viewport());
  m_cropWidget->hide();
  m_cropWidget->setImageViewer(m_viewer);
  connect(m_cropWidget, &CropWidget::info, this, &MainWindow::showInfo);
}

MainWindow::~MainWindow() { delete ui; }

bool MainWindow::loadImage(const QString &image_path, const bool reload) {
  if (m_imageWasModified) {
    QMessageBox::StandardButton ex = QMessageBox::warning(
        this, "Unsaed changes", "Do you want to saved changes?",
        QMessageBox::StandardButtons(
            {QMessageBox::Save, QMessageBox::Ignore}));
    switch (ex) {
      case QMessageBox::Save:
        on_actionSave_triggered();
        break;
      case QMessageBox::Ignore:
        updateImageChanged(false);
        break;
      default:
        return false;
    }
  }
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
    if (m_cropWidget->isVisible()) {
      on_actionCrop_triggered();
    }
  } else {
    QMessageBox::critical(this, "Error: " + QFileInfo(image_path).fileName(),
                          reader.errorString());
    return true;
  }
  if (!ui->actionZoomIn->isEnabled()) {
    ui->actionZoomIn->setEnabled(true);
    ui->actionZoomOut->setEnabled(true);
    ui->actionFitWidth->setEnabled(true);
    ui->actionScale100->setEnabled(true);
    ui->actionNext->setEnabled(true);
    ui->actionBack->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionFlip_H->setEnabled(true);
    ui->actionFlip_V->setEnabled(true);
    ui->actionRotate_L->setEnabled(true);
    ui->actionRotate_R->setEnabled(true);
    ui->actionCrop->setEnabled(true);
    ui->actionAuto_Crop->setEnabled(true);
    ui->actionCopy_To_Clipboard->setEnabled(true);
    ui->actionCopy_path_to_Clipboard->setEnabled(true);
  }
  return true;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Return && m_cropWidget->isVisible()) {
    doCrop();
    event->accept();
  } else if (event->key() == Qt::Key_Escape) {
    m_cropWidget->hide();
  } else {
    QMainWindow::keyPressEvent(event);
  }
}

void MainWindow::on_actionOpenImage_triggered() {
  QString imageFile =
      QFileDialog::getOpenFileName(this, "Image", m_lastImgPath);
  if (imageFile.isEmpty()) return;
  loadImage(imageFile, true);
}

void MainWindow::showPixel(int i, int j, double scf) {
  statusBar()->showMessage(
      QString("Pixel [%1,%2] | Zoom [%3\%]").arg(i).arg(j).arg(scf * 100.0));
}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
  if (index.isValid() && m_currentImageIndex != index.row()) {
    m_labelDisplyImgInfo->setText("Name: " +
                                  m_imagesListModel.fileName(index.row()));
    if (loadImage(m_imagesListModel.imagePath(index.row()), false)) {
      m_currentImageIndex = index.row();
    }
  }
}

void MainWindow::updateView() {
  m_imagesListModel.deactivateSoftLoading();
  ui->listView->reset();
}

void MainWindow::on_actionSave_triggered() {
  QString path = m_imagesListModel.imagePath(m_currentImageIndex);
  m_viewer->imagePtr()->save(path);

  QModelIndex index = m_imagesListModel.index(m_currentImageIndex);
  m_imagesListModel.refreshItem(m_currentImageIndex);
  ui->listView->update(index);
  updateImageChanged(false);
}

void MainWindow::showInfo(const QString &msg) { statusBar()->showMessage(msg); }

void MainWindow::on_actionSave_as_triggered() {
  QString imageFile =
      QFileDialog::getSaveFileName(this, "Save image", m_lastImgPath);
  if (imageFile.isEmpty()) return;
  m_viewer->imagePtr()->save(imageFile);
}

void MainWindow::updateImageChanged(bool val) {
  m_imageWasModified = val;
  ui->actionSave->setEnabled(m_imageWasModified);
}

void MainWindow::on_actionNext_triggered() {
  const int tmp = (m_currentImageIndex + 1) % m_imagesListModel.rowCount();
  if (loadImage(m_imagesListModel.imagePath(tmp), false)) {
    QModelIndex index = m_imagesListModel.index(tmp);
    ui->listView->setCurrentIndex(index);
    m_labelDisplyImgInfo->setText("Name: " +
                                  m_imagesListModel.fileName(index.row()));
    m_currentImageIndex = tmp;
  }
}

void MainWindow::on_actionBack_triggered() {
  int tmp = m_currentImageIndex;
  if (m_currentImageIndex == 0) {
    tmp = m_imagesListModel.rowCount();
  }
  tmp = (tmp - 1) % m_imagesListModel.rowCount();
  if (loadImage(m_imagesListModel.imagePath(tmp), false)) {
    QModelIndex index = m_imagesListModel.index(tmp);
    ui->listView->setCurrentIndex(index);
    m_labelDisplyImgInfo->setText("Name: " +
                                  m_imagesListModel.fileName(index.row()));
    m_currentImageIndex = tmp;
  }
}

void MainWindow::on_actionZoomIn_triggered() {
  m_viewer->setScf(std::min(m_viewer->scale() * kScaleFactorStep, 360.0));
}

void MainWindow::on_actionZoomOut_triggered() {
  m_viewer->setScf(std::max(m_viewer->scale() / kScaleFactorStep, 1.0 / 360));
}

void MainWindow::on_actionFitWidth_triggered() { m_viewer->fitWidth(); }

void MainWindow::on_actionScale100_triggered() { m_viewer->setScf(1.0); }

void MainWindow::on_actionCrop_triggered() {
  m_cropWidget->show();
  int d = m_cropWidget->marging();
  QRect r = m_viewer->canvasRect().adjusted(-d, -d, d, d);
  m_cropWidget->setGeometry(r);
}

void MainWindow::on_actionRotate_L_triggered() {
  QImage *img = m_viewer->imagePtr();
  QTransform rot;
  rot.rotate(-90);
  *img = img->transformed(rot);
  m_viewer->adjustAll();
  updateImageChanged(true);
}

void MainWindow::on_actionRotate_R_triggered() {
  QImage *img = m_viewer->imagePtr();
  QTransform rot;
  rot.rotate(90);
  *img = img->transformed(rot);
  m_viewer->adjustAll();
  updateImageChanged(true);
}

void MainWindow::on_actionFlip_H_triggered() {
  QImage *img = m_viewer->imagePtr();
  *img = img->mirrored(true, false);
  m_viewer->adjustAll();
  updateImageChanged(true);
}

void MainWindow::on_actionFlip_V_triggered() {
  QImage *img = m_viewer->imagePtr();
  *img = img->mirrored(false, true);
  m_viewer->adjustAll();
  updateImageChanged(true);
}

void MainWindow::doCrop() {
  QRect finalRect = m_cropWidget->cropRectImage();
  if (finalRect.isEmpty()) {
    QMessageBox::critical(this, "Error", "Invalid crop rectangle");
    return;
  }
  QImage *img = m_viewer->imagePtr();
  *img = img->copy(finalRect);
  m_viewer->adjustAll();
  m_cropWidget->hide();
  updateImageChanged(true);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::aboutQt(this, "ImageViewer");
}

void MainWindow::on_actionAuto_Crop_triggered()
{
    if (m_viewer->imagePtr()){

        QImage *img = m_viewer->imagePtr();
        QRect crop_rect1 = autoCropImage( *img, Qt::white);
        QRect crop_rect2 = autoCropImage( *img);

        const QRect crop_rect = crop_rect1&crop_rect2;

        *img = img->copy(crop_rect);
        m_viewer->adjustAll();
        m_cropWidget->hide();
        updateImageChanged(true);

    }
}

void MainWindow::on_actionCopy_To_Clipboard_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    if (m_viewer->imagePtr()){
        QMimeData *data = new QMimeData;
        const QImage image = *(m_viewer->imagePtr());
        data->setImageData(image);
        clipboard->setMimeData(data, QClipboard::Clipboard);
    }
}


void MainWindow::on_actionCopy_path_to_Clipboard_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    if (m_viewer->imagePtr()){
        QMimeData *data = new QMimeData;
        data->setText( m_imagesListModel.imagePath( m_currentImageIndex ) );
        clipboard->setMimeData(data, QClipboard::Clipboard);
    }
}

