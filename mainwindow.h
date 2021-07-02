#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "checker.h"
#include "cell.h"
#include <queue>
#include <QInputDialog>
#include <QPushButton>
#include <QRadioButton>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
struct point{
    int x,y;
};
struct position{
    QVector <QVector <int> > field;
    QVector <point> cnt;
    int black,white;
};

struct usatyk{
    QVector <QVector <int> > field;
    QVector <point> cnt;
};

struct move{
    int x0,y0,x,y;
    move(int x0_ = 0,int y0_ = 0,int x_ = 0,int y_ = 0):
        x0(x0_),y0(y0_),x(x_),y(y_){}
};
struct forbot{
    QVector <QVector <int> > field;
    QVector <struct move> m;
    int bot = 0;
    int chel = 0;
    forbot(QVector <QVector <int> > a_,QVector <struct move> m_,int bot_,int chel_):
        field(a_),m(m_),bot(bot_),chel(chel_){}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void    clearVariantes();
private:
    QPushButton* button;
     QPushButton* button2;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsScene *scene_2;
    QRadioButton* radio1;
    QRadioButton* radio2;
    QWidget *firstPageWidget;
    QWidget *secondPageWidget;
    QWidget *thirdPageWidget;
    QVector <checker*> blackList;
    QVector <checker*> whiteList;
    QVector <QVector<cell*> > gameField;
    //QVector <QVector <int> > field;
    bool goWhite = true;
    bool goBlack = true;
    int black = 12;
    int white = 12;
    void View();
    void CreateHelp();
    void createGameField();
    void createCheckerList();
    void createConnections(int f = 0);
    void createScene();
    QVector <point> Alekseybot_two(QVector <QVector <int> > field,int color);
    int Usatykbot(int color,QVector <QVector <int> > field,int depth,int alpha,int beta);
     void possibleEat(int depth,QVector <QVector <int> > field,int x,int y,QVector <point> cnt,int black,int white);
     void possibleEatKing(int depth,QVector <QVector <int> > field,int x,int y,QVector <point> cnt,int black,int white);
    int getFunction(int color,QVector <QVector <int> > field);
    checker *makeChecker (const int j, const int i,const int color, const int number,const int direction);
    void    findVariantes (const int number,  QVector<checker*>& list);
    void    findKingVariantes(const int number,  QVector<checker*>& list);
    bool    selectCellUp (const int x, const int y,int flag = 0);
    bool    selectCellDown (const int x, const int y,int flag = 0);
    bool moveToCell(const int x,const int y,QVector<checker*>& list);
    bool iseat(int depth,int b,int w,QVector <struct move> m,int k1,int k2,int f,int x,int y,QVector <QVector <int> > field,std::vector <forbot>& q);
     bool isKingeat(int depth,int color,int w,int b,int dx,int dy,int x,int y,QVector <QVector <int> > field,std::vector <forbot>& q,QVector < struct move> m);
     bool isKingeat2(int depth,int&b,int& w,int color,int& x0,int& y0,int dx,int dy,int& x,int& y,QVector <QVector <int> >& field,QVector < struct move>& cnt);
     void Kingstep(int color,int b,int w,int depth,int x,int y,int dx,int dy,QVector <QVector <int> > field,std::vector <forbot>& q,QVector < struct move> m);
     void    removeChecker (const int x,const int y, QVector<checker*>& list);
     bool isValid(int x,int y);
     void botstep(int type);
     void findmore(int x,int y, QVector<checker*>& list);
     void botgoing(QVector<checker*>& list,int k);
     bool  BotFind(QVector<checker*>& list,checker* c,int x,int y,int k);
     void botAleksey(int k);
     QPair<int,QPair<int,int> > botAlekseygoing(int x,int y,int k1,int k2,int k3,int k4,QVector <QVector <int> > field,int depth = 0);
     QVector <struct move> botAleksey2(QVector <QVector <int> > field);
     //void on_comboBox_currentIndexChanged(int index);
private slots:
    void    slotFindVariantes (const int type, const int color, const int number);
    //void    slotClearVariantes ();
    void slotMoveToCell(const int x,const int y);
    void    slotRemoveChecker(const int color, const int x, const int y);
    void    slotClearVariantes ();
    void on_comboBox_currentIndexChanged(int index);
    void on_comboBox_2_currentIndexChanged(int index);
    void on_pushButton_pressed();

    void on_button_pressed();
    void on_button2_pressed();
public slots:
        void StartOver(int k = 0);
};
#endif // MAINWINDOW_H
/*if(isValid(i + 2*k1,j - 2) && field[i + k1][j - 1] != k1 && field[i + k1][j - 1] != k2
        && field[i + 2*k1][j - 2] == 0 && field[i + k1][j - 1]){
    int d = field[i + k1][j - 1];
    field[i][j] = 0,field[i + 2*k1][j - 2] = k1,field[i + k1][j - 1] = 0;
   if(depth == 0) q.push({field,{i,j,i + 2*k1,j - 2}});
   else q.push({field,m});
   f1 = 1;
    field[i][j] = k1,field[i + 2*k1][j - 2] =0,field[i + k1][j - 1] = d;
}
if(isValid(i + 2*k1,j + 2) && field[i + k1][j + 1] != k1 && field[i + k1][j + 1] != k2
        && field[i + 2*k1][j + 2] == 0 && field[i + k1][j + 1]){
    int d = field[i + k1][j + 1];
    field[i][j] = 0,field[i + 2*k1][j + 2] = k1,field[i + k1][j + 1] = 0;
   if(depth == 0) q.push({field,{i,j,i + 2*k1,j + 2}});
   else q.push({field,m});
   f2 = 1;
    field[i][j] = k1,field[i + 2*k1][j + 2] =0,field[i + k1][j + 1] = d;
}*/
