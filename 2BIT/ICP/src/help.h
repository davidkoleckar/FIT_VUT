#ifndef HELP_H
#define HELP_H

#include <QDialog>

/**
 * Trida Help realizujici vykresleni napovedy
 * @file help.h
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

namespace Ui {
class Help;
}

class Help : public QDialog
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

private:
    Ui::Help *ui;
};

#endif // HELP_H
