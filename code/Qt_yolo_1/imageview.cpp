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

    //videoSink = new QVideoSink(this);
   // mediaPlayer->setVideoSink(videoSink);

    //connect(videoSink, &QVideoSink::videoFrameChanged, this, &ImageView::handleVideoFrame);
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
        resetTransform();
        QFileInfo info(path);
        QString fileName = info.fileName();
        QString ext = info.suffix();
        QString infoText = QString("圖片: %1, 副檔名: %2, 解析度: %3x%4")
                               .arg(fileName).arg(ext).arg(pixmap.width()).arg(pixmap.height());

        emit mediaInfoAvailable(infoText);

    }
}


void ImageView::loadVideo(const QString &path) {
    scene->clear();
    item = nullptr;
    videoItem = new QGraphicsVideoItem;
    scene->addItem(videoItem);
    resizeVideoItem();  // 安全執行（下一步優化）

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



/*void ImageView::handleVideoFrame(const QVideoFrame &frame) {
    if (!frame.isValid()) return;

    QImage image = frame.toImage();

    if (image.isNull()) {
        qDebug() << "[ImageView] QVideoFrame.toImage() failed. Format:" << frame.pixelFormat();
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(image);

    if (!item) {
        item = scene->addPixmap(pixmap);
    } else {
        item->setPixmap(pixmap);
    }

    scene->setSceneRect(pixmap.rect());
    update();  // ← 強制更新 View
}*/



void ImageView::wheelEvent(QWheelEvent *event) {
    const double scaleFactor = 1.15;
    if (event->angleDelta().y() > 0)
        scale(scaleFactor, scaleFactor);
    else
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
}

void ImageView::resizeVideoItem() {
    if (!videoItem)
        return;

    QSizeF viewSize = size();
    QSizeF videoSize = videoItem->nativeSize();

    if (videoSize.isEmpty()) {
        qDebug() << "[ImageView] nativeSize() is empty.";
        return;
    }

    qreal scale = qMin(viewSize.width() / videoSize.width(),
                       viewSize.height() / videoSize.height());

    videoItem->setTransform(QTransform::fromScale(scale, scale));
    videoItem->setPos((viewSize.width() - videoSize.width() * scale) / 2,
                      (viewSize.height() - videoSize.height() * scale) / 2);
}


