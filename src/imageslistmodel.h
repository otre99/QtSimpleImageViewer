#ifndef IMAGESLISTMODEL_H
#define IMAGESLISTMODEL_H
#include <QAbstractListModel>
#include <QCache>
#include <QDir>

class ImagesListModel : public QAbstractListModel {
public:
  ImagesListModel();
  void Init(const QString &folder);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QString GetImagePath(int row) const;

private:
  QStringList image_names_;
  QDir image_folder_;
  mutable QCache<int, QPixmap> pixmap_cache_;
};

#endif // IMAGESLISTMODEL_H
