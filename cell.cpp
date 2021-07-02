#include "cell.h"

cell::cell():
    QObject()
  , QGraphicsRectItem()
  , cellX(0), cellY(0), empty(true), selected(false)
{
     makecell();
}
cell::cell(const int x, const int y) :
    QObject()
  , QGraphicsRectItem()
  ,cellX(x),cellY(y)
{
    makecell();
}

void cell:: mousePressEvent(QGraphicsSceneMouseEvent *event){
       Q_UNUSED(event);
    if(selected){
        emit moveToThis(cellX,cellY);
        //emit botstep();
    }
    else{
        emit clearVariantes();
    }
}
void cell::resetSettings()
{
    checkerColor    = -1;
    checkerNumber   = -1;
    empty           = true;
    selected        = false;
}
void cell:: makecell(){
    if((cellX + cellY)%2){
        checkerNumber = -1;
        checkerColor = -1;
    }
    else{
        checkerNumber = -32768;
        checkerColor = -32768;
    }
    setDefaultColor();
}
void cell:: setDefaultColor(){
    if((cellX + cellY)%2){
        setBrush(QBrush(QColor("brown")));
    }
    else setBrush(QBrush(Qt::white));
}
