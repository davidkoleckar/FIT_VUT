#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "guigame.h"
#include "help.h"
#include "load.h"
/**
 * Trida Mainwindow vykresluje hlavni okno hry
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int s;
    bool isLoad;
    string filename;
public slots:
     void on_pushButton_4_clicked();
private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    guiGame *gui;
    Help *h;
    Load *l;
    bool isComp;

};

#endif // MAINWINDOW_H
