#include "guigame.h"
#include "ui_guigame.h"
#include "mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QtGui>
#include <QInputDialog>



/**
 * Trida guiGame realizujici vykresleni hraci desky a provadeni tahu
 *
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */


/**
 * Konstruktor guiGame
 * @param parent
 */
guiGame::guiGame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::guiGame)
{
    ui->setupUi(this);
    //timer pro tah AI
    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    connect(mTimer, SIGNAL(timeout()), SLOT(callAi()));
}

/**
 * Destruktor guiGame
 */
guiGame::~guiGame()
{
    delete ui;
}

/**
 * Funkce pro inicializaci hraci desky
 * @param size Velikost desky
 * @param gg Objekt Game
 * @param isComp Zda je hra proti PC
 */
void guiGame::init(int size, newGame *gg, bool isComp)
{

    s = size;
    newg = gg;
    g = newg->game;
    isPC = isComp;
    QGridLayout *controlsLayout = new QGridLayout;
    int height = 420/ size;
    if(size == 10 || size == 12)
    {
        ui->frame->resize(540,540);
        height = 540/size;
        int x = ui->pushButton->pos().x();
        int y = ui->pushButton->pos().y();
        ui->pushButton->move(x+140,y+100);
        y = ui->pushButton_2->pos().y();
        ui->pushButton_2->move(x+140,y+100);
        y = ui->pushButton_3->pos().y();
        ui->pushButton_3->move(x+140,y+100);
        x = ui->label->pos().x();
        y = ui->label->pos().y();
        ui->label->move(x+50,y);
        x = ui->label_5->pos().x();
        ui->label_5->move(x+130,y);
        x = ui->label_3->pos().x();
        ui->label_3->move(x+130,y);
    }

    int i;
    int j;
        for(i=1;i<size+1;i++)
        {
            for(j=1;j<size+1;j++)
            {
                    button[i][j] = new QPushButton();
                    button[i][j]->setMinimumSize(height,height);
                    button[i][j]->setStyleSheet("QPushButton{background: green; color:blue; border: 1px solid black;}");
                    connect(button[i][j], &QPushButton::released, [=]{
                        handleButton(i, j);
                    });

                    controlsLayout->addWidget(button[i][j],i,j);
                    controlsLayout->setHorizontalSpacing(0);
                    controlsLayout->setVerticalSpacing(0);
                    ui->frame->setLayout(controlsLayout);
                }
        }

        //vykresleni prvnich 4 sutru
        showMove();
        blackPaint();
        whitePaint();
        doText();
}

/**
 * Funkce pro vlozeni disku na hraci desku po kliknuti na dane policko
 * @param i Souradnice ve smeru osy X
 * @param j Souradnice ve smeru osy Y
 */
void guiGame::handleButton(int i, int j)
{
    if(!isPC){
        if(g->currentPlayer()->putDisk(g->getBoard()->getField(i,j))){
            g->nextPlayer();
            checkEnd();
            showMove();
            blackPaint();
            whitePaint();
            doText();
        }
    }
    else{
        if(g->currentPlayer()->putDisk(g->getBoard()->getField(i,j))){
            g->nextPlayer();
            checkEnd();
            showMove();
            blackPaint();
            whitePaint();
            doText();
            }
        if(g->currentPlayer()->is_white()){
            mTimer->start(1000);
        }
    }
}

/**
 * Funkce pro kontrolu konce hry
 * Spocita skore a zobrazi dialog s hracem ktery vyhral
 */
void guiGame::checkEnd(){
    if(!g->currentPlayer()->checkMoves()){
        g->nextPlayer();
        if(g->currentPlayer()->checkMoves()){
            showMove();
            whitePaint();
            blackPaint();
            doText();
            QMessageBox msgBox;
            msgBox.setText("CANT MOVE");
            msgBox.exec();
        }
        else{
            g->setEnd();
            whitePaint();
            blackPaint();
            doText();
            if(g->getDisksBlack()+g->getDisksWhite() != s*s){
                if(g->getDisksBlack() > g->getDisksWhite())
                    g->setDisks(false,s*s-g->getDisksWhite());
                else
                    g->setDisks(true,s*s- g->getDisksBlack());
            }
            if(g->getDisksBlack() > g->getDisksWhite()){
                QMessageBox msgBox;
                msgBox.setText("BLACK WON");
                msgBox.exec();
                this->close();
            }
            else{
                if(g->getDisksBlack() != g->getDisksWhite()){
                    if(isPC){
                        QMessageBox msgBox;
                        msgBox.setText("PC WON");
                        msgBox.exec();
                        this->close();
                    }
                    else{
                        QMessageBox msgBox;
                        msgBox.setText("WHITE WON");
                        msgBox.exec();
                        this->close();
                    }
                }
                else{
                    QMessageBox msgBox;
                    msgBox.setText("DRAW");
                    msgBox.exec();
                    this->close();
                }
            }
        }

    }
}

/**
 * Funkce pro aktualizaci aktualniho skore a hrace na tahu
 */
