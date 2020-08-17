/*************************************************************
 * Projekt: IVS - projekt 2
 * Soubor: mainwindow.h
 * Cas vytvoreni: 10.4.2017 11:50
 * Cas posledni zmeny: 23.4.2017 20:00
 *
 * Autor:   Koleckar David xkolec07@stud.fit.vurbr.cz
 *          Janota Ondrej xjanot02@stud.fit.vutbr.cz
 *
 * Popis: Hlavickovy soubor GUI.
 *
 ***********************************************************/
/**
 * @file mainwindow.h
 *
 * @author Ondrej Janota <xjanot02@stud.fit.vutbr.cz>
 * @author David Koleckar <xkolec07@stud.fit.vutbr.cz>
 *
 * @date 23.4.2017
 *
 * @brief Hlavickovy soubor GUI knihovny.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include <QApplication>

#include "ui_mainwindow.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include "string.h"

#include "../math_library.h"

namespace Ui {
class MainWindow;
}

/**
 * @class MainWindow
 *
 * Trida MainWindow realizujici vyklesleni kalkulacky.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    /**
     * Hlavni okno.
     */
    Ui::MainWindow *ui;
    /**
     * Flag pro tecku.
     */
    bool tecka;
    /**
     * Flag pro vysledek.
     */
    bool vysledek;
    /**
     * Vektor kalkulacek.
     */
    std::vector<Calculator> calc;
    /**
     * Vektor flagu soustav.
     */
    std::vector<bool> system_;
    /**
     * Vysledek hexa prevodu.
     */
    std::string hex_result;
    /**
     * Promenna pro pocet zavorek (.
     */
    int count_bracket = 0;

    void keyPressEvent(QKeyEvent *event);
    void enable10();
    void enable16();
    void enable8();
    void disable2();
    void disable8();
    void disable10();
    void disableoptions();

    void prepare_display();
    void update_text(std::string text);
    void error(std::string err);

    void make_it();
    void make_it_int();
    void make_it_double();

    void do_example_int(Calculator::ops op);
    void do_example_int_double(Calculator::ops op);
    void do_example_double_int(Calculator::ops op);
    void do_example_double(Calculator::ops op);

    double get_last_result();
    void new_calculator();

    void plus_minus(std::string line);
    void krat_deleno(std::string line);
    void mocnina(std::__cxx11::string zaklad_s);
    void after_pow();

    void operation_add_sub(double last_result, Calculator::type last);
    void operation_mul_div(double last_result, Calculator::type last);
    void operation_ypow_yroot(double last_result, Calculator::type last);

    void set_int(std::string number);
    void set_double(std::string number);

    void set_first(std::string number);
    void set_second(std::string number);

    void set_first_by_result();

    void set_first_by_last(double last_result, Calculator::type last);
    void set_second_by_last(double last_result, Calculator::type last);

    void do_last_operation();
    void set_new_calc();

    void set_result(double *last_result, Calculator::type *last);
    void write_number(std::string number);
    void clear_display();
    void erase_number();
private slots:
    void on_a_clicked();
    void on_b_clicked();
    void on_c_clicked();
    void on_d_clicked();
    void on_e_clicked();
    void on_f_clicked();
    void on_hex_clicked();
    void on_bin_clicked();
    void on_dec_clicked();
    void on_oct_clicked();
    void on_nula_clicked();
    void on_jednicka_clicked();
    void on_dvojka_clicked();
    void on_trojka_clicked();
    void on_ctverka_clicked();
    void on_petka_clicked();
    void on_sestka_clicked();
    void on_sedmicka_clicked();
    void on_osmicka_clicked();
    void on_devitka_clicked();
    void on_plus_clicked();
    void on_minus_clicked();
    void on_krat_clicked();
    void on_deleno_clicked();
    void on_faktorial_clicked();
    void on_odmocnina_clicked();
    void on_yodmocnina_clicked();
    void on_mocnina_clicked();
    void on_ymocnina_clicked();
    void on_rovnase_clicked(bool bracket);
    void on_clear_clicked();
    void on_levazavorka_clicked();
    void on_pravazavorka_clicked();
    void on_tecka_clicked();
};

#endif // MAINWINDOW_H
