#ifndef AUTOCROPBORDER_H
#define AUTOCROPBORDER_H
#include <QImage>


class AutoCropBorder
{
public:
    AutoCropBorder (const QImage *imagePtr = nullptr, const QColor &reference_color=QColor::Invalid);
    QRect calculate();
private:
    const QImage *m_imagePtr;
    QRect m_rect;
    QColor m_referenceColor;

    bool compareColor(const QColor &color);
    bool compareXDir(int start, int n, int y);
    bool compareYDir(int start, int n, int x);


};

#endif // AUTOCROPBORDER_H
