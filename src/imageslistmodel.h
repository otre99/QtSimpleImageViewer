#ifndef IMAGESLISTMODEL_H
#define IMAGESLISTMODEL_H
#include "asyncimageloader.h"
#include <QAbstractListModel>
#include <QCache>
#include <QDir>
#include <QPixmap>

class ImagesListModel : public QAbstractListModel {
  Q_OBJECT
public:
  ImagesListModel();
  void Init(const QString &folder);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QString GetImagePath(int row) const;

signals:
  void LoadImageRequest(const QString &, const QSize &, int) const;

public slots:
  void ActivateSoftLoading() { soft_loading_ = true; }
  void DeactivateSoftLoading() { soft_loading_ = false; }

private slots:
  void LoadingFinished(QPixmap *pixmap, int row);

private:
  QStringList image_names_;
  QDir image_folder_;
  mutable QCache<int, QPixmap> pixmap_cache_;
  AsyncImageLoader async_image_loader_;
  QPixmap fake_image_;
  bool soft_loading_;
};

#endif // IMAGESLISTMODEL_H
