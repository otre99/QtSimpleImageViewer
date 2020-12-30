#include "imageviewer.h"

#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>

ImageViewer::ImageViewer(QWidget *parent) : QAbstractScrollArea(parent) {
  Init();
  connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this,
          &ImageViewer::SetXmov);
  connect(verticalScrollBar(), &QScrollBar::sliderMoved, this,
          &ImageViewer::SetYmov);

  viewport()->setMouseTracking(true);
  viewport()->setCursor(QCursor(Qt::CrossCursor));
}

void ImageViewer::AttachImagePtr(QImage *ptr) {
  image_ptr_ = ptr;
  SelectScf();
  AdjustAll();
  setEnabled(image_ptr_ != nullptr);
}

void ImageViewer::Init() {
  image_ptr_ = nullptr;
  scf_ = 1.0;
  xmov_ = ymov_ = 0;
  cw_ = ch_ = 0;
  setEnabled(false);
}

void ImageViewer::SelectScf() {
  const double s1 = double(viewport()->width()) / image_ptr_->width();
  const double s2 = double(viewport()->height()) / image_ptr_->height();
  scf_ = std::min(std::min(s1, s2), 1.0);
  if (scf_ == 0.0) scf_ = 1.0;
}

void ImageViewer::resizeEvent(QResizeEvent *ev) {
  QAbstractScrollArea::resizeEvent(ev);
  if (image_ptr_) {
    AdjustAll();
  }
}

void ImageViewer::paintEvent(QPaintEvent *) {
  if (!image_ptr_) return;
  QPainter p(viewport());

  p.translate(QPoint(viewport()->width() / 2, viewport()->height() / 2));
  p.drawImage(QRect(-screen_w_ / 2, -screen_h_ / 2, screen_w_, screen_h_),
              *image_ptr_, QRect(xmov_, ymov_, cw_, ch_));
}

void ImageViewer::mouseMoveEvent(QMouseEvent *e) {
  const QPoint pd_pos = viewport()->mapFrom(this, e->pos());
  const int xxf = (pd_pos.x() - viewport()->width() / 2 + screen_w_ / 2) / scf_;
  const int yyf =
      (pd_pos.y() - viewport()->height() / 2 + screen_h_ / 2) / scf_;
  const int xx = xmov_ + xxf;
  const int yy = ymov_ + yyf;
  emit PixelTrack(xx, yy, scf_);

  if (e->buttons() & Qt::LeftButton) {
    xmov_ =
        std::max(std::min(last_pt_.x() - xxf, image_ptr_->width() - cw_), 0);
    ymov_ =
        std::max(std::min(last_pt_.y() - yyf, image_ptr_->height() - ch_), 0);
    horizontalScrollBar()->setValue(xmov_);
    verticalScrollBar()->setValue(ymov_);
  }
}

void ImageViewer::mousePressEvent(QMouseEvent *e) {
  QPoint pd_pos = viewport()->mapFrom(this, e->pos());
  last_pt_.setX(xmov_ +
                (pd_pos.x() - viewport()->width() / 2 + screen_w_ / 2) / scf_);
  last_pt_.setY(ymov_ +
                (pd_pos.y() - viewport()->height() / 2 + screen_h_ / 2) / scf_);
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
  const QPoint numDegrees = event->angleDelta();
  if (numDegrees.y() > 0) {
    SetScf(scf_ * 1.1);
  } else {
    SetScf(scf_ / 1.1);
  }
  event->accept();
}

void ImageViewer::AdjustAll() {
  screen_w_ = viewport()->width();
  screen_h_ = viewport()->height();

  const int old_w = cw_;
  const int old_h = ch_;

  cw_ = screen_w_ / scf_ + .5;
  if (cw_ > image_ptr_->width()) {
    cw_ = image_ptr_->width();
    screen_w_ = cw_ * scf_ + .5;
  }
  ch_ = screen_h_ / scf_ + .5;
  if (ch_ > image_ptr_->height()) {
    ch_ = image_ptr_->height();
    screen_h_ = ch_ * scf_ + .5;
  }

  horizontalScrollBar()->setPageStep(cw_);
  horizontalScrollBar()->setMaximum(image_ptr_->width() - cw_);
  xmov_ = qMin(qMax(0, xmov_ + (old_w - cw_) / 2),
               horizontalScrollBar()->maximum());
  horizontalScrollBar()->setValue(xmov_);

  verticalScrollBar()->setPageStep(ch_);
  verticalScrollBar()->setMaximum(image_ptr_->height() - ch_);
  ymov_ =
      qMin(qMax(0, ymov_ + (old_h - ch_) / 2), verticalScrollBar()->maximum());
  verticalScrollBar()->setValue(ymov_);

  viewport()->update();
}

void ImageViewer::FixWidth() {
  SetScf(double(viewport()->width()) / image_ptr_->width());
}

void ImageViewer::SetXmov(int x) {
  if (x != xmov_) {
    xmov_ = x;
    viewport()->update();
  }
}

void ImageViewer::SetYmov(int y) {
  if (y != ymov_) {
    ymov_ = y;
    viewport()->update();
  }
}

void ImageViewer::SetScf(const double s) {
  if (s != scf_) {
    scf_ = s;
    AdjustAll();
  }
}

QImage *ImageViewer::ImagePtr() { return image_ptr_; }
