#ifndef ASYNCIMAGELOADER_H
#define ASYNCIMAGELOADER_H

#include <QMutex>
#include <QSet>
#include <QSize>
#include <QStack>
#include <QThread>
#include <QWaitCondition>
#include <vector>

using namespace std;
class ImagesListModel;

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
  void enqueue(vector<int> rows);

 private:
  QMutex m_mutex;
  QWaitCondition m_waitCond;
  void loadImage(const QString &path, const QSize &target_size, int row);
  atomic_bool m_exit;
  vector<int> m_pendingKeys;
  ImagesListModel *m_imgModel;
};

#endif  // ASYNCIMAGELOADER_H
