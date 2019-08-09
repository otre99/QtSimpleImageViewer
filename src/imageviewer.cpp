#include "imageviewer.h"
#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QDebug>

ImageViewer::ImageViewer(QWidget *parent) :
    QAbstractScrollArea(parent)
{
    init();
    connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), SLOT(setXmov(int)));
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(setYmov(int)));
    viewport()->setMouseTracking(true);
    viewport()->setCursor(QCursor(Qt::CrossCursor));
}

void ImageViewer::attachImagePtr(QImage *ptr)
{
    m_ptrImage=ptr;
    adjustAll();
}

void ImageViewer::init()
{
    m_ptrImage=nullptr;
    scf=1.0;
    xmov = ymov = 0;
}

void ImageViewer::resizeEvent(QResizeEvent *)
{
    if (m_ptrImage) adjustAll();
}

void ImageViewer::paintEvent(QPaintEvent *)
{
    if (!m_ptrImage) return;
    QPainter p(viewport());
    p.translate( QPoint(viewport()->width()/2, viewport()->height()/2) );
    p.drawImage(QRect(-screenW/2,-screenH/2, screenW, screenH), *m_ptrImage, QRect(xmov, ymov, cw, ch));
}

void ImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint pdPos = viewport()->mapFrom(this, e->pos());
    const int xx = xmov + (pdPos.x() - viewport()->width()/2 + screenW/2)/scf;
    const int yy = ymov + (pdPos.y() - viewport()->height()/2 + screenH/2)/scf;
    emit pixelTrack(xx, yy);
}


void ImageViewer::adjustAll()
{
    screenW = viewport()->width();
    screenH = viewport()->height();
    cw = screenW/scf+.5;
    if (cw > m_ptrImage->width()){
        cw = m_ptrImage->width();
        screenW = cw*scf+.5;
    }
    ch = screenH/scf+.5;
    if (ch > m_ptrImage->height()){
        ch = m_ptrImage->height();
        screenH = ch*scf+.5;
    }
    horizontalScrollBar()->setPageStep(cw);
    horizontalScrollBar()->setMaximum(m_ptrImage->width()-cw);

    verticalScrollBar()->setPageStep(ch);
    verticalScrollBar()->setMaximum(m_ptrImage->height()-ch);
    viewport()->update();
}

void ImageViewer::setXmov(int x)
{
    xmov = x;
    viewport()->update();
}

void ImageViewer::setYmov(int y)
{
    ymov = y;
    viewport()->update();
}

void ImageViewer::setScf(const double s)
{
    scf = s;
    adjustAll();
}

QImage *ImageViewer::imagePtr()
{
    return m_ptrImage;
}
