#include "autocropborder.h"

AutoCropBorder::AutoCropBorder (const QImage *imagePtr, const QColor &reference_color)
    : m_imagePtr (imagePtr), m_referenceColor(reference_color)
{
    m_rect = QRect();
}

QRect AutoCropBorder::calculate()
{
    int y;
    for (y=0; y<m_imagePtr->height(); ++y){
        if ( !compareXDir(0, m_imagePtr->width(), y) ){
            break;
        }
    }
    const int y1=y;
    for (y=m_imagePtr->height()-1; y>y1; --y){
        if ( !compareXDir(0, m_imagePtr->width(), y) ){
            break;
        }
    }
    const int ny = y-y1+1;

    int x;
    for (x=0; x<m_imagePtr->width(); ++x){
        if ( !compareYDir(y1, ny, x) ){
            break;
        }
    }
    const int x1=x;
    for (x=m_imagePtr->width()-1; x>x1; --x){
        if ( !compareYDir(y1, ny, x) ){
            break;
        }
    }
    const int nx = x-x1+1;
    return {x1,y1,nx,ny};
}

bool AutoCropBorder::compareColor(const QColor &color)
{
    if (m_referenceColor.isValid()){
        return m_referenceColor==color;
    } else {
        return color.alpha() == 0;
    }
}

bool AutoCropBorder::compareXDir(int start, int n, int y)
{
    for (int i=start; i<start+n; ++i){
        if ( !compareColor( m_imagePtr->pixelColor(i,y) )  ){
            return false;
        }
    }
    return true;
}

bool AutoCropBorder::compareYDir(int start, int n, int x)
{
    for (int i=start; i<start+n; ++i){
        if ( !compareColor( m_imagePtr->pixelColor(x,i) )  ){
            return false;
        }
    }
    return true;
}

