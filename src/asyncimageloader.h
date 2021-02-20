#ifndef ASYNCIMAGELOADER_H
#define ASYNCIMAGELOADER_H

#include <QMutex>
#include <QSet>
#include <QSize>
#include <QStack>
#include <QThread>
#include <QWaitCondition>
#include <tuple>

class AsyncImageLoader : public QThread {
  Q_OBJECT
public:
  ~AsyncImageLoader() override;
  explicit AsyncImageLoader(QObject *parent = nullptr);
  void run() override;
  void reset();

signals:
  void imageLoaded(QPixmap *pixmap, int row);

public slots:
  void enqueue(const QString &path, const QSize &target_size, int row);

private:
  QMutex m_mutex;
  QWaitCondition m_waitCond;
  QStack<std::tuple<QString, QSize, int>> m_pendingStack;
  void loadImage(const QString &path, const QSize &target_size, int row);
  bool m_exit;
  QSet<int> m_pendingKeys;
};

#endif // ASYNCIMAGELOADER_H
