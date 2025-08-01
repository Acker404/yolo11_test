#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QPixmap>
#include <QImage>

// OpenCV / DNN / Inference
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

namespace cv {
    class Mat;
}

class ImageView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);
    void loadImage(const QString &path);
    void loadImage(const cv::Mat &image);
    void loadVideo(const QString &path);
    void play();
    void pause();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item = nullptr;
    QMediaPlayer *mediaPlayer = nullptr;
    QGraphicsVideoItem *videoItem = nullptr;

signals:
    void mediaInfoAvailable(const QString &info);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

};

#endif // IMAGEVIEW_H
