#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QVideoSink>
void loadImage(const cv::Mat &image);
#include "imageview.h"  
#include <QPixmap>  
#include <QImage>  


//view->loadImage(input_bgr);
#include <QImage>

class ImageView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);
    void loadImage(const QString &path);
    void loadVideo(const QString &path);
    void play();
    void pause();
    void resizeVideoItem();
protected:
    void wheelEvent(QWheelEvent *event) override;

private slots:
    //void handleVideoFrame(const QVideoFrame &frame);

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item = nullptr;
    QMediaPlayer *mediaPlayer = nullptr;
    //QVideoSink *videoSink = nullptr;
    QGraphicsVideoItem *videoItem = nullptr;
signals:
    void mediaInfoAvailable(const QString &info);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

};

#endif // IMAGEVIEW_H
