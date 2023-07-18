#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QAbstractScrollArea>
#include <QPixmap>
class QImage;

class ImageViewer : public QAbstractScrollArea {
  Q_OBJECT
public:
  explicit ImageViewer(QWidget *parent = nullptr);
  void attachImagePtr(QImage *ptr);
  void init();
  QImage *imagePtr();
  double scale() const { return m_scaleFactor; }
  QRect canvasRect() const;
  QRect imageRect() const;

protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *event) override;
  //  void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
  void pixelTrack(int x, int y, double f);

public slots:
  void setXmov(int x);
  void setYmov(int y);
  void setScf(double);
  void fitWidth();
  void generateCache();
  void adjustAll(int xMov = -1, int yMov = -1);

private:
  QImage *m_imagePtr;
  int m_xMov, m_yMov;
  int m_cW, m_cH;
  int m_screenW, m_screenH;
  double m_scaleFactor;
  QPoint m_lastPt;
  QPoint m_lastPs;
  void selectScf();
  QTimer *cacheTimer_;
  void queueGenerateCache();
  QPixmap m_cachedPixmap;
};

#endif // IMAGEVIEWER_H
