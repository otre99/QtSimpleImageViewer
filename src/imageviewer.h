#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QAbstractScrollArea>

class QImage;

class ImageViewer : public QAbstractScrollArea {
  Q_OBJECT
public:
  explicit ImageViewer(QWidget *parent = nullptr);
  void attachImagePtr(QImage *ptr);
  void init();
  QImage *imagePtr();
  double scale() const { return m_scaleFactor; }

protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *event);
signals:
  void pixelTrack(int x, int y, double f);

public slots:
  void setXmov(int x);
  void setYmov(int y);
  void setScf(double);
  void fitWidth();

private:
  QImage *m_imagePtr;
  int m_xMov, m_yMov;
  int m_cW_, m_cH_;
  int m_screenW, m_screenH;
  double m_scaleFactor;
  QPoint m_lastPt;

private:
  void adjustAll();
  void selectScf();
};

#endif // IMAGEVIEWER_H
