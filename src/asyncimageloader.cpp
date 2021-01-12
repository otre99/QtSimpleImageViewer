#include "asyncimageloader.h"
#include <QImageReader>
#include <QPixmap>
#include <QThread>

const int kMaxAsyncCalls = 1024;

AsyncImageLoader::AsyncImageLoader(QObject *parent) : QThread(parent) {}

void AsyncImageLoader::enqueue(const QString &path, const QSize &target_size,
                               const int row) {
  m_mutex.lock();
  if (!m_pendingKeys.contains(row)) {
    m_pendingKeys.insert(row);
    m_pendingStack.push({path, target_size, row});

    if (m_pendingStack.count() > kMaxAsyncCalls) {
      int key = std::get<2>(m_pendingStack.first());
      m_pendingKeys.remove(key);
      m_pendingStack.removeFirst();
    }
    m_mutex.unlock();
  } else {
    m_mutex.unlock();
    return;
  }
  m_waitCond.wakeOne();
}

void AsyncImageLoader::reset() {
  if (isRunning()) {
    m_mutex.lock();
    m_pendingStack.clear();
    m_pendingKeys.clear();
    m_mutex.unlock();
  } else {
    m_exit = false;
    m_pendingStack.clear();
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
  m_mutex.lock();
  m_pendingKeys.remove(row);
  m_mutex.unlock();

  QImageReader reader(path);
  if (reader.canRead()) {
    reader.setScaledSize(target_size);
    auto *pixmap = new QPixmap(QPixmap::fromImage(reader.read()));
    emit imageLoaded(pixmap, row);
  }
  emit imageLoaded(nullptr, row);
}

void AsyncImageLoader::run() {
  QString path;
  QSize target_size;
  int row;
  for (;;) {
    if (m_exit) {
      break;
    }
    m_mutex.lock();
    if (!m_pendingStack.empty()) {
      std::tie(path, target_size, row) = m_pendingStack.pop();
      m_mutex.unlock();
      loadImage(path, target_size, row);
    } else {
      m_waitCond.wait(&m_mutex);
      m_mutex.unlock();
    }
  }
}
