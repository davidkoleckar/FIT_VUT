#include "load.h"
#include "ui_load.h"
#include <QDir>
#include "mainwindow.h"
#include "include/newGame.h"
#include "guigame.h"

/**
 * Trida Load realizujici vykresleni nacteni hry
 * @file load.cpp
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

/**
 * Konstruktor Load
 * @param parent
 */
Load::Load(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Load)
{
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    QDir directory("examples");
    QStringList f = directory.entryList(QStringList(),QDir::Files);
    foreach(QString filename, f) {
       ui->listWidget->addItem(filename);
    }

}

/**
 * Destruktor
 */
Load::~Load()
{
    delete ui;
}

/**
 * Po kliknuti na tlacitko Load Game
 */
void Load::on_pushButton_clicked()
{
    isL = true;
    QString s  = ui->listWidget->currentItem()->text();
    filen = s.toUtf8().constData();
    filen = filen.substr(0, filen.size()-4);
    newGame *newg = new newGame(0, 0, 0, make_pair(isL, filen));

    guiGame *gui = new guiGame(this);
    if(newg->game->getBoard()->getRules()->getSize() == 10 || newg->game->getBoard()->getRules()->getSize() == 12){
        gui->setFixedSize(800,600);
        gui->setStyleSheet("background-image:url(:/img/bg2.png);");

    }
    cout<<filen;
    gui->show();
    gui->init(newg->game->getBoard()->getRules()->getSize(),newg,newg->game->isAI);
}
