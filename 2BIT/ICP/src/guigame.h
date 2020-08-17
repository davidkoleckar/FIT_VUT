#ifndef GUIGAME_H
#define GUIGAME_H

#include <QMainWindow>
#include <QPushButton>
#include <QThread>
#include "include/Game.h"
#include "include/newGame.h"
#include <QMessageBox>


/**
 * Trida guiGame realizujici vykresleni hraci desky a provadeni tahu
 *
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

namespace Ui {
class guiGame;
}

class guiGame : public QMainWindow
{

    Q_OBJECT


public:
    explicit guiGame(QWidget *parent = 0);
    void init(int size, newGame *gg, bool isComp);
    void myButtonPressed(int m);
    ~guiGame();

private:
    Ui::guiGame *ui;    
    QTimer* mTimer;
    QPushButton *button[14][14];
    void doText();
    void blackPaint();
    void whitePaint();
    void showMove();
    void remove();
    void checkEnd();
    int s;
    bool isPC;
    newGame *newg;
    Game *g;

private slots:
    void handleButton(int i, int j);
    void on_pushButton_clicked();
    void callAi();
    //void run();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // GUIGAME_H
