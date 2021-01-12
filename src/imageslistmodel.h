#ifndef IMAGESLISTMODEL_H
#define IMAGESLISTMODEL_H
#include "asyncimageloader.h"
#include <QAbstractListModel>
#include <QCache>
#include <QDir>
#include <QPixmap>
#include <QObject>

class ImagesListModel : public QAbstractListModel {
  Q_OBJECT
public:
  ImagesListModel();
  void init(const QString &folder);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QString imagePath(int row) const;
  QString fileName(int row) const { return m_imageNames[row]; }

  int indexOf(const QString &imgPath) { return m_imageNames.indexOf(imgPath); };

signals:
  void loadImageRequest(const QString &, const QSize &, int) const;

public slots:
  void activateSoftLoading() { m_softLoading = true; }
  void deactivateSoftLoading() { m_softLoading = false; }

private slots:
  void loadingFinished(QPixmap *pixmap, int row);

private:
  QStringList m_imageNames;
  QDir m_imageFolder;
  QCache<int, QPixmap> m_pixmapCache;
  AsyncImageLoader m_asyncImageLoader;
  QPixmap m_fakeImage;
  bool m_softLoading;
};

#endif // IMAGESLISTMODEL_H
