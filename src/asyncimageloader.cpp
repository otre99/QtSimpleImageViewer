#include "asyncimageloader.h"

#include <QImageReader>
#include <QPainter>
#include <QPixmap>
#include <QThread>

#include "imageslistmodel.h"

const int kMaxAsyncCalls = 1024;

AsyncImageLoader::AsyncImageLoader(QObject *parent) : QThread(parent) {
  m_imgModel = qobject_cast<ImagesListModel *>(parent);
}

void AsyncImageLoader::enqueue(vector<int> rows) {
  m_mutex.lock();
  m_pendingKeys = rows;
  m_mutex.unlock();
  m_waitCond.wakeOne();
}

void AsyncImageLoader::reset() {
  if (isRunning()) {
    m_mutex.lock();
    m_pendingKeys.clear();
    m_mutex.unlock();
  } else {
    m_exit = false;
    m_pendingKeys.clear();
    start();
  }
}

AsyncImageLoader::~AsyncImageLoader() {
  m_exit = true;
  m_waitCond.wakeAll();
  wait();
}

void AsyncImageLoader::loadImage(const QString &path, const QSize &target_size,
                                 int row) {
  QImageReader reader(path);
  reader.setAutoTransform(true);
  if (reader.canRead()) {
    double sw =
        static_cast<double>(target_size.width()) / reader.size().width();
    double sh =
        static_cast<double>(target_size.height()) / reader.size().height();
    double ss = std::min(sw, sh);
    reader.setScaledSize(reader.size() * ss);

    const QImage img = reader.read();
    QPixmap *pixmap = new QPixmap(target_size);
    pixmap->fill(Qt::transparent);
    QPainter painter(pixmap);
    int x = (target_size.width() - img.width()) / 2;
    int y = (target_size.height() - img.height()) / 2;
    painter.drawImage(QRect{{x, y}, img.size()}, img);
    painter.end();
    emit imageLoaded(pixmap, row);
  }
  emit imageLoaded(nullptr, row);
}

void AsyncImageLoader::run() {
  int row;
  for (;;) {
    if (m_exit) break;

    for (;;) {
      m_mutex.lock();
      size_t n = m_pendingKeys.size();
      if (n) {
        row = m_pendingKeys[n - 1];
        m_pendingKeys.pop_back();
      }
      m_mutex.unlock();
      if (m_exit || !n) break;

      const QString path = m_imgModel->imagePath(row);
      QSize target_size{160, 160};
      loadImage(path, target_size, row);
    }

    m_mutex.lock();
    m_waitCond.wait(&m_mutex);
    m_mutex.unlock();
  }
}
