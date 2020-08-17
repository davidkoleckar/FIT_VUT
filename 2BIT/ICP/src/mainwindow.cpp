#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "guigame.h"
#include "include/Board.h"
#include "include/Game.h"
#include "include/Player.h"
#include "include/ReversiRules.h"
#include "include/newGame.h"
#include <iostream>
/**
 * Trida Mainwindow vykresluje hlavni okno hry
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

/**
 * Konstruktor
 * @param parent
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox_2->setCurrentIndex(1);
    isComp=false;
    isLoad=false;
}

/**
 * Destruktor MainWindow
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Funkce pro kliknuti na tlacitko hrac vs hrac
 * Zobrazi nove okno guiGame s hraci deskou
 */
void MainWindow::on_pushButton_4_clicked()
{
    s = ui->comboBox_2->currentText().toInt();
    int ai =  ui->comboBox->currentText().toInt();
    filename = "";
    newGame *newg = new newGame(s, isComp, ai, make_pair(false, filename));

    gui = new guiGame(this);
    if(s == 10 || s == 12){
        gui->setFixedSize(800,600);
        gui->setStyleSheet("background-image:url(:/img/bg2.png);");

    }

    gui->show();
    gui->init(s,newg,isComp);
    isComp = false;
}


/**
 * Funkce pro vykresleni napovedy
 */
void MainWindow::on_pushButton_clicked()
{
    h = new Help(this);
    h->show();
}
/**
 * Funkce pro vykresleni okna nacteni hry
 */
void MainWindow::on_pushButton_2_clicked()
{
    l = new Load(this);
    l->show();
}

/**
 * Funkce pro kliknuti na tlacitko hrac vs pocitac
 * Nastavi priznak hra proti pc
 */
void MainWindow::on_pushButton_3_clicked()
{
    isComp = true;
    on_pushButton_4_clicked();
}
