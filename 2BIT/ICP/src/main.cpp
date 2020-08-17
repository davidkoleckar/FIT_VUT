#include "mainwindow.h"
#include <QApplication>
/**
 * Main
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
