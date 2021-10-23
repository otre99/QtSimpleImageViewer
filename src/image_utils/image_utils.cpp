#include "image_utils.h"
#include "autocropborder.h"
QRect autoCropImage(const QImage &image, const QColor &color){

    AutoCropBorder cropper(&image, color);
    return cropper.calculate();
}






