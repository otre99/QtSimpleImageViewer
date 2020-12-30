#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QAbstractScrollArea>

class QImage;

class ImageViewer : public QAbstractScrollArea {
  Q_OBJECT
 public:
  explicit ImageViewer(QWidget *parent = nullptr);
  void AttachImagePtr(QImage *ptr);
  void Init();
  QImage *ImagePtr();
  double GetScale() const { return scf_; }

 protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *ev) override;
  void mouseMoveEvent(QMouseEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void wheelEvent(QWheelEvent *event);
 signals:
  void PixelTrack(int x, int y, double f);

 public slots:
  void SetXmov(int x);
  void SetYmov(int y);
  void SetScf(double);
  void FixWidth();

 private:
  QImage *image_ptr_;
  int xmov_, ymov_;
  int img_w_, cw_, ch_;
  int screen_w_, screen_h_;
  double scf_;
  QPoint last_pt_;

 private:
  void AdjustAll();
  void SelectScf();
};

#endif  // IMAGEVIEWER_H
