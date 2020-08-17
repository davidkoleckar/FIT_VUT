#include "mainwindow.h"

/**
 * Hlavni funkce spusteni GUI.
 *
 * @param argc Pocet argumentu.
 * @param argv Argumenty programu.
 * @return Navratovy kod.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
