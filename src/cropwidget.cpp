#include "cropwidget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "imageviewer.h"

namespace {
const int kMinimunRectSize = 16 << 1;
}

CropWidget::CropWidget(QWidget *parent) : QWidget(parent) {
  m_imgViewer = nullptr;
  const QColor baseColor(Qt::red);
  m_bboxColor = baseColor;
  m_bboxColor.setAlpha(128);
  m_arrowColor = baseColor.darker(300);
  m_arrowColor.setAlpha(200);
  m_sqColor = m_arrowColor;
  m_sqColor.setAlpha(128);
  m_currentCorner = kInvaid;
}

int CropWidget::marging() const { return kMinimunRectSize >> 1; }

QRect CropWidget::cropRect() const {
  const int d = kMinimunRectSize >> 1;
  return geometry().adjusted(d, d, -d, -d);
}

QRect CropWidget::cropRectImage() const {
  if (m_imgViewer) {
    QRect imgRect = m_imgViewer->imageRect();
    QRect canvasRect = m_imgViewer->canvasRect();
    QRect cropRect = canvasRect.intersected(this->cropRect());
    const double x1 = (cropRect.x() - canvasRect.x()) /
                          static_cast<double>(canvasRect.width()) *
                          imgRect.width() +
                      imgRect.x();
    const double y1 = (cropRect.y() - canvasRect.y()) /
                          static_cast<double>(canvasRect.height()) *
                          imgRect.height() +
                      imgRect.y();
    const double x2 = (cropRect.right() - canvasRect.x() + 1) /
                          static_cast<double>(canvasRect.width()) *
                          imgRect.width() +
                      imgRect.x();
    const double y2 = (cropRect.bottom() - canvasRect.y() + 1) /
                          static_cast<double>(canvasRect.height()) *
                          imgRect.height() +
                      imgRect.y();
    return imgRect.intersected(
        QRectF(QPointF{x1, y1}, QPointF{x2, y2}).toRect());
  }
  return {};
}

void CropWidget::paintEvent(QPaintEvent *event) {
  (void)event;
  QPainter painter(this);
  const int d = kMinimunRectSize >> 1;
  const QRect brect = painter.viewport().adjusted(d, d, -d, -d);

  if (m_currentCorner == kInvaid) {
    painter.fillRect(brect, QColor(128, 128, 128, 64));
  } else {
    painter.fillRect(brect, m_bboxColor);
  }
  painter.setPen(Qt::NoPen);
  painter.setBrush(m_sqColor);
  QPainterPath path;
  if (m_currentCorner == kTopLeft) {
    QPainterPath path(QPoint{brect.x(), brect.y() + d});
    path.lineTo(brect.topLeft());
    path.lineTo(QPoint{brect.x() + d, brect.y()});
    painter.fillPath(path, m_arrowColor);
  } else {
    painter.drawRect(QRect(brect.x(), brect.y(), d, d));
  }

  if (m_currentCorner == kTopRight) {
#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
    path = QPainterPath();
#else
    path.clear();
#endif
    path.moveTo(QPoint{brect.right() - d, brect.y()});
    path.lineTo(QPoint{brect.right() + 1, brect.y()});
    path.lineTo(QPoint{brect.right() + 1, brect.y() + d});
    painter.fillPath(path, m_arrowColor);
  } else {
    painter.drawRect(QRect(brect.right() - d + 1, brect.y(), d, d));
  }

  if (m_currentCorner == kBottomRight) {
#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
    path = QPainterPath();
#else
    path.clear();
#endif
    path.moveTo(QPoint{brect.right() + 1, brect.bottom() - d});
    path.lineTo(QPoint{brect.right() + 1, brect.bottom() + 1});
    path.lineTo(QPoint{brect.right() - d, brect.bottom() + 1});
    painter.fillPath(path, m_arrowColor);
  } else {
    painter.drawRect(
        QRect(brect.right() - d + 1, brect.bottom() - d + 1, d, d));
  }

  painter.drawRect(QRect(brect.x(), brect.bottom() - d + 1, d, d));
}

