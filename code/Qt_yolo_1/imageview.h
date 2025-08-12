#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QImage>

// OpenCV / DNN / Inference
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/dnn.hpp>

namespace cv {
    class Mat;
}

class ImageView : public QGraphicsView {
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);
    void loadImage(const cv::Mat &image);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsScene *scene;
    QGraphicsPixmapItem *item = nullptr;
};

#endif // IMAGEVIEW_H