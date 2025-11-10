#include "imagelistview.h"

#include <QScrollBar>

#include "asyncimageloader.h"
#include "imageslistmodel.h"
constexpr int DELAY_MS = 150;

ImageListView::ImageListView(QWidget *parent) : QListView(parent) {
  m_reloadTimer.setSingleShot(true);
  connect(&m_reloadTimer, &QTimer::timeout, this,
          &ImageListView::loadVisibleItem);
}

void ImageListView::resizeEvent(QResizeEvent *event) {
  m_reloadTimer.start(DELAY_MS);
  QListView::resizeEvent(event);
}

void ImageListView::setModel(ImagesListModel *model) {
  if (m_imgModel != model) {
    connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this,
            [&]() { m_reloadTimer.start(DELAY_MS); });
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this,
            [&]() { m_reloadTimer.start(DELAY_MS); });
  }
  QListView::setModel(m_imgModel = model);
}

void ImageListView::loadVisibleItem() {
  const QRect vr = viewport()->rect();
  QModelIndex idx = this->indexAt(QPoint{1, 1});
  std::vector<int> visibleRows;

  while (idx.isValid()) {
    const int row = idx.row();
    if (m_imgModel->needToBeLoaded(row) && (visualRect(idx) & vr).isValid()) {
      visibleRows.push_back(row);
    }
    idx = idx.sibling(row + 1, 0);
  }

  if (!visibleRows.empty()) {
    m_imgModel->loader()->enqueue(std::move(visibleRows));
  }
}
