#include "imageslistmodel.h"
#include <QDebug>
#include <QImageReader>
#include <QPixmap>

namespace {
const QStringList img_ext = {"*.jpg", "*.JPG", "*.png", "*.PNG"};
const int kDisplayIconSize = 64;
} // namespace

ImagesListModel::ImagesListModel() = default;

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
  pixmap_cache_.setMaxCost(1000); // TODO (otre99): avoid  hardcoding!!!!
}

int ImagesListModel::rowCount(const QModelIndex & /*parent*/) const {
  return image_names_.count();
}

QVariant ImagesListModel::data(const QModelIndex &index, const int role) const {
  if (index.isValid() && role == Qt::DecorationRole) {
    const QString image_path = GetImagePath(index.row());
    QImageReader reader(image_path);
    if (reader.canRead()) {
      reader.setScaledSize(QSize(kDisplayIconSize, kDisplayIconSize));

      int key = index.row();
      if (pixmap_cache_.contains(key))
        return *pixmap_cache_[key];

      pixmap_cache_.insert(key, new QPixmap(QPixmap::fromImage(reader.read())));
      return *pixmap_cache_[key];
    }
  }
  return QVariant();
}
