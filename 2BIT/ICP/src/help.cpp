#include "help.h"
#include "ui_help.h"

/**
 * Trida Help realizujici vykresleni napovedy
 * @file help.cpp
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

/**
 * Konstruktor tridy Help
 * @param parent
 */
Help::Help(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
}

/**
 * Destruktor
 */
Help::~Help()
{
    delete ui;
}
