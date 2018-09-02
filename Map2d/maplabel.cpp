#include "maplabel.h"
#include <QMetaType>

MapLabel::MapLabel(QWidget* parent):
    QLabel(parent)
{
    clickedPoint=cv::Point2f(0,0);
    qRegisterMetaType<cv::Point2f>("cv::Point2f");
    qRegisterMetaType<cv::Point2f>("cv::Point2f&");
}

void MapLabel::mouseReleaseEvent(QMouseEvent* ev){
    int button=ev->button();
    if(button==Qt::RightButton){
        if(!rightClicked){
            setCursor(Qt::PointingHandCursor);
        }
        else{
            setCursor(Qt::ArrowCursor);
        }
        rightClicked=!rightClicked;
        leftClicked=false;
    }
    else if(button==Qt::LeftButton){
        if(rightClicked && !leftClicked){
            clickedPoint.x=ev->x();
            clickedPoint.y=ev->y();
            emit publishClickedPoint(clickedPoint);
            leftClicked=true;
            rightClicked=false;
            setCursor(Qt::ArrowCursor);
        }
    }
}
