#ifndef MAPLABEL_H
#define MAPLABEL_H

#include <QLabel>
#include <QObject>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

class MapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MapLabel(QWidget* parent= 0);
    ~MapLabel(){}

    inline cv::Point2f getClickedPoint(){
        return clickedPoint;
    }
protected:
    virtual void mouseReleaseEvent(QMouseEvent* ev);

signals:
    void publishClickedPoint(cv::Point2f&);

private:
    bool rightClicked = {false};
    bool leftClicked= {false};
    cv::Point2f clickedPoint;
};

#endif // MAPLABEL_H
