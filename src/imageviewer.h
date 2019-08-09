#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QAbstractScrollArea>

class QImage;

class ImageViewer : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void attachImagePtr(QImage *ptr);
    void init();
    QImage *imagePtr();


protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

signals:
    void pixelTrack(int x, int y);
    
public slots:
    void setXmov(int x);
    void setYmov(int y);
    void setScf( double );

private:
    QImage *m_ptrImage;
    int xmov, ymov;
    int imgW, cw, ch;
    int screenW, screenH;
    double scf;

private:
    void adjustAll();
};

#endif // IMAGEVIEWER_H
