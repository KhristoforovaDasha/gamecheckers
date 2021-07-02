#ifndef CHECKER_H
#define CHECKER_H
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include <QPair>
#include <QObject>

class checker:public QObject, public QGraphicsEllipseItem
{
     Q_OBJECT
public:
    explicit checker(int x, int y, int color, const int number);
    checker();
     void setDirection   (const int direction)   {checkerDirection = direction;};
     int number() {return checkerNumber;};
     int color(){return checkerColor;};
     int X() {return checkX;};
     int Y() {return checkY;};
     int Direct() {return checkerDirection;};
     void makeSelect(bool e) {selected = e;};
     bool isSelect() {return selected;};
     void makeKing() {
         cType = 1;
         //int k = 0;
     };
     void Delete() {deleted = 1;};
     bool isdelete() {return deleted;};
     bool Type(){return cType;};
     void setPosition    (const QPair<int,int> pos);
     void setPosition(const int x, const int y);
     void setDefaultColor();
     void ChangeColor();
signals:
    void findVariant(const int type, const int color, const int number);
    void clearVariantes();
    void removeCheckers(const int color, const int x, const int y);
private:
    int cType = 0;
    int checkX;
    int checkY;
    int oldX;
    int oldY;
    bool removed;
    bool selected = false;
    bool deleted = 0;
    int     checkerNumber;
    int     checkerColor;
    int     checkerDirection;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CHECKER_H
