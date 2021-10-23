#ifndef CROPWIDGET_H
#define CROPWIDGET_H

#include <QWidget>

class ImageViewer;

class CropWidget : public QWidget {
  Q_OBJECT
  enum Corner {
    kInvaid = -2,
    kCenter = -1,
    kTopLeft = 0,
    kTopRight = 1,
    kBottomRight = 2,
    kBottomLeft = 3,
    kTopCenter,
    kRightCenter,
    kBottomCenter,
    kLeftCenter
  } m_currentCorner;

public:
  explicit CropWidget(QWidget *parent = nullptr);
  int marging() const;
  QRect cropRect() const;
  QRect cropRectImage() const;
  void setImageViewer(ImageViewer *viewer);

protected:
  void paintEvent(QPaintEvent *event) override final;
  void mouseMoveEvent(QMouseEvent *) override final;
  void mousePressEvent(QMouseEvent *) override final;
  void mouseReleaseEvent(QMouseEvent *) override final;

signals:
  void info(const QString &r);

private:
  QPoint m_lastPoint;
  QRect buildRectFromTwoPoints(const QPoint &p1, const QPoint &p2, bool &sw,
                               bool &sh);
  Corner positionInsideBBox(const QPoint &pos);
  QColor m_bboxColor;
  QColor m_arrowColor;
  QColor m_sqColor;
  ImageViewer *m_imgViewer;
};

#endif // CROPWIDGET_H
