#include "imageview.h"
#include <QWheelEvent>
#include <QPixmap>
#include <QPainter>
#include <QVideoFrame>
#include <QVideoFrameFormat>
#include <QFileDialog>
#include <QGraphicsVideoItem>

ImageView::ImageView(QWidget *parent)
    : QGraphicsView(parent),
    scene(new QGraphicsScene(this)) {

    setScene(scene);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    mediaPlayer = new QMediaPlayer(this);
    connect(mediaPlayer, &QMediaPlayer::metaDataChanged, this, [=]() {
        auto file = mediaPlayer->source().toLocalFile();
        QString name = QFileInfo(file).fileName();
        QSize size = videoItem ? videoItem->nativeSize().toSize() : QSize(0, 0);
        QString ext = QFileInfo(file).suffix();
        QString info = QString("檔名: %1, 副檔名: %2, 解析度: %3x%4")
                           .arg(name).arg(ext).arg(size.width()).arg(size.height());

        emit mediaInfoAvailable(info);
    });

    connect(mediaPlayer, &QMediaPlayer::durationChanged,
            this, &ImageView::durationChanged);
    connect(mediaPlayer, &QMediaPlayer::positionChanged,
            this, &ImageView::positionChanged);
}

void ImageView::loadImage(const cv::Mat &image) {
    if (mediaPlayer && mediaPlayer->playbackState() != QMediaPlayer::StoppedState)
        mediaPlayer->stop();

    scene->clear();
    item = nullptr;
    videoItem = nullptr;

    QImage qimg(image.data, image.cols, image.rows, image.step, QImage::Format_BGR888);
    QPixmap pixmap = QPixmap::fromImage(qimg);

    if (!pixmap.isNull()) {
        item = scene->addPixmap(pixmap);
        scene->setSceneRect(pixmap.rect());
        fitInView(item, Qt::KeepAspectRatio);
    }
}

void ImageView::loadImage(const QString &path) {
    if (mediaPlayer && mediaPlayer->playbackState() != QMediaPlayer::StoppedState)
        mediaPlayer->stop();

    scene->clear();
    item = nullptr;
    videoItem = nullptr;

    QPixmap pixmap(path);
    if (!pixmap.isNull()) {
        item = scene->addPixmap(pixmap);
        scene->setSceneRect(pixmap.rect());
        fitInView(item, Qt::KeepAspectRatio);

        QFileInfo info(path);
        QString fileName = info.fileName();
        QString ext = info.suffix();
        qint64 fileSize = info.size();
        QString infoText = QString("圖片: %1, 副檔名: %2, 解析度: %3x%4, 檔案大小: %5 KB")
                               .arg(fileName).arg(ext).arg(pixmap.width()).arg(pixmap.height()).arg(fileSize / 1024);

        emit mediaInfoAvailable(infoText);
    }
}


void ImageView::loadVideo(const QString &path) {
    scene->clear();
    item = nullptr;
    videoItem = new QGraphicsVideoItem;
    scene->addItem(videoItem);
    connect(videoItem, &QGraphicsVideoItem::nativeSizeChanged, this, [=](const QSizeF &size) {
        fitInView(videoItem, Qt::KeepAspectRatio);
    });

    mediaPlayer->setVideoOutput(videoItem);
    mediaPlayer->setSource(QUrl::fromLocalFile(path));
    mediaPlayer->play();
}

void ImageView::play() {
    if (mediaPlayer)
        mediaPlayer->play();
}

void ImageView::pause() {
    if (mediaPlayer)
        mediaPlayer->pause();
}

void ImageView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void ImageView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    if (item) {
        fitInView(item, Qt::KeepAspectRatio);
    } else if (videoItem) {
        fitInView(videoItem, Qt::KeepAspectRatio);
    }
}