#ifndef ASYNCIMAGELOADER_H
#define ASYNCIMAGELOADER_H

#include <QMutex>
#include <QSize>
#include <QStack>
#include <QThread>
#include <QWaitCondition>
#include <tuple>

class AsyncImageLoader : public QThread {
  Q_OBJECT
  QMutex mutex_;
  QWaitCondition wait_cond_;
  QStack<std::tuple<QString, QSize, int>> pending_stack_;
  void LoadImage(const QString &path, const QSize &target_size, int row);
  bool exit_;

public:
  ~AsyncImageLoader() override;
  explicit AsyncImageLoader(QObject *parent = nullptr);
  void run() override;
  void Reset();

signals:
  void ImageLoaded(QPixmap *pixmap, int row);

public slots:
  void Enqueue(const QString &path, const QSize &target_size, int row);
};

#endif // ASYNCIMAGELOADER_H