void CropWidget::mouseMoveEvent(QMouseEvent *ev) {
  const QPoint cpos = ev->pos() + this->pos();
  QPoint dl = cpos - m_lastPoint;
  m_lastPoint = cpos;
  const auto p = dynamic_cast<QWidget *>(parent());
  QRect newrect(geometry());
  if (m_currentCorner == kCenter) {
    newrect.translate(dl);
  } else {
    switch (m_currentCorner) {
    case kTopLeft:
      if (newrect.width() - dl.x() < kMinimunRectSize)
        dl.setX(newrect.width() - kMinimunRectSize);
      if (newrect.height() - dl.y() < kMinimunRectSize)
        dl.setY(newrect.height() - kMinimunRectSize);
      newrect.adjust(dl.x(), dl.y(), 0, 0);
      break;
    case kTopRight:
      if (newrect.width() + dl.x() < kMinimunRectSize)
        dl.setX(kMinimunRectSize - newrect.width());
      if (newrect.height() - dl.y() < kMinimunRectSize)
        dl.setY(newrect.height() - kMinimunRectSize);
      newrect.adjust(0, dl.y(), dl.x(), 0);
      break;
    case kBottomRight:
      if (newrect.width() + dl.x() < kMinimunRectSize)
        dl.setX(kMinimunRectSize - newrect.width());
      if (newrect.height() + dl.y() < kMinimunRectSize)
        dl.setY(kMinimunRectSize - newrect.height());
      newrect.adjust(0, 0, dl.x(), dl.y());
      break;
    case kBottomLeft:
      if (newrect.width() - dl.x() < kMinimunRectSize)
        dl.setX(newrect.width() - kMinimunRectSize);
      if (newrect.height() + dl.y() < kMinimunRectSize)
        dl.setY(kMinimunRectSize - newrect.height());
      newrect.adjust(dl.x(), 0, 0, dl.y());
      break;
    case kTopCenter:
      dl.setX(0);
      if (newrect.height() - dl.y() < kMinimunRectSize)
        dl.setY(newrect.height() - kMinimunRectSize);
      newrect.adjust(0, dl.y(), 0, 0);
      break;
    case kBottomCenter:
      dl.setX(0);
      if (newrect.height() + dl.y() < kMinimunRectSize)
        dl.setY(kMinimunRectSize - newrect.height());
      newrect.adjust(0, 0, 0, dl.y());
      break;
    case kRightCenter:
      dl.setY(0);
      if (newrect.width() + dl.x() < kMinimunRectSize)
        dl.setX(kMinimunRectSize - newrect.width());
      newrect.adjust(0, 0, dl.x(), 0);
      break;
    case kLeftCenter:
      dl.setY(0);
      if (newrect.width() - dl.x() < kMinimunRectSize)
        dl.setX(newrect.width() - kMinimunRectSize);
      newrect.adjust(dl.x(), 0, 0, 0);
      break;
    default:;
    }
  }
  const int d = kMinimunRectSize >> 1;
  const QRect parentRect = p->geometry().adjusted(-d - 1, -d - 1, d, d);
  newrect &= parentRect;
  if (newrect.isValid()) {
    setGeometry(newrect);
  }
  ev->accept();
}

void CropWidget::setImageViewer(ImageViewer *viewer) { m_imgViewer = viewer; }

void CropWidget::mouseReleaseEvent(QMouseEvent *ev) {
  m_currentCorner = kInvaid;
  update();

  if (m_imgViewer) {
    QRect finalRect = cropRectImage();
    QString r = QString("Crop Rect -> [ (%1 , %2) %3 X %4]")
                    .arg(finalRect.x())
                    .arg(finalRect.y())
                    .arg(finalRect.width())
                    .arg(finalRect.height());
    emit info(r);
  }
  setCursor(Qt::SizeAllCursor);
  ev->accept();
}

QRect CropWidget::buildRectFromTwoPoints(const QPoint &p1, const QPoint &p2,
                                         bool &sw, bool &sh) {
  sw = p2.x() < p1.x();
  sh = p2.y() < p1.y();
  double rw = qAbs(p2.x() - p1.x()) + 1;
  double rh = qAbs(p2.y() - p1.y()) + 1;
  QPoint topleft(qMin(p1.x(), p2.x()), qMin(p1.y(), p2.y()));
  return {topleft, QSize(rw, rh)};
}

CropWidget::Corner CropWidget::positionInsideBBox(const QPoint &pos) {
  const int x = pos.x();
  const int y = pos.y();
  Corner result = kCenter;
  const int d = kMinimunRectSize >> 1;
  const QRect brect = geometry().adjusted(d, d, -d, -d);
  if ((brect.x() <= x) && (x <= brect.x() + d) && (brect.y() <= y) &&
      (y <= brect.y() + d)) {
    result = kTopLeft;
    setCursor(Qt::SizeFDiagCursor);
  } else if ((brect.x() + brect.width() - d <= x) &&
             (x <= brect.x() + brect.width()) && (brect.y() <= y) &&
             (y <= brect.y() + d)) {
    result = kTopRight;
    setCursor(Qt::SizeBDiagCursor);
  } else if ((brect.x() + brect.width() - d <= x) &&
             (x <= brect.x() + brect.width()) &&
             (brect.y() + brect.height() - d <= y) &&
             (y <= brect.y() + brect.height())) {
    result = kBottomRight;
    setCursor(Qt::SizeFDiagCursor);
  } else if ((brect.x() <= x) && (x <= brect.x() + d) &&
             (brect.y() + brect.height() - d <= y) &&
             (y <= brect.y() + brect.height())) {
    result = kBottomLeft;
    setCursor(Qt::SizeBDiagCursor);
  } else if ((brect.x() + d <= x) && (x <= brect.x() + brect.width() - d) &&
             (brect.y() <= y) && (y <= brect.y() + d)) {
    result = kTopCenter;
    setCursor(Qt::SizeVerCursor);
  } else if ((brect.x() + brect.width() - d <= x) &&
             (x <= brect.x() + brect.width()) && (brect.y() + d <= y) &&
             (y <= brect.y() + brect.height() - d)) {
    result = kRightCenter;
    setCursor(Qt::SizeHorCursor);
  } else if ((brect.x() + d <= x) && (x <= brect.x() + brect.width() - d) &&
             (brect.y() + brect.height() - d <= y) &&
             (y <= brect.y() + brect.height())) {
    result = kBottomCenter;
    setCursor(Qt::SizeVerCursor);
  } else if ((brect.x() <= x) && (x <= brect.x() + d) && (brect.y() + d <= y) &&
             (y <= brect.y() + brect.height() - d)) {
    result = kLeftCenter;
    setCursor(Qt::SizeHorCursor);
  } else {
    setCursor(Qt::SizeAllCursor);
  }
  return result;
}

void CropWidget::mousePressEvent(QMouseEvent *ev) {
  m_lastPoint = ev->pos() + this->pos();
  m_currentCorner = positionInsideBBox(m_lastPoint);
  update();
};
