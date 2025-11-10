#ifndef IMAGELISTVIEW_H
#define IMAGELISTVIEW_H

#include <QListView>
#include <QTimer>
class ImagesListModel;

class ImageListView : public QListView {
  Q_OBJECT
 public:
  ImageListView(QWidget *parent = nullptr);
  virtual void resizeEvent(QResizeEvent *event) override final;
  void setModel(ImagesListModel *model);

 public slots:
  void loadVisibleItem();

 private:
  ImagesListModel *m_imgModel{nullptr};
  QTimer m_reloadTimer;
};

#endif  // IMAGELISTVIEW_H
