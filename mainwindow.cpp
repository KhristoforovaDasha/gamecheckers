#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <queue>
#include <QMessageBox>
#include <QStackedLayout>
#include <QWidget>
#include <QLabel>
int ind = 0;
int ind2 = 0;
int Mans = 0;
int INF_MIN = -1e9,INF_MAX = 1e9;
int WHITE = -1,BLACK = 1;
bool fiseat = 0;
QVector <usatyk> step;
QVector <point> res,blackwhite;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CreateHelp();
    View();
    createScene();
    createGameField();
    createCheckerList();
    createConnections();
}
void MainWindow:: CreateHelp(){
    firstPageWidget = new QWidget;
    secondPageWidget = new QWidget;
    thirdPageWidget = new QWidget;

    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(firstPageWidget);
    stackedLayout->addWidget(secondPageWidget);
    stackedLayout->addWidget(thirdPageWidget);
    connect(ui->comboBox_3, QOverload<int>::of(&QComboBox::activated),
                stackedLayout, &QStackedLayout::setCurrentIndex);
    QGraphicsScene* scene_3 = new QGraphicsScene(secondPageWidget);
    QGraphicsView* graphicsView_3 = new QGraphicsView(secondPageWidget);
    scene_3->addPixmap(QPixmap("://images/rules.png"));
    graphicsView_3->setScene(scene_3);
    graphicsView_3->show();
    thirdPageWidget->resize(200,200);
    QLabel* label = new QLabel(thirdPageWidget);
    label->setText(QString("What color pieces are given up?"));
    radio1 = new QRadioButton(thirdPageWidget);
    radio2 = new QRadioButton(thirdPageWidget);
    button = new QPushButton(thirdPageWidget);
    button->setText(QString("OK"));
    button2 = new QPushButton(thirdPageWidget);
    button2->setText(QString("Cancel"));
    radio1->setText(QString("White"));
    radio2->setText(QString("Black"));
     radio1->move(30,30);
     radio2->move(30,60);
     button->move(90,90);
     button2->move(10,90);
     firstPageWidget->close();
}
void MainWindow:: View(){
    scene_2 = new QGraphicsScene(this);
    ui->graphicsView_2->resize(150,150);
    scene_2->addPixmap(QPixmap("://images/beginner.png"));
    ui->graphicsView_2->setScene(scene_2);
    ui->graphicsView_2->show();
    /*auto z = new QGraphicsPixmapItem;
    z->setPixmap(QPixmap("‪D:\\GuJCav3Mlek.jpg"));
    z->setPos(5, 5);
    z->setScale(0.02);
     scene_2->addItem(z);*/
//     scene_2->addRect(0, 0, 10, 10);
    /*ui->graphicsView_2->setBackgroundBrush(QImage("D:\beginner.png"));
    ui->graphicsView_2->setCacheMode(QGraphicsView::CacheBackground);*/
}
void MainWindow::on_comboBox_currentIndexChanged(int index){
    ind = index;
    for(checker* c : whiteList){
        c->ChangeColor();
    }
    for(checker* c : blackList){
        c->ChangeColor();
    }
    goBlack = true;
    goWhite = false;
    botstep(1);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow:: createScene(){
    scene = new QGraphicsScene(this);
    ui->graphicsView->resize(500,500);
    ui->graphicsView->setScene(scene);
}
void MainWindow::createGameField(){
    for(int i = 0; i < 8; ++i){
        QVector<cell*> v;
        for(int j = 0; j < 8; ++j){
            cell* c = new cell(j,i);
            c->setRect(i*50,j*50,50,50);
            v.append(c);
        }
        gameField.append(v);
    }
    for(QVector<cell*> v : gameField){
        for(cell* c : v){
            scene->addItem(c);
        }
     }
}
checker* MainWindow::makeChecker (const int j, const int i,const int color, const int number,const int direction){
    checker *check = new checker(j,i,color,number);
    check->setRect(
                        gameField[j][i]->rect().x()        +10,
                        gameField[j][i]->rect().y()        +10,
                        gameField[j][i]->rect().width()    -20,
                        gameField[j][i]->rect().height()   -20
                );
    check->setDirection(direction);
    gameField[j][i]->makeEmpty(false);
    gameField[j][i]->setNumber(check->number());
    gameField[j][i]->setColor(check->color());
    return check;
}
void MainWindow:: createCheckerList(){
    int k = 0;
    for(int i = 0; i < 3; ++i){
        for(int j = 0; j < 8; ++j){
            if((i + j)%2){
                blackList.append(makeChecker(j,i,1,k,1));
                scene->addItem(blackList[k]);
                k++;
            }
        }
    }
    k = 0;
    for(int i = 7; i > 4; --i){
        for(int j = 0; j < 8; ++j){
            if((i + j)%2){
                whiteList.append(makeChecker(j,i,0,k,0));
                scene->addItem(whiteList[k]);
                k++;
            }
        }
    }
}
void MainWindow:: createConnections(int f){
    QObject::connect(button,SIGNAL(clicked()),this,SLOT(on_button_pressed()));
    for(checker *check : blackList){
        QObject::connect(check,SIGNAL(findVariant(int,int,int)),
                this,SLOT(slotFindVariantes(int,int,int)));

       QObject:: connect(check,SIGNAL(clearVariantes()),
                         this,SLOT(slotClearVariantes()));

        QObject::connect(check,SIGNAL(removeCheckers(int,int,int)),
                this,SLOT(slotRemoveChecker(int,int,int)));

    }
    for(checker *check : whiteList){
       QObject:: connect(check,SIGNAL(findVariant(int,int,int)),
                this,SLOT(slotFindVariantes(int,int,int)));

        QObject::connect(check,SIGNAL(clearVariantes()),
                this,SLOT(slotClearVariantes()));

        QObject::connect(check,SIGNAL(removeCheckers(int,int,int)),
                this,SLOT(slotRemoveChecker(int,int,int)));

    }
    if(f == 0){
    for(QVector<cell*> v : gameField){
        for(cell* c : v){
            QObject:: connect(c,SIGNAL(moveToThis(int,int)),this,SLOT(slotMoveToCell(int,int)));
            QObject:: connect(c,SIGNAL(botstep(int,int)),this,SLOT(slotBotStep(int,int)));
        }
    }
    }
}
void MainWindow:: slotFindVariantes (const int type, const int color, const int number){
  if(type == 0){
    if(color == 0 && goWhite == true){
        if(ind == 0) findVariantes(number,whiteList);
        else findVariantes(number,blackList);
    }
    else if(color == 1 && goBlack == true){
        if(ind == 0) findVariantes(number,blackList);
        else findVariantes(number,whiteList);
    }
   }
  else{
      if(color == 0 && goWhite == true){
          findKingVariantes(number,whiteList);
      }
      else if(color == 1 && goBlack == true){
          findKingVariantes(number,blackList);
      }
  }
}
void MainWindow:: findVariantes (const int number,  QVector<checker*>& list){
    int x = list[number]->X();
    int y = list[number]->Y();
    int direct = list[number]->Direct();
    bool a = selectCellUp(x,y);
    bool b = selectCellDown(x,y);
    if(direct == 1){
        if(!(b || a)){
            if(y + 1 < 8 && x + 1 < 8 && gameField[x + 1][y + 1]->isempty()) {
                gameField[x + 1][y + 1]->setBrush(QBrush(Qt:: green));
                gameField[x + 1][y + 1]->makeSelect(true);
            }
            if(y + 1 < 8 && x - 1 >= 0 && gameField[x - 1][y + 1]->isempty()) {
                gameField[x - 1][y + 1]->setBrush(QBrush(Qt:: green));
                gameField[x - 1][y + 1]->makeSelect(true);
            }
        }
    }
    else{
        if(!(b || a)){
            if(y - 1 >= 0 && x + 1 < 8 && gameField[x + 1][y - 1]->isempty()) {
                gameField[x + 1][y - 1]->setBrush(QBrush(Qt:: green));
                gameField[x + 1][y - 1]->makeSelect(true);
            }
            if(y -1 >= 0 && x - 1 >= 0 && gameField[x - 1][y - 1]->isempty()) {
                gameField[x - 1][y - 1]->setBrush(QBrush(Qt:: green));
                gameField[x - 1][y - 1]->makeSelect(true);
            }
        }
    }
}
void  MainWindow:: findKingVariantes(const int number,  QVector<checker*>& list){
    //std::ofstream file;
    //file.open("D:\\tree.txt",std::ios_base::app);
    int x = list[number]->X();
    int y = list[number]->Y();
    int color = list[number]->color();
    while (isValid(++x,++y) && gameField[x][y]->color() != color ) {
        if(!gameField[x][y]->isempty()) {
            selectCellUp(x - 1,y - 1,1);
            if(!isValid(++x,++y) || !gameField[x][y]->isempty()) break;
        }
        gameField[x][y]->setBrush(Qt::green);
        gameField[x][y]->makeSelect(true);
        //file << x <<" " << y <<'\n';
    }
        //file <<" end" <<" " << x <<" " << y <<'\n';
    //if(isValid(x,y) && !gameField[x][y]->isempty()) selectCellUp(--x,--y,1);
    x = list[number]->X();
    y = list[number]->Y();
    while (isValid(--x,++y) && gameField[x][y]->color() != color) {
        if(!gameField[x][y]->isempty()) {
            selectCellUp(x + 1,y - 1,2);
            if(!isValid(--x,++y) || !gameField[x][y]->isempty()) break;
        }
        gameField[x][y]->setBrush(Qt::green);
        gameField[x][y]->makeSelect(true);
       // file << x <<" " << y <<'\n';
    }
    //file <<" end" <<" " << x <<" " << y <<'\n';
    //if(isValid(x,y) && !gameField[x][y]->isempty()) selectCellUp(++x,--y,2);
    x = list[number]->X();
    y = list[number]->Y();
    while (isValid(--x,--y) && gameField[x][y]->color() != color ) {
        if(!gameField[x][y]->isempty()) {
            selectCellDown(x + 1,y + 1,1);
            if(!isValid(--x,--y) || !gameField[x][y]->isempty()) break;
        }
        gameField[x][y]->setBrush(Qt::green);
        gameField[x][y]->makeSelect(true);
        //file << x <<" " << y <<'\n';
    }
    //file <<" end" <<" " << x <<" " << y<<'\n';
    //if(isValid(x,y) && !gameField[x][y]->isempty()) selectCellDown(++x,++y,1);
    x = list[number]->X();
    y = list[number]->Y();
    while (isValid(++x,--y) && gameField[x][y]->color() != color) {
        if(!gameField[x][y]->isempty()) {
            selectCellDown(x - 1,y + 1,2);
            if(!isValid(++x,--y) || !gameField[x][y]->isempty()) break;
        }
        gameField[x][y]->setBrush(Qt::green);
        gameField[x][y]->makeSelect(true);
        //file << x <<" " << y <<'\n';
    }
    //file <<" end" <<" " << x <<" " << y <<'\n';
    //if(isValid(x,y) && !gameField[x][y]->isempty()) selectCellDown(--x,++y,2);
    //file <<'\n' <<'\n';
    //file.close();
}
bool MainWindow:: isValid(int x,int y){
    return x >= 0 && y >= 0 && y < 8 && x < 8;
}
bool  MainWindow:: selectCellUp (const int x, const int y,int flag){
    bool res = 0;
    if(y + 1 < 8 ){
        if(x + 1 < 8 && flag != 2){
            /*if(flag == 1){
                std::ofstream file;
                file.open("D:\\tree.txt",std::ios_base::app);
                file <<"up" <<" " << flag <<" " <<  !gameField[std::max(x - 1,0)][std::max(y - 1,0)]->isempty() <<" " << gameField[x - 1][y - 1]->color();
            }*/
            if(!gameField[x + 1][y + 1]->isempty() && gameField[x + 1][y + 1]->color() != gameField[x][y]->color()){
                if((x + 2) < 8 && (y + 2 )< 8 && gameField[x + 2][y + 2]->isempty()) {
                    gameField[x + 1][y + 1]->setBrush(QBrush(Qt::red));
                    gameField[x + 2][y + 2]->setBrush(QBrush(Qt::green));
                    gameField[x + 2][y + 2]->makeSelect(true);
                    res = 1;
                }
            }
        }
        if(x - 1 >= 0 && flag != 1){
            /*if(flag == 2){
                std::ofstream file;
                file.open("D:\\tree.txt",std::ios_base::app);
                file <<"up" <<" " << flag <<" " <<  !gameField[x - 1][y - 1]->isempty() <<" " << gameField[x - 1][y - 1]->color() <<" " <<gameField[x][y]->color();
                 file <<" " << (x - 2) <<" " << y - 2 << " " << gameField[x - 2][y - 2]->isempty() <<'\n';
                 file.close();
            }*/
            if(!gameField[x - 1][y + 1]->isempty() && gameField[x - 1][y + 1]->color() != gameField[x][y]->color()){
                if((x - 2) >= 0 && (y + 2 )< 8 && gameField[x - 2][y + 2]->isempty()) {
                    gameField[x - 1][y + 1]->setBrush(QBrush(Qt::red));
                    gameField[x - 2][y + 2]->setBrush(QBrush(Qt::green));
                    gameField[x - 2][y + 2]->makeSelect(true);
                    res = 1;
                }
            }
        }
    }
    return res;
}
bool  MainWindow:: selectCellDown (const int x, const int y,int flag){
    bool res = 0;
    if(y - 1 >= 0 ){
        if(x + 1 < 8 && flag != 1){
            /*if(flag == 1){
                std::ofstream file;
                file.open("D:\\tree.txt",std::ios_base::app);
                file <<"down" <<" " << flag <<" " << !gameField[x - 1][y - 1]->isempty() <<" " << gameField[x - 1][y - 1]->color() <<" " <<gameField[x][y]->color();
                 file <<" " << (x - 2) <<" " << y - 2 << " " << gameField[x - 2][y - 2]->isempty() <<'\n';
                 file.close();
            }*/
            if(!gameField[x + 1][y - 1]->isempty() && gameField[x + 1][y - 1]->color() != gameField[x][y]->color()){
                if((x + 2) < 8 && (y - 2 ) >= 0 && gameField[x + 2][y - 2]->isempty()) {
                    gameField[x + 1][y - 1]->setBrush(QBrush(Qt::red));
                    gameField[x + 2][y - 2]->setBrush(QBrush(Qt::green));
                    gameField[x + 2][y - 2]->makeSelect(true);
                    res = 1;
                }
            }
        }
        if(x - 1 >= 0 && flag != 2){
            if(!gameField[x - 1][y - 1]->isempty() && gameField[x - 1][y - 1]->color() != gameField[x][y]->color()){
                if((x - 2) >= 0 && (y - 2 ) >= 0 && gameField[x - 2][y - 2]->isempty()) {
                    gameField[x - 1][y - 1]->setBrush(QBrush(Qt::red));
                    gameField[x - 2][y - 2]->setBrush(QBrush(Qt::green));
                    gameField[x - 2][y - 2]->makeSelect(true);
                    res = 1;
                }
            }
        }
    }
    return res;
}
bool MainWindow:: moveToCell(const int x,const int y,QVector<checker*>& list){
    for(checker* check : list){
        if(check->isSelect()){
           /* std::ofstream file;
            file.open("D:\\tree.txt",std::ios_base::app);
            if(check->color() == 1) file <<1 <<" " <<check->number() <<" " <<  x <<" " << y <<'\n';
            else file <<2 <<" " <<check->number() <<" " <<  x <<" " << y <<'\n';
            file.close();*/
            int flag = 0;
            if(check->Direct() == 1 && x == 7){
                check->makeKing();
            }
            if(check->Direct() == 0 && x == 0){
                check->makeKing();
            }
            gameField[check->X()][check->Y()]->resetSettings();
            check->setRect(gameField[y ][x]->rect().x()        +10,
                             gameField[y][x]->rect().y()        +10,
                             gameField[y][x]->rect().width()    -20,
                             gameField[y][x]->rect().height()   -20);
            if(qAbs(check->X() - y) > 1 && qAbs(check->Y() - x) > 1){
                    flag = 1;
                    int xx = x,yy = y;
                    if(check->X() > yy){
                        if(check->Y() > xx){
                            while(isValid(xx,yy) && gameField[yy][xx]->isempty()) yy++,xx++;
                        }
                        else{
                            while(isValid(xx,yy) && gameField[yy][xx]->isempty()) yy++,xx--;
                        }
                    }
                    else{
                        if(check->Y() > xx){
                            while(isValid(xx,yy) && gameField[yy][xx]->isempty()) yy--,xx++;
                        }
                        else{
                            while(isValid(xx,yy) && gameField[yy][xx]->isempty()) yy--,xx--;
                        }
                    }
                    if(isValid(xx,yy) && !gameField[yy][xx]->isempty()){
                   if(ind == 0) {
                       if(gameField[yy][xx]->color() == 0) removeChecker(yy,xx,whiteList);
                    else removeChecker(yy,xx,blackList);
                   }
                   else{
                       if(gameField[yy][xx]->color() == 0) removeChecker(yy,xx,blackList);
                    else removeChecker(yy,xx,whiteList);
                   }
                    }
            }
            check->setPosition({y,x});
            check->makeSelect(false);
            gameField[y][x]->makeEmpty(false);
            gameField[y][x]->setColor(check->color());
            gameField[y][x]->setNumber(check->number());
            for(QVector<cell*> v : gameField){
                for(cell *c : v){
                    c->setDefaultColor();
                    c->makeSelect(false);
                }
            }
            if(flag && (selectCellDown(y,x) || selectCellUp(y,x))) {
                check->makeSelect(true);
                return false;
            }
            else{
            //emit clearVariantes();
            for (checker *c : blackList) {
                c->setDefaultColor();
            }
            for (checker *c : whiteList) {
                c->setDefaultColor();

            }
            return true;
            }
        }
    }
    //emit clearVariantes();
    return false;
}
void MainWindow:: slotMoveToCell(const int x,const int y){
    if(ind2 != 3){
       //if(ind == 0){
        if(moveToCell(x,y,whiteList)){
        if(goBlack) {
           botstep(1);
        }
        }
       //}
       /*else {
           if(moveToCell(x,y,blackList)){
           if(goWhite) {
              botstep(1);
           }
           }
       }*/
        goBlack = 1;
    }
    else{
        if(goWhite) moveToCell(x,y,whiteList),goBlack = true,goWhite = false;
        else moveToCell(x,y,blackList),goBlack = false,goWhite = true;
    }
    /*if(moveToCell(x,y,whiteList) && goBlack) {
            botstep(1);
            goBlack = true;
    }*/
    /*else if(moveToCell(x,y,blackList)){
          goBlack = false;
          goWhite = true;
    }*/

}
void MainWindow:: findmore(int x,int y,QVector<checker*>& list){
    int k = moveToCell(x,y,list);
    while(!k) {
        bool f = 0;
        for(QVector<cell*> v : gameField){
            for(cell *c : v){
                if(c->isSelecte()){
                    k = moveToCell(c->X(),c->Y(),list);
                    f = 1;
                    std::ofstream file;
                    file.open("D:\\tree.txt",std::ios_base::app);
                    file << c->X() << " " << c->Y() <<'\n';
                    file.close();
                    break;
                }
            }
            if(f) break;
        }
        if(f == 0) break;
    }
}
void MainWindow:: botgoing(QVector<checker*>& list,int k){
    for(checker* c : list){
        int x = c->X(),y = c->Y();
        if(!c->isdelete()){
            if(!BotFind(list,c,x,y,k)){
                if(c->Type() == 1){
                    if(BotFind(list,c,x,y,-k)){
                        break;
                    }
                }
            }
            else break;
        }
    }
}
bool MainWindow:: BotFind(QVector<checker*>& list,checker* c,int x,int y,int k){
    if(isValid(y + k*2,x + 2)  && gameField[x + 2][y + 2*k]->isempty() &&
            !gameField[x + 1][y + k]->isempty() && gameField[x + 1][y + k]->color() != gameField[x][y]->color()){
        c->makeSelect(true);
        findmore(y + 2*k,x + 2,list);
        return 1;
    }
    else if(isValid(y + k*2,x - 2)  && gameField[x - 2][y + 2*k]->isempty() &&
            !gameField[x - 1][y + k]->isempty() && gameField[x - 1][y + k]->color() != gameField[x][y]->color()){
        c->makeSelect(true);
        findmore(y + 2*k,x - 2,list);
        //moveToCell(y + 2,x - 2,blackList);
        return 1;
    }
    else if(isValid(y + k,x + 1) && gameField[x + 1][y + k]->isempty()) {
        c->makeSelect(true);
        moveToCell(y + k,x + 1,list);
        return 1;
    }
    else if( isValid(y + k,x - 1)  && gameField[x - 1][y + k]->isempty()){
        c->makeSelect(true);
        moveToCell(y + k,x - 1,list);
        return 1;
    }
    return 0;
}
void MainWindow:: botstep(int type){
   if(ind2 == 0){
       botgoing(blackList,1);
    /*if(type == 0) botgoing(whiteList,-1);
    else botgoing(blackList,1);*/
   }
   else if(ind2 == 1 || ind2 == 2){
       botAleksey(1);
     /*if(type == 0) botAleksey(-1);
     else botAleksey(1);*/
   }
}
void MainWindow::botAleksey(int k){
    QVector <QVector <int> > field(8,QVector <int>(8,0));
    for(checker* c : whiteList){
        if(c->Type() == 0 && !c->isdelete()) field[c->Y()][c->X()] = -1;
        else if(!c->isdelete()) field[c->Y()][c->X()] = -2;
    }
    for(checker* c : blackList){
        if(c->Type() == 0 && !c->isdelete()) field[c->Y()][c->X()] = 1;
        else if(!c->isdelete()) field[c->Y()][c->X()] = 2;
    }
    std:: fstream file("D:\\program.txt",std::ios_base::app);
     for(int i = 0; i < 8; ++i){
         for(int j = 0; j < 8; ++j){
             file << field[i][j] <<" ";
         }
         file <<'\n';
     }
     file.close();
    //QPair<int,QPair<int,int> > p = botAlekseygoing(0,0,k,2*k,-k,-2*k,field);
    if(ind2 == 1) {
    QVector <point> res = Alekseybot_two(field,BLACK);
    std:: fstream file("D:\\tree.txt",std::ios_base::app);
    for(int i = 0; i < res.size(); ++i){
        file << res[i].x <<" " << res[i].y <<'\n';
    }
    file <<'\n';
    file.close();
    for(int i = 1; i < (int) res.size(); ++i){
      for(checker*c : blackList){
        if(c->Y() == res[i - 1].x && c->X() == res[i - 1].y){
            c->makeSelect(true);
            if(k == 1) moveToCell(res[i].x,res[i].y,blackList);
            else  moveToCell(res[i].x,res[i].y,whiteList);
            break;
        }
       }
    }
    }
    else{
       int kk = Usatykbot(1,field,0,-1e9,1e9);
       for(int i = 1; i < (int) res.size(); ++i){
         for(checker*c : blackList){
           if(c->Y() == res[i - 1].x && c->X() == res[i - 1].y){
               c->makeSelect(true);
               if(k == 1) moveToCell(res[i].x,res[i].y,blackList);
               else  moveToCell(res[i].x,res[i].y,whiteList);
               break;
           }
          }
       }
       res.clear();
    }
}
bool MainWindow:: iseat(int depth,int b,int w,QVector <struct move> m,int k1,int k2,int f,int x,int y,QVector <QVector <int> > field,std::vector <forbot>& q){
    int xx ,yy,x0,y0;
    QVector < struct move> cnt = m;
    bool flag = 0;
    if(f == 1) xx = x - 1, yy = y + 1,x0 = x - 2,y0 = y + 2;
    else if(f == 2) xx = x - 1,yy = y - 1,x0 = x - 2,y0 = y - 2;
    else if(f == 3) xx = x + 1,yy = y + 1,x0 = x + 2,y0 = y + 2;
    else xx = x + 1, yy = y - 1,x0 = x + 2,y0 = y - 2;
    if(isValid(x,y) && isValid(xx,yy) && isValid(x0,y0)){
      if(field[xx][yy] != k1 && field[xx][yy] != k2 && field[x][y] == 0 && field[xx][yy]){
        field[x0][y0] = 0,field[x][y] = k1,field[xx][yy]= 0,flag = 1;
        if(k1 == 1) w -= 1;
        else b -= 1;
       }
    }
    x0 = x,y0 = y;
    while(flag){
        if(isValid(x0 - 2,y0 + 2) && isValid(x0,y0) && isValid(x0 - 1,y0 + 1) && field[x0 - 2][y0 + 2] == 0 && field[x0 - 1][y0 + 1] != k1 && field[x0 - 1][y0 + 1] != k2 && field[x0 - 1][y0 + 1]){
            field[x0][y0] = 0,field[x0-2][y0 + 2] = k1,field[x0 - 1][y0 + 1] = 0;
            if(depth == 0) {
                cnt.push_back({x0,y0,x0 - 2,y0 + 2});
            }
            x0 -= 2,y0 += 2;
            if(k1 == 1) w -= 1;
            else b -= 1;
        }
        else if(isValid(x0 - 2,y0 - 2)&& isValid(x0,y0) && isValid(x0 - 1,y0 - 1) && field[x0 - 2][y0 - 2] == 0 && field[x0 - 1][y0 - 1] != k1 && field[x0 - 1][y0 - 1] != k2 && field[x0 - 1][y0 - 1]){
            field[x0][y0] = 0,field[x0-2][y0 - 2] = k1,field[x0 - 1][y0 - 1] = 0;
            if(depth == 0) {
                cnt.push_back({x0,y0,x0 - 2,y0 - 2});
            }
            x0 -= 2, y0 -= 2;
            if(k1 == 1) w -= 1;
            else b -= 1;
        }
        else if(isValid(x0 + 2,y0 + 2) && isValid(x0,y0) && isValid(x0 + 1,y0 + 1) && field[x0 + 2][y0 + 2] == 0 && field[x0 + 1][y0 + 1] != k1 && field[x0 + 1][y0 + 1] != k2 && field[x0 + 1][y0 + 1]){
            field[x0][y0] = 0,field[x0 + 2][y0 + 2] = k1,field[x0 + 1][y0 + 1] = 0;
            if(depth == 0) {
                cnt.push_back({x0,y0,x0 + 2,y0 + 2});
            }
            x0 += 2, y0 += 2;
            if(k1 == 1) w -= 1;
            else b -= 1;
        }
        else if(isValid(x0 + 2,y0 - 2) && isValid(x0,y0) && isValid(x0 + 1,y0 - 1) && field[x0 + 2][y0 - 2] == 0 && field[x0 + 1][y0 - 1] != k1 && field[x0 + 1][y0 - 1] != k2 && field[x0 + 1][y0 - 1]){
            field[x0][y0] = 0,field[x0 + 2][y0 - 2] = k1,field[x0 + 1][y0 - 1] = 0;
            if(depth == 0) {
                cnt.push_back({x0,y0,x0 + 2,y0 - 2});
            }
            x0 += 2,y0 -= 2;
            if(k1 == 1) w -= 1;
            else b -= 1;
        }
        else break;
    }
    if(flag) q.push_back({field,cnt,b,w});
    return flag;
}
bool MainWindow::isKingeat2(int depth,int&b, int& w,int color,int& x0,int& y0,int dx,int dy,int& x,int& y,QVector <QVector <int> >& field, QVector < struct move>& cnt){
    bool flag = 0;
    while(true){
        if(isValid(x + dx,y+ dy)){
            x += dx, y += dy;
            if(field[x][y] == -color && field[x][y]== -2*color && isValid(x + dx,y+ dy) && field[x + dx][y + dy] == 0){
               field[x - dx][y - dy] = 0,field[x][y] = 0,field[x + dx][y + dy] = color;
               x += dx, y += dy;
             if(depth == 0) cnt.push_back({x0,y0,x,y});
              if(color == 1) w -= 1;
              else b -= 1;
              x0 = x,y0 = y;
              flag = 1;
              break;
            }
            else if(field[x][y] != 0) break;
        }
        else break;
    }
    return flag;
}
bool MainWindow:: isKingeat(int depth,int color,int w,int b,int dx,int dy,int x,int y,QVector <QVector <int> > field,std::vector <forbot>& q,QVector < struct move> m){
    bool flag = 0;
    int x0 = x, y0 = y;
    QVector < struct move> cnt = m;
    while(true){
        if(isValid(x + dx,y + dy)){
            if(field[x + dx][y + dy] == 0) x += dx,y += dy;
            else if(field[x + dx][y + dy] != color && field[x + dx][y + dy] != 2*color && isValid(x + 2*dx,y + 2*dy) && field[x + 2*dx][y + 2*dy] == 0) {
                field[x][y] = 0,field[x + dx][y + dy] = 0,field[x + 2*dx][y + 2*dy] = color;
                x += 2*dx,y += 2*dy;
                flag = 1;
                if(depth == 0) cnt.push_back({x0,y0,x,y});
                if(color == 1) w -= 1;
                else b -= 1;
                break;
            }
            else break;
        }
        else break;
    }
    x0 = x,y0 = y;
    while(flag){
        if(!isKingeat2(depth,b,w,color,x0,y0,-1,-1,x,y,field,cnt) && !isKingeat2(depth,b,w,color,x0,y0,1,-1,x,y,field,cnt)
                && !isKingeat2(depth,b,w,color,x0,y0,-1,1,x,y,field,cnt) && !isKingeat2(depth,b,w,color,x0,y0,1,1,x,y,field,cnt)) break;
    }
    if(flag){
        q.push_back({field,cnt,b,w});
    }
    return flag;
}
void MainWindow::Kingstep(int color,int b,int w,int depth,int x,int y,int dx,int dy,QVector <QVector <int> > field,std::vector <forbot>& q,QVector < struct move> m){

    while(isValid(x + dx,y + dy)){
        if(field[x + dx][y + dy] == 0){
            if(depth == 0) m = {{x,y,x + dx,y + dy}};
            field[x][y] = 0,field[x + dx][y + dy] = color;
            q.push_back({field,m,b,w});
            field[x][y] = color,field[x + dx][y + dy] = 0;
            //break;
        }
    }
}
QVector <struct move> MainWindow::botAleksey2(QVector <QVector <int> > field){
    /*std:: fstream file("D:\\program.txt",std::ios_base::app);
    for(int ii = 0; ii < 8; ii++){
        for(int jj = 0;jj < 8; ++jj){
            file << field[ii][jj] <<" ";
        }
        file <<'\n';
    }
    file <<'\n';
    file.close();*/
   std::vector <forbot> q;
   int depth = 0;
   q.push_back({field,{{0,0,0,0}},black,white});
   while(depth < 5){
     bool flag = 0;
    int k1,k2,k3,k4;
    if(depth%2 == 0) k1 = 1,k2 = 2,k3 = -1,k4 = -2;
    else k1 = -1,k2 = -2,k3 = 1,k4 = 2;
    std::vector <forbot> tmp;
    for(int i = 0; i <(int)q.size(); ++i){
        /*std:: fstream file("D:\\program.txt",std::ios_base::app);
        file << sz <<" ";
        file.close();*/
       QVector <QVector <int> > field = q[i].field;
       QVector <struct move> m;
       if(depth != 0) {
            m = q[i].m;
       }
       int b = q[i].bot;
       int w = q[i].chel;
       for(int i = 0; i < 8; ++i){
          for(int j = 0; j < 8; ++j){
             bool f1 = 0,f2 = 0,f3 = 0,f4 = 0;
             if(field[i][j] == k1){
                 flag = 1;
                 if(depth == 0) m = {{i,j,i + 2,j - 2}};
                 f1 = iseat(depth,b,w,m,k1,k2,1,i + 2,j - 2,field,tmp);
                 if(depth == 0) m = {{i,j,i + 2,j + 2}};
                 f2 = iseat(depth,b,w,m,k1,k2,2,i + 2,j + 2,field,tmp);
                 if(depth == 0) m = {{i,j,i - 2,j - 2}};
                 f3 = iseat(depth,b,w,m,k1,k2,3,i - 2,j - 2,field,tmp);
                 if(depth == 0) m = {{i,j,i - 2,j + 2}};
                 f4 = iseat(depth,b,w,m,k1,k2,4,i - 2,j + 2,field,tmp);

              if(isValid(i + k1,j + 1) && field[i + k1][j + 1] == 0 && !f1 && !f2 && !f3 && !f4){
                  field[i][j] = 0,field[i + k1][j + 1] = k1;
                  if(depth == 0) tmp.push_back({field,{{i,j,i + k1,j + 1}},b,w});
                  else tmp.push_back({field,m,b,w});
                  field[i][j] = k1,field[i + k1][j + 1] =0;
              }
              if(isValid(i + k1,j - 1) && field[i + k1][j - 1] == 0 && !f1 && !f2 && !f3 && !f4){
                  field[i][j] = 0,field[i + k1][j - 1] = k1;
                  if(depth == 0) tmp.push_back({field,{{i,j,i + k1,j - 1}},b,w});
                  else tmp.push_back({field,m,b,w});
                  field[i][j] = k1,field[i + k1][j - 1] =0;
              }
               /*if((int)tmp.size() == sz && sz == 0){
                   std:: fstream file("D:\\program.txt",std::ios_base::app);
                   file << tmp.size() <<" " << i <<" " << j << '\n';
                   for(int ii = 0; ii < 8; ii++){
                       for(int jj = 0;jj < 8; ++jj){
                           file << field[ii][jj] <<" ";
                       }
                       file <<'\n';
                   }
                   file <<'\n';
                   file.close();
               }*/
             }
             else if(field[i][j] == k2){
                 flag = 1;
                 if(depth == 0) m.clear();
                 f1 = isKingeat(depth,k1,b,w,-1,-1,i,j,field,tmp,m);
                 f2 = isKingeat(depth,k1,b,w,-1,1,i,j,field,tmp,m);
                 f3 = isKingeat(depth,k1,b,w,1,-1,i,j,field,tmp,m);
                 f4 = isKingeat(depth,k1,b,w,1,1,i,j,field,tmp,m);
                 if(!f1 && !f2 && !f3 && !f4){
                     Kingstep(k2,b,w,depth,i,j,-1,-1,field,tmp,m);
                     Kingstep(k2,b,w,depth,i,j,1,-1,field,tmp,m);
                     Kingstep(k2,b,w,depth,i,j,-1,1,field,tmp,m);
                     Kingstep(k2,b,w,depth,i,j,1,1,field,tmp,m);
                 }
             }
          }
        }

      }
       if(flag == 0) break;
      int mx = 1e9,mn = 1e9;
      if(tmp.size() == 0){
      std:: fstream File("D:\\program.txt",std::ios_base::app);
      File << depth <<'\n';
      File.close();
      }
         for(int i = 0; i < (int)tmp.size(); ++i){
             if(depth%2 == 0) mx = qMin(mx,tmp[i].chel);
             else mn = qMin(tmp[i].bot,mn);
         }
         q.clear();
        for(int i = 0; i < (int)tmp.size(); ++i){
            if(depth%2 == 0 && mx == tmp[i].chel) q.push_back(tmp[i]);
            else if(depth%2 && mn == tmp[i].bot) q.push_back(tmp[i]);
        }
      depth++;
   }
   /*std:: fstream File("D:\\program.txt",std::ios_base::app);
   File << q.size() <<'\n';
   File.close();*/
   int bot_ans = 0,chel_ans = 1e9;
   QVector <struct move> ans;
   for(int i = 0; i < (int)q.size(); ++i){
       int bot_cnt = q[i].bot,chel_cnt = q[i].chel;
       QVector <struct move> cnt = q[i].m;
       if(bot_cnt > bot_ans){
           bot_ans = bot_cnt;
           chel_ans = chel_cnt;
           ans = cnt;
       }
       else if(bot_ans == bot_cnt && chel_ans > chel_cnt) chel_ans = chel_cnt,ans = cnt;
   }
   return ans;
}
/*QPair<int,QPair<int,int> > MainWindow:: botAlekseygoing(int x,int y,int k1,int k2,int k3,int k4,QVector <QVector <int> > field,int depth){

    if(depth == 3){
    int bot = 0;
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(field[i][j] == k1 || field[i][j] == k2) bot++;
        }
     }
    return {bot,{x,y}};
   }
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){

           if(field[i][j] == k1){

                QPair<int,QPair<int,int> > p1,p2,p3,p4;
                if(depth%2 == 0){
                    p1.first = p2.first = p3.first = p4.first = -1e9;
                }
                else{
                   p1.first = p2.first = p3.first = p4.first = 1e9;
                }
                if(isValid(i + k1,j + 1) && field[i + k1][j + 1] == 0){
                    field[i][j] = 0,field[i + k1][j + 1] = k1;
                    p1 = botAlekseygoing(i + k1,j + 1,k3,k4,k1,k2,field,depth + 1);
                    field[i][j] = k1,field[i + k1][j + 1] =0;
                }
                if(isValid(i + k1,j - 1) && field[i + k1][j - 1] == 0){
                    field[i][j] = 0,field[i + k1][j - 1] = k1;
                    p2 = botAlekseygoing(i + k1,j - 1,k3,k4,k1,k2,field,depth + 1);
                    field[i][j] = k1,field[i + k1][j - 1] =0;
                }
                if(isValid(i + 2*k1,j - 2) && field[i + k1][j - 1] != k1 && field[i + k1][j - 1] != k2
                        && field[i + 2*k1][j - 2] == 0 && field[i + k1][j - 1]){
                    int d = field[i + k1][j - 1];
                    field[i][j] = 0,field[i + 2*k1][j - 2] = k1,field[i + k1][j - 1] = 0;
                    p3 = botAlekseygoing(i + 2*k1,j - 2,k3,k4,k1,k2,field,depth + 1);
                    field[i][j] = k1,field[i + 2*k1][j - 2] =0,field[i + k1][j - 1] = d;
                }
                if(isValid(i + 2*k1,j + 2) && field[i + k1][j + 1] != k1 && field[i + k1][j + 1] != k2
                        && field[i + 2*k1][j + 2] == 0 && field[i + k1][j + 1]){
                    int d = field[i + k1][j + 1];
                    field[i][j] = 0,field[i + 2*k1][j + 2] = k1,field[i + k1][j + 1] = 0;
                    p4 = botAlekseygoing(i + 2*k1,j + 2,k3,k4,k1,k2,field,depth + 1);
                    field[i][j] = k1,field[i + 2*k1][j + 2] =0,field[i + k1][j + 1] = d;
                }
                int M;
                QPair <int,int> p;
                if(depth%2 == 0){
                    M = qMax(p1.first,qMax(p2.first,qMax(p3.first,p4.first)));
                //else  M = qMin(p1.first,qMin(p2.first,qMin(p3.first,p4.first)));
                if(M == p1.first){
                    p = {i + k1,j + 1};
                }
                else if(M == p2.first){
                    p = {i + k1,j - 1};
                }
                else if(M == p3.first){
                    p = {i + 2*k1,j - 2};
                }
                else{
                   p = {i + 2*k1,j + 2};
                }
                }

                if(depth%2 == 0 && M > Mans) {
                    if(i == 0 && j == 1){
                        std:: fstream file("D:\\program.txt",std::ios_base::app);
                         file << M << '\n';
                         file.close();
                    }
                    Mans = M,coord = p;
                    check_num = {i,j};
                }
            }
        }
    }
    return{Mans,coord};
}*/

void MainWindow:: slotRemoveChecker(const int color, const int x, const int y){
    if(color == 1){
        removeChecker(x,y,blackList);
    }
    else{
        removeChecker(x,y,whiteList);
    }
    if(white == 0){
        QMessageBox msgBox;
        msgBox.setText("Game is over.Black won!");
        msgBox.exec();
        StartOver();
    }
    else if(black == 0){
        QMessageBox msgBox;
        msgBox.setText("Game is over.White won.");
        msgBox.exec();
        StartOver();
    }
}
void MainWindow:: removeChecker (const int x,const int y, QVector<checker*>& list){
    for(checker* check: list){
        if(check->X() == x && check->Y() == y && !check->isdelete()){
            if(check->color() == 0) white--;
            else black--;
            scene->removeItem(check);
            check->Delete();
            gameField[x][y]->makeEmpty(true);
            gameField[x][y]->setColor(-1);
        }
    }
    if(white == 0){
        QMessageBox msgBox;
        msgBox.setText("Game is over.Black won!");
        msgBox.exec();
        StartOver();
    }
    else if(black == 0){
        QMessageBox msgBox;
        msgBox.setText("Game is over.White won.");
        msgBox.exec();
        StartOver(1);
    }
}
void MainWindow:: StartOver(int k){
    scene->clear();
    scene->update();
    blackList.clear();
    whiteList.clear();
    gameField.clear();
    createGameField();
    createCheckerList();
    createConnections();
    if(k){
      goBlack = false;
      goWhite = true;
    }
    black = 12;
    white = 12;
}
void  MainWindow:: slotClearVariantes (){
        for(QVector<cell*> v : gameField){
            for(cell *c : v){
                c->setDefaultColor();
                c->makeSelect(false);
            }
        }
        for (checker *c : blackList) {
            c->setDefaultColor();
        }
        for (checker *c : whiteList) {
            c->setDefaultColor();
        }
}



void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    ind2 = index;
    scene_2->clear();
    scene_2->update();
    if(ind2 == 0){
        scene_2->addPixmap(QPixmap("://images/beginner.png"));
        ui->graphicsView_2->show();
    }
    else if(ind2 == 1){
       scene_2->addPixmap(QPixmap("://images/Alexey.jpg"));
        ui->graphicsView_2->show();
    }
    else if(ind2 == 2){
        scene_2->addPixmap(QPixmap("://images/Usatyk.jpg"));
         ui->graphicsView_2->show();
    }
    else{
        scene_2->addPixmap(QPixmap("://images/other_person.png"));
        ui->graphicsView_2->show();
    }
}

