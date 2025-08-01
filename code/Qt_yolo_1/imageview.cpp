#include "imageview.h"
#include <QWheelEvent>
#include <QPixmap>
#include <QPainter>

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this)) {

    setScene(scene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

void ImageView::loadImage(const cv::Mat &image) {
    scene->clear();
    item = nullptr;

    QImage qimg(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(qimg);

    if (!pixmap.isNull()) {
        item = scene->addPixmap(pixmap);
        scene->setSceneRect(pixmap.rect());
        fitInView(item, Qt::KeepAspectRatio);
    }
}

void ImageView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    if (item) {
        fitInView(item, Qt::KeepAspectRatio);
    }
}
