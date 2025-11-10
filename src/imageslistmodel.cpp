#include "imageslistmodel.h"

#include <QImageReader>

#include "asyncimageloader.h"

namespace {
Q_GLOBAL_STATIC(QStringList, kImgExts)

static const int kDisplayIconSize = 160;
static const size_t kMaxImageInCache =
    (256 * 1024 * 1024) /
    (kDisplayIconSize * kDisplayIconSize * 4);  // 256 Mb of cache
}  // namespace

ImagesListModel::ImagesListModel() {
  // connect(this, &ImagesListModel::loadImageRequest, &m_asyncImageLoader,
  //        &AsyncImageLoader::enqueue);

  m_asyncImageLoader = new AsyncImageLoader(this);
  connect(m_asyncImageLoader, &AsyncImageLoader::imageLoaded, this,
          &ImagesListModel::loadingFinished);

  const auto imgFmt = QImageReader::supportedImageFormats();
  for (const QByteArray &fmt : imgFmt) {
    QString wc(fmt);
    kImgExts->append("*." + wc);
    kImgExts->append("*." + wc.toUpper());
  }
}

QString ImagesListModel::imagePath(int row) const {
  return m_imageFolder.filePath(m_imageNames[row]);
}

void ImagesListModel::init(const QString &folder) {
  QFileInfo info(folder);
  m_imageFolder.setPath(folder);
  if (info.exists() && info.isFile()) {
    m_imageFolder.cdUp();
  }

  // qDebug() << "INIT ";

  beginResetModel();
  m_imageNames = m_imageFolder.entryList(*kImgExts, QDir::Files);
  endResetModel();
  m_pixmapCache.clear();
  m_pixmapCache.setMaxCost(
      kMaxImageInCache);  // TODO (otre99): avoid  hardcoding!!!!

  QImage loadinIcon(":/icons/icons/loading.png");
  m_fakeImage = QPixmap::fromImage(
      loadinIcon.scaled(QSize(kDisplayIconSize, kDisplayIconSize)));
  m_asyncImageLoader->reset();
  m_softLoading = false;
}

void ImagesListModel::refreshItem(int row) { m_pixmapCache.remove(row); }

bool ImagesListModel::needToBeLoaded(int row) const {
  return !m_pixmapCache.contains(row);
}

AsyncImageLoader *ImagesListModel::loader() { return m_asyncImageLoader; }

int ImagesListModel::rowCount(const QModelIndex & /*parent*/) const {
  return m_imageNames.count();
}

QVariant ImagesListModel::data(const QModelIndex &index, const int role) const {
  if (!index.isValid()) return {};
  const int row = index.row();
  switch (role) {
    case Qt::DecorationRole:
      return m_pixmapCache.contains(row) ? *m_pixmapCache[row] : m_fakeImage;
    case Qt::DisplayRole:
      return fileName(row);
  }
  return QVariant();
}

void ImagesListModel::loadingFinished(QPixmap *pixmap, int row) {
  if (pixmap) {
    m_pixmapCache.insert(row, pixmap);
    QModelIndex tl = index(row);
    emit dataChanged(tl, tl);
  }
}