void guiGame::doText(){
    if(g->currentPlayer()->is_white()){
        ui->label_2->setStyleSheet("color: white; font-size:18pt;  background:none; font-style:bold;");
        ui->label_2->setText("BLACK");
        ui->label_3->setStyleSheet("color: yellow; font-size:18pt;  background:none; font-style:bold;");
        ui->label_3->setText("WHITE");
    }
    else{
        ui->label_3->setStyleSheet("color: white; font-size:18pt;  background:none; font-style:bold;");
        ui->label_2->setText("BLACK");
        ui->label_2->setStyleSheet("color: yellow; font-size:18pt;  background:none; font-style:bold;");
        ui->label_3->setText("WHITE");
    }
    ui->label_4->setStyleSheet("font-size:18pt; color:white; background:none; font-style:bold;");
    ui->label_4->setText(QString::number(g->getDisksBlack()));
    ui->label_5->setStyleSheet("font-size:18pt; color:white; background:none; font-style:bold;");
    ui->label_5->setText(QString::number(g->getDisksWhite()));
}

/**
 * Funkce pro tah umele inteligence
 * Pouziva zpozdeni Timer 1 sekundu
 */
void guiGame::callAi(){
    g->AImoves();
    g->nextPlayer();
    showMove();
    blackPaint();
    whitePaint();
    doText();
    checkEnd();
}

/**
 * Funkce vykresli cerne kameny na hraci desku
 */
void guiGame::blackPaint(){
    for(int i=1;i<s+1;i++){
        for(int j=1;j<s+1;j++){
            if(g->getBoard()->getField(i,j)->getDisk() != nullptr)
            {
                if(!g->getBoard()->getField(i,j)->getDisk()->is_white()){
                    QString options = "background: green; background-image:url(:/img/"+QString::number(s)+"b.png); background-position:center; background-repeat: repeat-no; border: 1px solid black;";
                    button[i][j]->setStyleSheet(options);
                    }
            }
        }
    }

}

/**
 * Funkce vykresli bile kameny na hraci desku
 */
void guiGame::whitePaint(){
    for(int i=1;i<s+1;i++){
        for(int j=1;j<s+1;j++){
            if(g->getBoard()->getField(i,j)->getDisk() != nullptr)
            {
                if(g->getBoard()->getField(i,j)->getDisk()->is_white()){
                    QString options = "background: green; background-image:url(:/img/"+QString::number(s)+"w.png); background-position:center; background-repeat: repeat-no; border: 1px solid black;";
                    button[i][j]->setStyleSheet(options);
                    }
            }
        }
    }
}

/**
 * Funkce pro zobrazeni moznych tahu na hraci desce
 */
void guiGame::showMove(){
    for(int i=1;i<s+1;i++){
        for(int j=1;j<s+1;j++){
            if(g->currentPlayer()->canPutDisk(g->getBoard()->getField(i,j)))
            {
                button[i][j]->setStyleSheet("QPushButton {background: green; border: 1px solid black;} "
                                      "QPushButton:hover {background-color: rgba(255, 255, 255, 15); }");

            }
            else{
                button[i][j]->setStyleSheet("QPushButton {background: green; border: 1px solid black;} "
                                      "QPushButton:hover {background: green; border: 1px solid black; }");

            }
        }
    }

}


/**
 * Funkce po kliknuti na tlacitko Save game
 * Vykresli dialog pro zadani jmena souboru
 */
void guiGame::on_pushButton_clicked()
{
    bool ok;
    QInputDialog* inputDialog = new QInputDialog();
    QString text = inputDialog->getText(nullptr, tr("Save"),
                                           tr("File name:"), QLineEdit::Normal,
                                           QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty())
         g->saveGame(text.toUtf8().constData());
}
/**
 * Funkce pro kliknuti na tlacitko undo
 */
void guiGame::on_pushButton_2_clicked()
{
    newg->doUndo();
    remove();
    doText();
}

/**
 * Funkce pro odstraneni kamenu
 */
void guiGame::remove(){
    for(int i=1;i<s+1;i++){
        for(int j=1;j<s+1;j++){
            if(g->getBoard()->getField(i,j)->getDisk() != nullptr)
            {
                if(g->getBoard()->getField(i,j)->getDisk()->is_white()){
                    QString options = "background: green; background-image:url(:/img/"+QString::number(s)+"w.png); background-position:center; background-repeat: repeat-no; border: 1px solid black;";
                    button[i][j]->setStyleSheet(options);
                    }
                else{
                    QString options = "background: green; background-image:url(:/img/"+QString::number(s)+"b.png); background-position:center; background-repeat: repeat-no; border: 1px solid black;";
                    button[i][j]->setStyleSheet(options);
                }
            }
            else if(g->currentPlayer()->canPutDisk(g->getBoard()->getField(i,j)))
            {
                button[i][j]->setStyleSheet("QPushButton {background: green; border: 1px solid black;} "
                                      "QPushButton:hover {background-color: rgba(255, 255, 255, 15); }");

            }
            else if(!g->currentPlayer()->canPutDisk(g->getBoard()->getField(i,j))){
                button[i][j]->setStyleSheet("QPushButton {background: green; border: 1px solid black;} "
                                      "QPushButton:hover {background: green; border: 1px solid black; }");

            }
            else{
                QString options = "background: green; border: 1px solid black;";
                button[i][j]->setStyleSheet(options);

            }
        }

    }
}
/**
 * Funkce pro kliknuti na tlacitko redo
 */
void guiGame::on_pushButton_3_clicked()
{
 newg->doRedo();
 remove();
 doText();
}
