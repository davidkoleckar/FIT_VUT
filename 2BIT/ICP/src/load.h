#ifndef LOAD_H
#define LOAD_H

#include <QDialog>
#include <string>
using namespace std;

/**
 * Trida Load realizujici vykresleni nacteni hry
 * @file load.cpp
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

namespace Ui {
class Load;
}

class Load : public QDialog
{
    Q_OBJECT

public:
    explicit Load(QWidget *parent = 0);
    string filen;
    bool isL;
    ~Load();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Load *ui;

};

#endif // LOAD_H
