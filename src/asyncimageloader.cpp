#include "asyncimageloader.h"
#include <QDebug>
#include <QImageReader>
#include <QPixmap>
#include <QThread>
const int kMaxAsyncCalls = 1024;

AsyncImageLoader::AsyncImageLoader(QObject *parent) : QThread(parent) {}

void AsyncImageLoader::Enqueue(const QString &path, const QSize &target_size,
                               const int row) {
  mutex_.lock();
  if (!pending_keys_.contains(row)) {
    pending_keys_.insert(row);
    pending_stack_.push({path, target_size, row});

    if (pending_stack_.count() > kMaxAsyncCalls) {
      int key = std::get<2>(pending_stack_.first());
      pending_keys_.remove(key);
      pending_stack_.removeFirst();
    }
    mutex_.unlock();
  } else {
    mutex_.unlock();
    return;
  }
  wait_cond_.wakeOne();
}

void AsyncImageLoader::Reset() {
  if (isRunning()) {
    mutex_.lock();
    pending_stack_.clear();
    pending_keys_.clear();
    mutex_.unlock();
  } else {
    exit_ = false;
    pending_stack_.clear();
    pending_keys_.clear();
    start();
  }
}

AsyncImageLoader::~AsyncImageLoader() {
  exit_ = true;
  wait_cond_.wakeAll();
  wait();
}

void AsyncImageLoader::LoadImage(const QString &path, const QSize &target_size,
                                 int row) {
  mutex_.lock();
  pending_keys_.remove(row);
  mutex_.unlock();

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
    if (exit_) {
      break;
    }
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
