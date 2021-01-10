#include "imageviewer.h"
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>

ImageViewer::ImageViewer(QWidget *parent) : QAbstractScrollArea(parent) {
  init();
  connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this,
          &ImageViewer::setXmov);
  connect(verticalScrollBar(), &QScrollBar::sliderMoved, this,
          &ImageViewer::setYmov);

  viewport()->setMouseTracking(true);
  viewport()->setCursor(QCursor(Qt::CrossCursor));
}

void ImageViewer::attachImagePtr(QImage *ptr) {
  m_imagePtr = ptr;
  selectScf();
  adjustAll();
  setEnabled(m_imagePtr != nullptr);
}

void ImageViewer::init() {
  m_imagePtr = nullptr;
  m_scaleFactor = 1.0;
  m_xMov = m_yMov = 0;
  m_cW_ = m_cH_ = 0;
  setEnabled(false);
}

void ImageViewer::selectScf() {
  const double s1 = double(viewport()->width()) / m_imagePtr->width();
  const double s2 = double(viewport()->height()) / m_imagePtr->height();
  m_scaleFactor = std::min(std::min(s1, s2), 1.0);
  if (m_scaleFactor == 0.0)
    m_scaleFactor = 1.0;
}

void ImageViewer::resizeEvent(QResizeEvent *ev) {
  QAbstractScrollArea::resizeEvent(ev);
  if (m_imagePtr) {
    adjustAll();
  }
}

void ImageViewer::paintEvent(QPaintEvent *) {
  if (!m_imagePtr)
    return;
  QPainter p(viewport());

  p.translate(QPoint(viewport()->width() / 2, viewport()->height() / 2));
  p.drawImage(QRect(-m_screenW / 2, -m_screenH / 2, m_screenW, m_screenH),
              *m_imagePtr, QRect(m_xMov, m_yMov, m_cW_, m_cH_));
}

void ImageViewer::mouseMoveEvent(QMouseEvent *e) {
  const QPoint pd_pos = viewport()->mapFrom(this, e->pos());
  const int xxf =
      (pd_pos.x() - viewport()->width() / 2 + m_screenW / 2) / m_scaleFactor;
  const int yyf =
      (pd_pos.y() - viewport()->height() / 2 + m_screenH / 2) / m_scaleFactor;
  const int xx = m_xMov + xxf;
  const int yy = m_yMov + yyf;
  emit pixelTrack(xx, yy, m_scaleFactor);

  if (e->buttons() & Qt::LeftButton) {
    m_xMov =
        std::max(std::min(m_lastPt.x() - xxf, m_imagePtr->width() - m_cW_), 0);
    m_yMov =
        std::max(std::min(m_lastPt.y() - yyf, m_imagePtr->height() - m_cH_), 0);
    horizontalScrollBar()->setValue(m_xMov);
    verticalScrollBar()->setValue(m_yMov);
  }
}

void ImageViewer::mousePressEvent(QMouseEvent *e) {
  QPoint pd_pos = viewport()->mapFrom(this, e->pos());
  m_lastPt.setX(m_xMov +
                (pd_pos.x() - viewport()->width() / 2 + m_screenW / 2) /
                    m_scaleFactor);
  m_lastPt.setY(m_yMov +
                (pd_pos.y() - viewport()->height() / 2 + m_screenH / 2) /
                    m_scaleFactor);
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
  const QPoint numDegrees = event->angleDelta();
  if (numDegrees.y() > 0) {
    setScf(m_scaleFactor * 1.1);
  } else {
    setScf(m_scaleFactor / 1.1);
  }
  event->accept();
}

void ImageViewer::adjustAll() {
  m_screenW = viewport()->width();
  m_screenH = viewport()->height();

  const int oldW = m_cW_;
  const int oldH = m_cH_;

  m_cW_ = m_screenW / m_scaleFactor + .5;
  if (m_cW_ > m_imagePtr->width()) {
    m_cW_ = m_imagePtr->width();
    m_screenW = m_cW_ * m_scaleFactor + .5;
  }
  m_cH_ = m_screenH / m_scaleFactor + .5;
  if (m_cH_ > m_imagePtr->height()) {
    m_cH_ = m_imagePtr->height();
    m_screenH = m_cH_ * m_scaleFactor + .5;
  }

  horizontalScrollBar()->setPageStep(m_cW_);
  horizontalScrollBar()->setMaximum(m_imagePtr->width() - m_cW_);
  m_xMov = qMin(qMax(0, m_xMov + (oldW - m_cW_) / 2),
                horizontalScrollBar()->maximum());
  horizontalScrollBar()->setValue(m_xMov);

  verticalScrollBar()->setPageStep(m_cH_);
  verticalScrollBar()->setMaximum(m_imagePtr->height() - m_cH_);
  m_yMov = qMin(qMax(0, m_yMov + (oldH - m_cH_) / 2),
                verticalScrollBar()->maximum());
  verticalScrollBar()->setValue(m_yMov);

  viewport()->update();
}

void ImageViewer::fitWidth() {
  setScf(double(viewport()->width()) / m_imagePtr->width());
}

void ImageViewer::setXmov(int x) {
  if (x != m_xMov) {
    m_xMov = x;
    viewport()->update();
  }
}

void ImageViewer::setYmov(int y) {
  if (y != m_yMov) {
    m_yMov = y;
    viewport()->update();
  }
}

void ImageViewer::setScf(const double s) {
  if (s != m_scaleFactor) {
    m_scaleFactor = s;
    adjustAll();
  }
}

QImage *ImageViewer::imagePtr() { return m_imagePtr; }
