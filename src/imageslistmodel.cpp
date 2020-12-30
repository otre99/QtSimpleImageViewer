#include "imageslistmodel.h"

#include <QDebug>
#include <QImageReader>

namespace {
static const QStringList img_ext = {"*.jpg",  "*.JPG", "*.jpeg",
                                    "*.JPEG", "*.png", "*.PNG"};
static const int kDisplayIconSize = 128;
static const size_t kMaxImageInCache =
    (256 * 1024 * 1024) /
    (kDisplayIconSize * kDisplayIconSize * 4);  // 256 Mb of cache
}  // namespace

ImagesListModel::ImagesListModel() {
  connect(this, &ImagesListModel::LoadImageRequest, &async_image_loader_,
          &AsyncImageLoader::Enqueue);
  connect(&async_image_loader_, &AsyncImageLoader::ImageLoaded, this,
          &ImagesListModel::LoadingFinished);
}

QString ImagesListModel::GetImagePath(int row) const {
  return image_folder_.filePath(image_names_[row]);
}

void ImagesListModel::Init(const QString &folder) {
  QFileInfo info(folder);
  image_folder_.setPath(folder);
  if (info.exists() && info.isFile()) {
    image_folder_.cdUp();
  }

  beginResetModel();
  image_names_ = image_folder_.entryList(img_ext, QDir::Files);
  endResetModel();
  pixmap_cache_.clear();
  pixmap_cache_.setMaxCost(
      kMaxImageInCache);  // TODO (otre99): avoid  hardcoding!!!!

  QImage loadinIcon(":/icons/icons/loading.png");
  fake_image_ = QPixmap::fromImage(
      loadinIcon.scaled(QSize(kDisplayIconSize, kDisplayIconSize)));
  async_image_loader_.Reset();
  soft_loading_ = false;
}

int ImagesListModel::rowCount(const QModelIndex & /*parent*/) const {
  return image_names_.count();
}

QVariant ImagesListModel::data(const QModelIndex &index, const int role) const {
  if (index.isValid() && role == Qt::DecorationRole) {
    const int key = index.row();
    if (pixmap_cache_.contains(key)) return *pixmap_cache_[key];
    const QString image_path = GetImagePath(key);
    if (!soft_loading_)
      emit LoadImageRequest(image_path,
                            QSize(kDisplayIconSize, kDisplayIconSize), key);
    return fake_image_;
  } else if (role == Qt::DisplayRole) {
    return QString::number(index.row());
  }
  return QVariant();
}

void ImagesListModel::LoadingFinished(QPixmap *pixmap, int row) {
  if (pixmap) {
    pixmap_cache_.insert(row, pixmap);
    QModelIndex tl = index(row);
    emit dataChanged(tl, tl);
  }
}
