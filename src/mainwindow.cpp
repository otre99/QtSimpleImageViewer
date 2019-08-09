#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "imageviewer.h"
#include <QDebug>
#include <QImageReader>

static const double dx = 1.0/16.0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(viewer = new ImageViewer(this));
    connect(viewer, SIGNAL(pixelTrack(int,int)), SLOT(showPixel(int,int)));
    ui->mainToolBar->addWidget(ui->scfSlider);
    ui->mainToolBar->addWidget(ui->goTo100);
    ui->mainToolBar->addWidget(ui->tbFull);
    ui->mainToolBar->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString imageFile = QFileDialog::getOpenFileName(this, "Image",   m_lastImg);
    if (imageFile.isEmpty()) return;
    loadImage(imageFile);
}

void MainWindow::showPixel(int i, int j)
{
    statusBar()->showMessage(QString("[%1,%2]").arg(i).arg(j));
}

void MainWindow::loadImage(const QString &image_path)
{
    if ( viewer->imagePtr() ){
        delete viewer->imagePtr();
        viewer->init();
    }

    QImageReader reader(image_path);
    if ( reader.canRead() ){
        viewer->attachImagePtr( new QImage(image_path) );
        ui->mainToolBar->setEnabled(true);
        m_lastImg = image_path;
    } else
        ui->mainToolBar->setEnabled(false);
}

void MainWindow::on_scfSlider_valueChanged(int value)
{
    viewer->setScf(value*dx);
}

void MainWindow::on_goTo100_clicked()
{
    ui->scfSlider->setValue(16);
}

void MainWindow::on_tbFull_clicked()
{
    if ( isFullScreen() ){
        showNormal();
        ui->tbFull->setText("Full");
    } else {
        showFullScreen();
        ui->tbFull->setText("Normal");
    }
}
