#ifndef CELL_H
#define CELL_H
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QMessageBox>

class cell :public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit cell(const int x, const int y);
    cell();
    void setNumber  (const int number)  {checkerNumber = number;}
    void setColor   (const int color)   {checkerColor = color;}
    void makeEmpty  (bool e)            {empty = e;}
    int X() {return cellX;};
    int Y() {return cellY;};
    int color() {return checkerColor;};
    bool isempty() {return empty;};
    void makeSelect(bool e) {selected = e;};
    void resetSettings();
    void setDefaultColor();
    int number() {return checkerNumber;};
    bool isSelecte(){return selected;};
    int Type() {return cType;};
private:
    int     cellX = 0;
    int     cellY = 0;
    bool    empty    = true;
    bool    selected = false;
    int     checkerNumber;
    int     checkerColor;
    int     cType = 0;
    void makecell();
signals:
    void moveToThis(const int x,const int y);
    void clearVariantes();
    void botstep();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CELL_H
