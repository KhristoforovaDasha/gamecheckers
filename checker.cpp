#include "checker.h"
#include <iostream>
checker::checker():
    QObject()
  , QGraphicsEllipseItem()
  , checkX(0), checkY(0), checkerColor(0), checkerNumber(0)
  , cType(0),oldX(0), oldY(0)
{
    setDefaultColor();
}
checker::checker(int x, int y, int color, const int number) :
    QObject()
  , QGraphicsEllipseItem()
  , checkerColor(color), checkerNumber(number)
//  , cType(NORMAL),oldX(x), oldY(y)
{
    cType = 0;
    oldX = x;
    oldY = y;
    checkX = x;
    checkY = y;
    setDefaultColor();
}
void checker::setPosition(const int x, const int y){
    oldX = checkX;
    oldY = checkY;
    checkX = x;
    checkY = y;
    if(qAbs(oldX - checkX) > 1 && qAbs(oldY - checkY) > 1){
        int color;
        checkerColor == 1 ? color = 0 : color = 1;
        if(checkY > oldY){
            if(checkX > oldX){
                emit removeCheckers(color,checkX - 1,checkY - 1);
            }
            else if (checkX < oldX){
                emit removeCheckers(color,checkX + 1,checkY - 1);
            }
        }
        else{
            if(checkX > oldX){
                emit removeCheckers(color,checkX - 1,checkY + 1);
            }
            else if(checkX < oldX){
                emit removeCheckers(color,checkX + 1,checkY + 1);
            }
        }
    }
}
void checker::setPosition    (const QPair<int,int> pos){
    oldX = checkX;
    oldY = checkY;
    checkX = pos.first;
    checkY = pos.second;
}
void checker:: setDefaultColor(){
    if(cType != 1){
        if(checkerColor == 0){
            setBrush(QBrush(Qt::yellow));
        }
        else setBrush(QBrush(Qt::black));
        setPen(QPen(Qt::green,1.5));
    }
    else{
        if(checkerColor == 0){
            setBrush(QBrush(Qt::green));
        }
        else setBrush(QBrush(Qt::gray));
        setPen(QPen(Qt::green,1.5));
    }
}
void checker:: ChangeColor(){
    if(cType != 1){
        if(checkerColor == 1){
            checkerColor = 0;
            setBrush(QBrush(Qt::yellow));
        }
        else {
            checkerColor = 1;
            setBrush(QBrush(Qt::black));
        }
        setPen(QPen(Qt::green,1.5));
    }
}
void checker::mousePressEvent(QGraphicsSceneMouseEvent *event){
        Q_UNUSED(event);
      if(!selected){
          selected = true;
          std:: cout <<"selected";
          if(checkerColor == 0){
              setBrush(QColor(20,200,200));
          } else if (checkerColor == 1) {
              setBrush(QColor(200,20,200));
          }
          emit findVariant(cType,checkerColor,checkerNumber);
          return;
      }
      else{
          selected = false;
          emit clearVariantes();
      }
}