void MainWindow::on_pushButton_pressed()
{
    StartOver();
}

void MainWindow::on_button_pressed()
{
    QString s;
    bool f = 0;
    if(radio1->isChecked()){
        s = "Game is over.White won.",f = 1;
    }
    else if(radio2->isChecked()){
        s = "Game is over.Black won.",f = 1;
    }
    if(f){
        QMessageBox msgBox;
        msgBox.setText(s);
        msgBox.exec();
        StartOver();
    }
    thirdPageWidget->close();
    //std:: string str;
    //bool ok;
   //str = QInputDialog::getText(0,QString::fromUtf8("Input"),QString::fromStdString("What kind "),QLineEdit::Normal,"",&ok).toStdString();
}
void MainWindow::on_button2_pressed(){
    thirdPageWidget->close();
}
void MainWindow:: possibleEat(int depth,QVector <QVector <int> > field,int i,int j,QVector <point> cnt,int black,int white){
       int color = field[i][j];
       bool f1 = 0,f2 = 0,f3 = 0,f4 = 0;
       if(isValid(i + 2, j + 2) && field[i + 2][j + 2] == 0 && (field[i + 1][j + 1] == -color || field[i + 1][j + 1] == -2*color)){
           if(!depth) cnt.push_back({i,j});
           f1 = 1,fiseat = 1;
           int d = field[i + 1][j + 1];
           field[i + 2][j + 2] = color,field[i + 1][j + 1] = 0,field[i][j] = 0;
           color == BLACK ? white-- : black--;
           possibleEat(depth,field,i + 2,j + 2,cnt,black,white);
           color == BLACK ? white++ : black++;
           field[i + 2][j + 2] = 0,field[i + 1][j + 1] = d,field[i][j] = color;
           if(!depth) cnt.pop_back();
           return;
        }
       if(isValid(i - 2, j + 2) && field[i - 2][j + 2] == 0 && (field[i - 1][j + 1] == -color || field[i - 1][j + 1] == -2*color)){
           if(!depth) cnt.push_back({i,j});
           f2 = 1,fiseat = 1;
           int d = field[i - 1][j + 1];
           field[i - 2][j + 2] = color,field[i - 1][j + 1] = 0,field[i][j] = 0;
           color == BLACK ? white-- : black--;
           possibleEat(depth,field,i - 2,j + 2,cnt,black,white);
           color == BLACK ? white++ : black++;
           field[i - 2][j + 2] = 0,field[i - 1][j + 1] = d,field[i][j] = color;
           if(!depth) cnt.pop_back();
           return;
        }
       if(isValid(i + 2, j - 2) && field[i + 2][j - 2] == 0 && (field[i + 1][j - 1] == -color || field[i + 1][j - 1] == -2*color)){
           if(!depth) cnt.push_back({i,j});
           f3 = 1,fiseat = 1;
           int d = field[i + 1][j - 1];
           field[i + 2][j - 2] = color,field[i + 1][j - 1] = 0,field[i][j] = 0;
           color == BLACK ? white-- : black--;
           possibleEat(depth,field,i + 2,j - 2,cnt,black,white);
           color == BLACK ? white++ : black++;
           field[i + 2][j - 2] = 0,field[i + 1][j - 1] = d,field[i][j] = color;
           if(!depth) cnt.pop_back();
           return;
        }
       if(isValid(i - 2, j - 2) && field[i - 2][j - 2] == 0 && (field[i - 1][j - 1] == -color || field[i - 1][j - 1] == -2*color)){
           if(!depth) cnt.push_back({i,j});
           f4 = 1,fiseat = 1;
           int d = field[i - 1][j - 1];
           field[i - 2][j - 2] = color,field[i - 1][j - 1] = 0,field[i][j] = 0;
           color == BLACK ? white-- : black--;
           possibleEat(depth,field,i - 2,j - 2,cnt,black,white);
           color == BLACK ? white++ : black++;
           field[i - 2][j - 2] = 0,field[i - 1][j - 1] = d,field[i][j] = color;
           if(!depth) cnt.pop_back();
           return;
        }
       if(!f1 && !f2 && !f3 && !f4){
           if(!cnt.size() || !fiseat){
               if(color == WHITE) {
                   if(isValid(i - 1,j + 1) && field[i - 1][j + 1] == 0){
                       field[i - 1][j + 1] = color,field[i][j] = 0;
                       if(!depth) cnt.push_back({i,j});
                       if(!depth) cnt.push_back({ i - 1,j + 1});
                       step.push_back({field,cnt});
                       blackwhite.push_back({black,white});
                       field[i - 1][j + 1] = 0,field[i][j] = color;
                   }
                   if(isValid(i - 1,j - 1) && field[i - 1][j - 1] == 0){
                       field[i - 1][j - 1] = color,field[i][j] = 0;
                       if(!depth) cnt.push_back({i,j});
                       if(!depth) cnt.push_back({ i - 1,j - 1});
                       step.push_back({field,cnt});
                       blackwhite.push_back({black,white});
                       field[i - 1][j - 1] = 0,field[i][j] = color;
                   }
               }
               else{
                   if(isValid(i + 1,j + 1) && field[i + 1][j + 1] == 0){
                       field[i + 1][j + 1] = color,field[i][j] = 0;
                       if(!depth) cnt.push_back({i,j});
                       if(!depth) cnt.push_back({ i + 1,j + 1});
                       step.push_back({field,cnt});
                       blackwhite.push_back({black,white});
                       field[i + 1][j + 1] = 0,field[i][j] = color;
                   }
                   if(isValid(i + 1,j - 1) && field[i + 1][j - 1] == 0){
                       field[i + 1][j - 1] = color,field[i][j] = 0;
                       if(!depth) cnt.push_back({i,j});
                       if(!depth) cnt.push_back({ i + 1,j - 1});
                       step.push_back({field,cnt});
                       blackwhite.push_back({black,white});
                       field[i + 1][j - 1] = 0,field[i][j] = color;
                   }
               }
           }
           else {
              if(!depth) cnt.push_back({i,j});
               step.push_back({field,cnt});
               blackwhite.push_back({black,white});
           }
           return;
       }
}
int MainWindow:: getFunction(int color,QVector <QVector <int> > field){
    int res = 0;
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(field[i][j] == color || field[i][j] == 2*color) res++;
        }
     }
    return res;
}
void MainWindow::possibleEatKing(int depth,QVector <QVector <int> > field,int x,int y,QVector <point> cnt,int black,int white){
    int x0 = x,y0 = y;
    int color = field[x][y]/2;
    int f1 = 0,f2 = 0,f3 = 0,f4 = 0;
    while(isValid(x0 + 1,y0 + 1)){
        if(field[x0 + 1][y0 + 1] == 0) x0 += 1, y0 += 1;
        else if(isValid(x0 + 2,y0 + 2) && (field[x0 + 1][y0 + 1] == -color || field[x0 + 1][y0 + 1] == -2*color) && !field[x0 + 2][y0 + 2]){
            int d = field[x0 + 1][y0 + 1];
            field[x0 + 1][y0 + 1] = 0,field[x][y] = 0,field[x0 + 2][y0 + 2] = 2*color,f1 = 1,fiseat = 1;
            if(!depth) cnt.push_back({x,y});
            color == BLACK ? white-- : black--;
            possibleEatKing(depth,field,x0 + 2,y0 + 2,cnt,black,white);
            color == BLACK ? white++ : black++;
            if(!depth) cnt.pop_back();
            field[x0 + 1][y0 + 1] = d,field[x][y] = 2*color,field[x0 + 2][y0 + 2] = 0;
        }
    }
    while(isValid(x0 - 1,y0 + 1)){
        if(field[x0 - 1][y0 + 1] == 0) x0 -= 1, y0 += 1;
        else if(isValid(x0 - 2,y0 + 2) && (field[x0 - 1][y0 + 1] == -color || field[x0 - 1][y0 + 1] == -2*color) && !field[x0 - 2][y0 + 2]){
            int d = field[x0 - 1][y0 + 1];
            field[x0 - 1][y0 + 1] = 0,field[x][y] = 0,field[x0 - 2][y0 + 2] = 2*color,f2 = 1,fiseat = 1;
            if(!depth) cnt.push_back({x,y});
            color == BLACK ? white-- : black--;
            possibleEatKing(depth,field,x0 - 2,y0 + 2,cnt,black,white);
            color == BLACK ? white++ : black++;
            if(!depth) cnt.pop_back();
            field[x0 - 1][y0 + 1] = d,field[x][y] = 2*color,field[x0 - 2][y0 + 2] = 0;
        }
    }
    while(isValid(x0 + 1,y0 - 1)){
        if(field[x0 + 1][y0 - 1] == 0) x0 += 1, y0 -= 1;
        else if(isValid(x0 + 2,y0 - 2) && (field[x0 + 1][y0 - 1] == -color || field[x0 + 1][y0 - 1] == -2*color) && !field[x0 + 2][y0 - 2]){
            int d = field[x0 + 1][y0 - 1];
            field[x0 + 1][y0 - 1] = 0,field[x][y] = 0,field[x0 + 2][y0 - 2] = 2*color,f3 = 1,fiseat = 1;
            if(!depth) cnt.push_back({x,y});
            color == BLACK ? white-- : black--;
            possibleEatKing(depth,field,x0 + 2,y0 - 2,cnt,black,white);
            color == BLACK ? white++ : black++;
            if(!depth) cnt.pop_back();
            field[x0 + 1][y0 - 1] = d,field[x][y] = 2*color,field[x0 + 2][y0 - 2] = 0;
        }
    }
    while(isValid(x0 - 1,y0 - 1)){
        if(field[x0 - 1][y0 - 1] == 0) x0 -= 1, y0 -= 1;
        else if(isValid(x0 - 2,y0 - 2) && (field[x0 - 1][y0 - 1] == -color || field[x0 - 1][y0 - 1] == -2*color) && !field[x0 - 2][y0 - 2]){
            int d = field[x0 - 1][y0 - 1];
            field[x0 - 1][y0 - 1] = 0,field[x][y] = 0,field[x0 - 2][y0 - 2] = 2*color,f4 = 1,fiseat = 1;
            if(!depth) cnt.push_back({x,y});
            color == BLACK ? white-- : black--;
            possibleEatKing(depth,field,x0 - 2,y0 - 2,cnt,black,white);
            color == BLACK ? white++ : black++;
            if(!depth) cnt.pop_back();
            field[x0 - 1][y0 - 1] = d,field[x][y] = 2*color,field[x0 - 2][y0 - 2] = 0;
        }
    }
    if(!f1 && !f2 && !f3 && !f4){
        if(!cnt.size() || !fiseat){
            while(isValid(x0 + 1,y0 + 1) && !field[x0 + 1][y0 + 1]){
                field[x0][y0] = 0,field[x0 + 1][y0 + 1] = 2*color;
                if(!depth) step.push_back({field,{{x,y},{x0 + 1,y0 + 1}}});
                else step.push_back({field,cnt});
                blackwhite.push_back({black,white});
                field[x0][y0] = 2*color,field[x0 + 1][y0 + 1] = 0;
                x0 += 1,y0 += 1;
            }
            while(isValid(x0 - 1,y0 + 1) && !field[x0 - 1][y0 + 1]){
                field[x0][y0] = 0,field[x0 - 1][y0 + 1] = 2*color;
                if(!depth) step.push_back({field,{{x,y},{x0 - 1,y0 + 1}}});
                else step.push_back({field,cnt});
                blackwhite.push_back({black,white});
                field[x0][y0] = 2*color,field[x0 - 1][y0 + 1] = 0;
                x0 -= 1,y0 += 1;
            }
            while(isValid(x0 + 1,y0 - 1) && !field[x0 + 1][y0 - 1]){
                field[x0][y0] = 0,field[x0 + 1][y0 - 1] = 2*color;
                if(!depth) step.push_back({field,{{x,y},{x0 + 1,y0 - 1}}});
                else step.push_back({field,cnt});
                blackwhite.push_back({black,white});
                field[x0][y0] = 2*color,field[x0 + 1][y0 - 1] = 0;
                x0 += 1,y0-= 1;
            }
            while(isValid(x0 - 1,y0 - 1) && !field[x0 - 1][y0 - 1]){
                field[x0][y0] = 0,field[x0 - 1][y0 - 1] = 2*color;
                if(!depth) step.push_back({field,{{x,y},{x0 - 1,y0 - 1}}});
                else step.push_back({field,cnt});
                blackwhite.push_back({black,white});
                field[x0][y0] = 2*color,field[x0 - 1][y0 - 1] = 0;
                x0 -= 1, y0 -= 1;
            }
        }
        else {
            if(!depth) cnt.push_back({x,y});
            step.push_back({field,cnt});
            blackwhite.push_back({black,white});
        }
    }
}
QVector <point> MainWindow::Alekseybot_two(QVector <QVector <int> > field,int color){
    QVector <position> pos;
    pos.push_back({field,{{0,0}},black,white});
 for(int depth = 0; depth < 5; ++depth){
  for(int k = 0; k < pos.size(); ++k){
     QVector <QVector <int> > field = pos[k].field;
     int bl = pos[k].black,wh = pos[k].white;
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(field[i][j] == color || field[i][j] == 2*color){
                QVector <point> cnt;
                if(depth) cnt = pos[k].cnt;
                if(field[i][j] == color) possibleEat(depth,field,i,j,cnt,bl,wh);
                else possibleEatKing(depth,field,i,j,cnt,bl,wh);
                fiseat = 0;
            }
        }
     }
  }

    int ans = 1e9;
    pos.clear();
    for(int i = 0; i < blackwhite.size(); ++i){
        if(depth%2 == 0) ans = qMin(ans,blackwhite[i].y);
        else ans = qMin(ans,blackwhite[i].x);
    }
    for(int i = 0; i < blackwhite.size(); ++i){
        if((depth%2 == 0 && blackwhite[i].y == ans) || (depth%2 && blackwhite[i].x == ans)){
             pos.push_back({step[i].field,step[i].cnt,blackwhite[i].x,blackwhite[i].y});
        }
    }
    std:: fstream file("D:\\circle.txt",std::ios_base::app);
    file << pos.size() <<'\n';
    for(int ii = 0; ii < pos.size(); ++ii){
        file <<pos[ii].black <<" " << pos[ii].white <<'\n';
        for(int i = 0; i < 8; ++i){
            for(int j = 0; j < 8; ++j){
                file <<pos[ii].field[i][j] <<" ";
            }
           file <<'\n';
        }
        file <<'\n';
    }
    file.close();
    step.clear();
    blackwhite.clear();
    if(ans == 0) {
        int kk = 0;
        break;
    }
    color *= -1;
}
   if(pos.size() == 0){
       int k = 0;

   }
   return pos[0].cnt;
}
int MainWindow::Usatykbot(int color,QVector <QVector <int> > field,int depth,int alpha,int beta){

    if(depth == 5) return getFunction(color,field);
    if(!getFunction(color,field)){
         if(color == BLACK) return INF_MAX;
         else return INF_MIN;
     }
    int MinMax = (color == -1) ? -1e9 : 1e9;
   // QVector <point> res;
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            if(field[i][j] == color || field[i][j] == 2*color){
                    QVector <point> cnt;
                    if(field[i][j] == color) possibleEat(0,field,i,j,cnt,0,0);
                    else possibleEatKing(0,field,i,j,cnt,0,0);
                    QVector <usatyk> tmp = step;
                    step.clear();
                    for(int k = 0; k < tmp.size(); ++k){
                        int test = Usatykbot(-color,tmp[k].field,depth + 1,alpha,beta);
                        if((color == WHITE && test > MinMax) || (color == BLACK && test < MinMax)){
                            MinMax = test;
                            if(depth == 0) res = tmp[k].cnt;
                            /*std:: fstream file("D:\\program.txt",std::ios_base::app);
                             for(int ii = 0; ii < res.size(); ++ii){
                                 file << res[ii].x <<" " << res[ii].y <<" ";
                             }
                             file <<'\n';
                             file.close();*/
                        }
                        if(color == WHITE) alpha = qMax(alpha,test);
                        else beta = qMin(beta,test);
                        if(alpha > beta) {
                            break;
                        }
                    }
            }
        }
    }
    return MinMax;
}
