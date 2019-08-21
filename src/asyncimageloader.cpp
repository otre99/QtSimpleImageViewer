#include "asyncimageloader.h"
#include <QDebug>
#include <QImageReader>
#include <QPixmap>
#include <QThread>

AsyncImageLoader::AsyncImageLoader(QObject *parent) : QThread(parent) {}

void AsyncImageLoader::Enqueue(const QString &path, const QSize &target_size,
                               const int row) {
  mutex_.lock();
  pending_stack_.push({path, target_size, row});
  mutex_.unlock();
  wait_cond_.wakeOne();
}

void AsyncImageLoader::Reset() {
  if (isRunning()) {
    mutex_.lock();
    pending_stack_.clear();
    mutex_.unlock();
  } else {
    pending_stack_.clear();
    start();
  }
}

void AsyncImageLoader::LoadImage(const QString &path, const QSize &target_size,
                                 int row) {
  QImageReader reader(path);
  if (reader.canRead()) {
    reader.setScaledSize(target_size);
    auto *pixmap = new QPixmap(QPixmap::fromImage(reader.read()));
    emit ImageLoaded(pixmap, row);
  }
  emit ImageLoaded(nullptr, row);
}

void AsyncImageLoader::run() {
  QString path;
  QSize target_size;
  int row;
  for (;;) {
    mutex_.lock();
    if (!pending_stack_.empty()) {
      std::tie(path, target_size, row) = pending_stack_.pop();
      mutex_.unlock();
      LoadImage(path, target_size, row);
    } else {
      wait_cond_.wait(&mutex_);
      mutex_.unlock();
    }
  }
}
