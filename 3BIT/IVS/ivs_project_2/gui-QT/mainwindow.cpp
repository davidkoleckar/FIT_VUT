#include "mainwindow.h"

/**
 * Konstruktor hlavního okna.
 *
 * @param parent Rodicovsky widget.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    system_.push_back(false);
    system_.push_back(false);
    system_.push_back(true);
    system_.push_back(false);
    new_calculator();
    tecka = false;
    vysledek = false;

    enable10();
    ui->dec->setStyleSheet("background-color:rgb(25,25,25); color: rgb(199,61,0);");
}

/**
 * Destruktor hlavniho okna.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Funkce pro pripravu operaci pro prevod ciselnych soustav.
 *
 * Zmeni barvu textu BIN/OCT/DEC/HEX na sedou.
 */
void MainWindow::disableoptions(){
    ui->dec->setStyleSheet("background-color:rgb(25,25,25); color: rgb(148,148,148);");
    ui->hex->setStyleSheet("background-color:rgb(25,25,25); color: rgb(148,148,148);");
    ui->oct->setStyleSheet("background-color:rgb(25,25,25); color: rgb(148,148,148);");
    ui->bin->setStyleSheet("background-color:rgb(25,25,25); color: rgb(148,148,148);");
}

/**
 * Funkce pro vypnuti operaci pro desitkovou soustavu.
 *
 * Zmeni barvu a znemozni pouziti tlacitek hexadecimalni soustavy.
 */
void MainWindow::disable10(){
    ui->a->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->a->setEnabled(false);
    ui->b->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->b->setEnabled(false);
    ui->c->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->c->setEnabled(false);
    ui->d->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->d->setEnabled(false);
    ui->e->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->e->setEnabled(false);
    ui->f->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->f->setEnabled(false);
}

/**
 * Funkce pro vypnuti operaci pro osmickovou soustavu.
 *
 * Zmeni barvu a znemozni pouziti tlacitek hexa/decimalni soustavy.
 */
void MainWindow::disable8(){
    disable10();
    ui->osmicka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->osmicka->setEnabled(false);
    ui->devitka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->devitka->setEnabled(false);
}

/**
 * Funkce pro vypnuti operaci pro dvojkovou soustavu.
 *
 * Zmeni barvu a znemozni pouziti tlacitek hexa/decimalni/oktalove soustavy.
 */
void MainWindow::disable2(){
    disable8();
    ui->dvojka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->dvojka->setEnabled(false);
    ui->trojka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->trojka->setEnabled(false);
    ui->ctverka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->ctverka->setEnabled(false);
    ui->petka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->petka->setEnabled(false);
    ui->sestka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->sestka->setEnabled(false);
    ui->sedmicka->setStyleSheet("background-color: rgba(255, 255, 255, 10); color:rgb(148,148,148); border: 3px solid rgb(45,45,45);");
    ui->sedmicka->setEnabled(false);
}

/**
 * Funkce pro zapnuti operaci pro sestnactkovou soustavu.
 *
 * Zmeni barvu a umozni pouziti tlacitek hexa/decimalni/oktalove/binarni soustavy.
 */
void MainWindow::enable16(){
    ui->dvojka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->dvojka->setEnabled(true);
    ui->trojka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->trojka->setEnabled(true);
    ui->ctverka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->ctverka->setEnabled(true);
    ui->petka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->petka->setEnabled(true);
    ui->sestka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->sestka->setEnabled(true);
    ui->sedmicka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->sedmicka->setEnabled(true);
    ui->osmicka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->osmicka->setEnabled(true);
    ui->devitka->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->devitka->setEnabled(true);
    ui->a->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->a->setEnabled(true);
    ui->b->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->b->setEnabled(true);
    ui->c->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->c->setEnabled(true);
    ui->d->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->d->setEnabled(true);
    ui->e->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->e->setEnabled(true);
    ui->f->setStyleSheet("background-color:rgb(45,45,45);color: rgb(148,148,148);");
    ui->f->setEnabled(true);
}

/**
 * Funkce pro zapnuti operaci pro desitkovou soustavu.
 *
 * Zmeni barvu a znemozni pouziti tlacitek hexadecimalni soustavy a
 * umozni pouziti tlacitek decimalni soustavy.
 */
void MainWindow::enable10(){
        enable16();
        disable10();
}

/**
 * Funkce pro zapnuti operaci pro osmickovou soustavu.
 *
 * Zmeni barvu a znemozni pouziti tlacitek hexadecimalni soustavy a
 * umozni pouziti tlacitek oktalove soustavy.
 */
void MainWindow::enable8(){
        enable16();
        disable8();
}

/**
 * Funkce pro zajisteni stisku klavesy.
 *
 * Spousti operaci, ktera by byla spustena pri stisku tlacitka.
 *
 * @param event Udalost, ktera udrzuje dany klic.
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()){
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Equal:
            if (system_.at(2))
                on_rovnase_clicked(false);
        break;

        case Qt::Key_0:
            on_nula_clicked();
        break;

        case Qt::Key_1:
            on_jednicka_clicked();
        break;

        case Qt::Key_2:
            if (!system_.at(0))
                on_dvojka_clicked();
        break;

        case Qt::Key_3:
            if (!system_.at(0))
                on_trojka_clicked();
        break;

        case Qt::Key_4:
            if (!system_.at(0))
                on_ctverka_clicked();
        break;

        case Qt::Key_5:
            if (!system_.at(0))
                on_petka_clicked();
        break;

        case Qt::Key_6:
            if (!system_.at(0))
                on_sestka_clicked();
        break;

        case Qt::Key_7:
            if (!system_.at(0))
                on_sedmicka_clicked();
        break;

        case Qt::Key_8:
            if (!system_.at(0))
                if (!system_.at(1))
                    on_osmicka_clicked();
        break;

        case Qt::Key_9:
            if (!system_.at(0))
                if (!system_.at(1))
                    on_devitka_clicked();
        break;

        case Qt::Key_A:
            if (system_.at(3))
                on_a_clicked();
        break;

        case Qt::Key_B:
            if (system_.at(3))
                on_b_clicked();
        break;

        case Qt::Key_C:
            if (system_.at(3))
                on_c_clicked();
        break;

        case Qt::Key_D:
            if (system_.at(3))
                on_d_clicked();
        break;

        case Qt::Key_E:
            if (system_.at(3))
                on_e_clicked();
        break;

        case Qt::Key_F:
            if (system_.at(3))
                on_f_clicked();
        break;

        case Qt::Key_Plus:
            if (system_.at(2))
                on_plus_clicked();
       break;

        case Qt::Key_Minus:
            if (system_.at(2))
                on_minus_clicked();
        break;

        case Qt::Key_ParenLeft:
            on_levazavorka_clicked();
        break;

        case Qt::Key_ParenRight:
            on_pravazavorka_clicked();
        break;

        case Qt::Key_Slash:
            if (system_.at(2))
                on_deleno_clicked();
        break;

        case Qt::Key_Asterisk:
            if (system_.at(2))
                on_krat_clicked();
        break;

        case Qt::Key_Period:
        case Qt::Key_Comma:
            if (system_.at(2))
                on_tecka_clicked();
        break;

        case Qt::Key_Delete:
        case Qt::Key_Escape:
            on_clear_clicked();
        break;

        case Qt::Key_Backspace:
            erase_number();
        break;

        case Qt::Key_P:
            if (system_.at(2))
                on_mocnina_clicked();
        break;

        case Qt::Key_R:
            if (system_.at(2))
                on_odmocnina_clicked();
        break;

        case Qt::Key_X:
            if (system_.at(2))
                on_yodmocnina_clicked();
        break;

        case Qt::Key_Y:
            if (system_.at(2))
                on_ymocnina_clicked();
        break;

        case Qt::Key_Exclam:
            if (system_.at(2))
                on_faktorial_clicked();
        break;

        case Qt::Key_F1:
            on_bin_clicked();
        break;

        case Qt::Key_F2:
            on_oct_clicked();
        break;

        case Qt::Key_F3:
            on_dec_clicked();
        break;

        case Qt::Key_F4:
            on_hex_clicked();
        break;

        default:
            break;
    }
}

/**
 * Funkce zajisteni kliknuti na A.
 */
void MainWindow::on_a_clicked()
{
    if (!system_.at(3))
        return;

    clear_display();

    write_number("A");
}

/**
 * Funkce zajisteni kliknuti na B.
 */
void MainWindow::on_b_clicked()
{
    if (!system_.at(3))
        return;

    clear_display();

    write_number("B");
}

/**
 * Funkce zajisteni kliknuti na C.
 */
void MainWindow::on_c_clicked()
{
    if (!system_.at(3))
        return;

    clear_display();

    write_number("C");
}

/**
 * Funkce zajisteni kliknuti na D.
 */
void MainWindow::on_d_clicked()
{
    if (!system_.at(3))
        return;

    clear_display();

    write_number("D");
}

/**
 * Funkce zajisteni kliknuti na E.
 */
void MainWindow::on_e_clicked()
{
    if (!system_.at(3))
        return;

    clear_display();

    write_number("E");
}

/**
 * Funkce zajisteni kliknuti na F.
 */
void MainWindow::on_f_clicked()
{
    if (!system_.at(3))
        return;

    clear_display();

    write_number("F");
}

/**
 * Funkce zajisteni klinkuti na HEX.
 */
void MainWindow::on_hex_clicked()
{
    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    set_first(number);

    std::vector<bool> copy = system_;

    system_[3] = true;

    for (unsigned i = 0; i < copy.size(); i++)
        if (copy.at(i))
        {
            switch(i)
            {
                case 0:
                    calc.back().operation = Calculator::bin_hex;
                break;

                case 1:
                    calc.back().operation = Calculator::oct_hex;
                break;

                case 2:
                    calc.back().operation = Calculator::dec_hex;
                break;
            }

            system_[i] = false;
            break;
        }

    enable16();
    disableoptions();
    ui->hex->setStyleSheet("background-color:rgb(25,25,25); color: rgb(199,61,0);");

    make_it();

    update_text(hex_result);
}

/**
 * Funkce zajisteni kliknuti na DEC.
 */
void MainWindow::on_dec_clicked()
{
    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    set_first(number);

    std::vector<bool> copy = system_;

    system_[2] = true;

    for (unsigned i = 0; i < copy.size(); i++)
        if (copy.at(i))
        {
            switch(i)
            {
                case 0:
                    calc.back().operation = Calculator::bin_dec;
                break;

                case 1:
                    calc.back().operation = Calculator::oct_dec;
                break;

                case 3:
                    hex_result = number;
                    calc.back().operation = Calculator::hex_dec;
                break;
            }

            system_[i] = false;
            break;
        }

    enable10();
    disableoptions();
    ui->dec->setStyleSheet("background-color:rgb(25,25,25); color: rgb(199,61,0);");

    make_it();

    update_text(std::to_string(calc.back().get_result()));
}

/**
 * Funkce zajisteni kliknuti na OCT.
 */
void MainWindow::on_oct_clicked()
{
    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    set_first(number);

    std::vector<bool> copy = system_;

    system_[1] = true;

    for (unsigned i = 0; i < copy.size(); i++)
        if (copy.at(i))
        {
            switch(i)
            {
                case 0:
                    calc.back().operation = Calculator::bin_oct;
                break;

                case 2:
                    calc.back().operation = Calculator::dec_oct;
                break;

                case 3:
                    hex_result = number;
                    calc.back().operation = Calculator::hex_oct;
                break;
            }

            system_[i] = false;
            break;
        }

    enable8();
    disableoptions();
    ui->oct->setStyleSheet("background-color:rgb(25,25,25); color: rgb(199,61,0);");

    make_it();

    update_text(std::to_string(calc.back().get_result()));
}

/**
 * Funkce zajisteni kliknuti na BIN.
 */
void MainWindow::on_bin_clicked()
{
    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    set_first(number);

    std::vector<bool> copy = system_;

    system_[0] = true;

    for (unsigned i = 0; i < copy.size(); i++)
        if (copy.at(i))
        {
            switch(i)
            {
                case 1:
                    calc.back().operation = Calculator::oct_bin;
                break;

                case 2:
                    calc.back().operation = Calculator::dec_bin;
                break;

                case 3:
                    hex_result = number;
                    calc.back().operation = Calculator::hex_bin;
                break;
            }

            system_[i] = false;
            break;
        }

    disable2();
    disableoptions();
    ui->bin->setStyleSheet("background-color:rgb(25,25,25); color: rgb(199,61,0);");

    make_it();

    update_text(std::to_string(calc.back().get_result()));
}

/**
 * Funkce pro vycisteni displeje.
 */
void MainWindow::clear_display()
{
    if (vysledek)
    {
        ui->lineEdit->setText("");
        vysledek = false;
    }
}

/**
 * Funkce pro smazani posledniho cisla.
 */
void MainWindow::erase_number()
{
    std::string line = ui->lineEdit->text().toStdString();

    if (line.size())
        if (isdigit(line.at(line.size()-1)))
            ui->lineEdit->setText(QString::fromStdString(line.substr(0, line.size()-1)));
}

/**
 * Funkce pro zapsani 0.
 */
void MainWindow::on_nula_clicked()
{
    clear_display();

    std::string line = ui->lineEdit->text().toStdString();

    if ((line.find(")") != line.size()-1) || !line.size())
        if ((line.find_first_of("0") != 0) || (line.find_first_of("0") == 0 && (line.find_first_of(".") == 1)))
            ui->lineEdit->setText(ui->lineEdit->text() + "0");
}

/**
 * Funkce pro zapsani tecky.
 */
void MainWindow::on_tecka_clicked()
{
    if (!system_.at(2))
        return;

    if (vysledek)
    {
        ui->lineEdit->setText("");
        vysledek = false;
        tecka = false;
    }

    if (!tecka)
    {
        unsigned size = 0;
        if ((ui->lineEdit->text().toStdString().find(")") != (size = ui->lineEdit->text().toStdString().size())-1) || !size)
        {
            if (size == 0)
                ui->lineEdit->setText(ui->lineEdit->text() + "0");
            ui->lineEdit->setText(ui->lineEdit->text() + ".");
            tecka = true;
        }
    }
}

/**
 * Funkce pro zapsani 1.
 */
void MainWindow::on_jednicka_clicked()
{
    clear_display();

    write_number("1");
}

/**
 * Funkce pro zapsani 2.
 */
void MainWindow::on_dvojka_clicked()
{
    if (system_.at(0))
        return;

    clear_display();

    write_number("2");
}

/**
 * Funkce pro zapsani 3.
 */
void MainWindow::on_trojka_clicked()
{
    if (system_.at(0))
        return;

    clear_display();

    write_number("3");
}

/**
 * Funkce pro zapsani 4.
 */
void MainWindow::on_ctverka_clicked()
{
    if (system_.at(0))
        return;

    clear_display();

    write_number("4");
}

/**
 * Funkce pro zapsani 5.
 */
void MainWindow::on_petka_clicked()
{
    if (system_.at(0))
        return;

    clear_display();

    write_number("5");
}

/**
 * Funkce pro zapsani 6.
 */
void MainWindow::on_sestka_clicked()
{
    if (system_.at(0))
        return;

    clear_display();

    write_number("6");
}

/**
 * Funkce pro zapsani 7.
 */
void MainWindow::on_sedmicka_clicked()
{
    if (system_.at(0))
        return;

    clear_display();

    write_number("7");
}

/**
 * Funkce pro zapsani 8.
 */
void MainWindow::on_osmicka_clicked()
{
    if (system_.at(0) || system_.at(1))
        return;

    clear_display();

    write_number("8");
}

/**
 * Funkce pro zapsani 9.
 */
void MainWindow::on_devitka_clicked()
{
    if (system_.at(0) || system_.at(1))
        return;

    clear_display();

    write_number("9");
}

/**
 * Funkce pro zapsani cisla na displej.
 *
 * @param number Cislo, ktere ma byt zapsano.
 */
void MainWindow::write_number(std::string number)
{
    std::string line = ui->lineEdit->text().toStdString();

    if (line.size())
    {
        if (line.at(line.size()-1) != ')')
        {
            if (calc.size())
            {
                if (calc.back().operation != Calculator::fact_)
                    ui->lineEdit->setText(ui->lineEdit->text() + QString::fromStdString(number));
            }
            else
                ui->lineEdit->setText(ui->lineEdit->text() + QString::fromStdString(number));
        }
        else
            ui->lineEdit->setText(ui->lineEdit->text() + QString::fromStdString(number));
    }
    else
        ui->lineEdit->setText(ui->lineEdit->text() + QString::fromStdString(number));

    if ((ui->lineEdit->text().toStdString().find_first_of("0") == 0) && ui->lineEdit->text().toStdString().find_first_of(".") != 1)
        ui->lineEdit->setText(QString::fromStdString(number));
}

/**
 * Funkce pro pripravu displeje po ziskani vysledku.
 */
void MainWindow::prepare_display()
{
    if (vysledek)
    {
        if (ui->label->text().toStdString().find("!") != std::string::npos)
            return;

        ui->lineEdit->setText(ui->label->text());
        vysledek = false;
    }
}

/**
 * Funkce pro nastaveni desetinneho cisla.
 *
 * @param number Desetinne cislo.
 */
void MainWindow::set_double(std::string number)
{
    if (calc.back().operation == Calculator::mul || calc.back().operation == Calculator::div ||
            calc.back().operation == Calculator::ypow_ || calc.back().operation == Calculator::yroot)
    {
        calc.back().second_double = std::stod(number.c_str(), NULL);
        calc.back().second = Calculator::double_;
    }
    else
    {
        calc.back().first_double = std::stod(number.c_str(), NULL);
        calc.back().first = Calculator::double_;
    }
}

/**
 * Funkce pro nastaveni celeho cisla.
 *
 * @param number Cele cislo.
 */
void MainWindow::set_int(std::string number)
{
    if (calc.back().operation == Calculator::mul || calc.back().operation == Calculator::div ||
            calc.back().operation == Calculator::ypow_ || calc.back().operation == Calculator::yroot)
    {
        calc.back().second_int = atoi(number.c_str());
        calc.back().second = Calculator::int_;
    }
    else
    {
        calc.back().first_int = atoi(number.c_str());
        calc.back().first = Calculator::int_;
    }
}

/**
 * Funkce pro provedeni posledni operace.
 */
void MainWindow::do_last_operation()
{
    if (calc.back().first == Calculator::int_)
    {
        calc[calc.size()-2].second_int = calc.back().first_int;
        calc[calc.size()-2].second = Calculator::int_;
    }
    else
    {
        calc[calc.size()-2].second_double = calc.back().first_double;
        calc[calc.size()-2].second = Calculator::double_;
    }

    calc.pop_back();

    make_it();

    set_first_by_result();
}

/**
 * Funkce pro nastaveni prvniho operandu pomoci vysledku.
 *
 * Vysledek posledni operace se zapise do prvniho operandu
 * podle jeho typu.
 */
void MainWindow::set_first_by_result()
{
    double result = calc.back().get_result();

    if ((result - (int)result) != 0.0)
    {
        calc.back().first_double = result;
        calc.back().first = Calculator::double_;
    }
    else
    {
        calc.back().first_int = (int)result;
        calc.back().first = Calculator::int_;
    }

    calc.back().second = Calculator::none_;
}

/**
 * Funkce nastavi prvni operand pomoci posledniho vysledku.
 *
 * Vysledek posledni operace se zapise do prvniho operandu.
 *
 * @param last_result Posledni vysledek.
 * @param last Typ posledniho vysledku.
 */
void MainWindow::set_first_by_last(double last_result, Calculator::type last)
{
    if (last == Calculator::int_)
    {
        calc.back().first_int = (int)last_result;
        calc.back().first = Calculator::int_;
    }
    else
    {
        calc.back().first_double = last_result;
        calc.back().first = Calculator::double_;
    }

    calc.back().second_int = 0;
    calc.back().second = Calculator::int_;
}

/**
 * Funkce nastavi druhy operand pomoci posledniho vysledku.
 *
 * Vysledek posledni operace se zapise do druheho operandu.
 *
 * @param last_result Posledni vysledek.
 * @param last Typ posledniho vysledku.
 */
void MainWindow::set_second_by_last(double last_result, Calculator::type last)
{
    if (last == Calculator::int_)
    {
        std::ostringstream out;
        out << std::setprecision(15)<< last_result;
        calc.back().second_int = std::atoi(out.str().c_str());
        calc.back().second = Calculator::int_;
    }
    else
        if (last == Calculator::double_)
        {
            std::ostringstream out;
            out << std::setprecision(15)<< last_result;
            calc.back().second_double = std::stod(out.str());
            calc.back().second = Calculator::double_;
        }
        else
        {
            std::string line = ui->lineEdit->text().toStdString();

            std::string last_res_s;

            last_res_s = line.substr(line.find_last_of("+*-/^ ") + 1);

            if (last_res_s.find(".") != std::string::npos)
            {
                calc.back().second_double = std::stod(last_res_s.c_str(), NULL);
                calc.back().second = Calculator::double_;
            }
            else
            {
                calc.back().second_int = atoi(last_res_s.c_str());
                calc.back().second = Calculator::int_;
            }
        }
}

/**
 * Nastavi novou kalkulacku.
 *
 * Provede posledni operaci a vysledek nastavi do prvniho operandu.
 */
void MainWindow::set_new_calc()
{
    make_it();

    set_first_by_result();
}

/**
 * Funkce nastavi prvni operand.
 *
 * @param number Cislo, ktere se nastavi.
 */
void MainWindow::set_first(std::string number)
{
    if (number.find(".") != std::string::npos)
    {
        calc.back().first_double = std::stod(number.c_str(), NULL);
        calc.back().first = Calculator::double_;
    }
    else
    {
        calc.back().first_int = atoi(number.c_str());
        calc.back().first = Calculator::int_;
    }
}

/**
 * Funkce nastavi druhy operand.
 *
 * @param number Cislo, ktere se nastavi.
 */
void MainWindow::set_second(std::string number)
{
    if (number.find(".") != std::string::npos)
    {
        calc.back().second_double = std::stod(number.c_str());
        calc.back().second = Calculator::double_;
    }
    else
    {
        calc.back().second_int = std::atoi(number.c_str());
        calc.back().second = Calculator::int_;
    }
}

/**
 * Funkce zajistujici spravny chod operace +/-.
 *
 * @param line Radek kalkulacky.
 */
void MainWindow::plus_minus(std::string line)
{
    std::string number = line;

    if (calc.size() >= 2)
        if (calc[calc.size() - 2].operation == Calculator::bracket)
             number = number = number.substr(number.find_last_of("(") + 1);


    if (calc.back().operation == Calculator::fact_)
    {
        make_it();
        set_first_by_result();

        if (calc.size() >= 2)
            if (calc[calc.size()-2].operation == Calculator::yroot || calc[calc.size()-2].operation == Calculator::ypow_)
                do_last_operation();
    }
    else
    if (calc.back().operation == Calculator::ypow_ || calc.back().operation == Calculator::yroot)
    {
        number = number.substr(number.find_last_of("*/^ ") + 1);

        set_second(number);

        set_new_calc();

        if (calc.size() >= 2)
            if (calc[calc.size()-2].operation == Calculator::mul || calc[calc.size()-2].operation == Calculator::div)
                do_last_operation();
    }
    else
        if (calc.back().operation == Calculator::mul || calc.back().operation == Calculator::div)
        {
            number = number.substr(number.find_last_of("*/^ ") + 1);

            set_second(number);

            set_new_calc();

            if (calc.size() >= 2)
                if (calc[calc.size()-2].operation == Calculator::sub)
                    do_last_operation();
        }
        else
            if (calc.back().operation != Calculator::root_ && calc.back().operation != Calculator::pow_)
            {
                if (number.find_last_of("+*-/^ ") != 0)
                {
                    if (number.find_last_of("+*-/^ ") != std::string::npos)
                        number = number.substr(number.find_last_of("+*-/^ ") + 1);
                }

                if (number.find(".") != std::string::npos)
                    set_double(number);
                else
                    set_int(number);

                if (calc.back().operation == Calculator::mul || calc.back().operation == Calculator::div ||
                        calc.back().operation == Calculator::ypow_ || calc.back().operation == Calculator::yroot)
                    set_new_calc();

                if (calc.size() >= 2)
                    if (calc[calc.size()-2].operation == Calculator::sub)
                        do_last_operation();
            }
            else
                if (calc.size() >= 2)
                    if (calc[calc.size()-2].operation == Calculator::mul || calc[calc.size()-2].operation == Calculator::div)
                        do_last_operation();
}

/**
 * Funkce zajistujici kliknuti na +.
 */
void MainWindow::on_plus_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string line = ui->lineEdit->text().toStdString();

    int size = line.size();

    if(size != 0)
    {
        if (!isdigit(line.at(line.size()-1)) && (line.at(line.size()-1) != ')') && calc.back().operation != Calculator::fact_)
            return;

        plus_minus(line);

        calc.back().operation = Calculator::add;

        new_calculator();

        ui->lineEdit->setText(QString::fromStdString(line) + "+");

        tecka = false;
    }
}

/**
 * Funkce zajistujici kliknuti na -.
 */
void MainWindow::on_minus_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string line = ui->lineEdit->text().toStdString();

    int size = line.size();

    if (size != 0)
    {
        if (!isdigit(line.at(line.size()-1)) && (line.at(line.size()-1) != ')') && calc.back().operation != Calculator::fact_)
            return;

        plus_minus(line);

        calc.back().operation = Calculator::sub;

        new_calculator();

        ui->lineEdit->setText(QString::fromStdString(line) + "-");

        tecka = false;
    }
    else
        ui->lineEdit->setText(QString::fromStdString(line) + "-");
}

/**
 * Funkce zajistujici spravny chod operace * a /
 * @param line Radek kalkulacky.
 */
void MainWindow::krat_deleno(std::string line)
{
    std::string number = line;

    if (number.find_last_of("+*-/^ ") != std::string::npos)
        number = number.substr(number.find_last_of("+*-/^ ") + (line.find_last_of("+-*/^ ") != 0));

    if (calc.size() >= 2)
        if (calc[calc.size() - 2].operation == Calculator::bracket)
             number = number = number.substr(number.find_last_of("(") + 1);

    if (calc.back().operation == Calculator::fact_)
    {
        make_it();
        set_first_by_result();
    }
    else
    if (calc.back().operation != Calculator::root_ && calc.back().operation != Calculator::pow_)
    {
        if (calc.back().operation == Calculator::mul || calc.back().operation == Calculator::div ||
            calc.back().operation == Calculator::ypow_ || calc.back().operation == Calculator::yroot)
        {
            set_second(number);

            set_new_calc();

            if (calc.back().operation == Calculator::ypow_ || calc.back().operation == Calculator::yroot)
                if (calc.size() >= 2)
                    if (calc[calc.size()-2].operation == Calculator::mul || calc[calc.size()-2].operation == Calculator::div)
                        do_last_operation();
        }
        else
            if (calc.size() >= 2)
            {
                if ((calc[calc.size()-2].operation == Calculator::mul || calc[calc.size()-2].operation == Calculator::div) && calc.back().first != Calculator::none_)
                {
                    do_last_operation();
                    set_first_by_result();
                }
                else
                    if (calc.back().first == Calculator::none_)
                        set_first(number);
            }
            else
                if (calc.back().first == Calculator::none_)
                    set_first(number);
    }
    else
        if (calc.size() >= 2)
            if (calc[calc.size()-2].operation == Calculator::mul || calc[calc.size()-2].operation == Calculator::div)
                do_last_operation();
}

/**
 * Funkce zajistujici kliknuti na *.
 */
void MainWindow::on_krat_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string line = ui->lineEdit->text().toStdString();

    int size = line.size();

    if (size != 0)
    {
        if (!isdigit(line.at(line.size()-1)) && (line.at(line.size()-1) != ')') && calc.back().operation != Calculator::fact_)
            return;

        krat_deleno(line);

        calc.back().operation = Calculator::mul;

        ui->lineEdit->setText(QString::fromStdString(line) + "*");

        tecka = false;
    }
}

/**
 * Funkce zajistujici kliknuti na /.
 */
void MainWindow::on_deleno_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string line = ui->lineEdit->text().toStdString();

    int size = line.size();

    if (size != 0)
    {
        if (!isdigit(line.at(line.size()-1)) && (line.at(line.size()-1) != ')') && calc.back().operation != Calculator::fact_)
            return;

        krat_deleno(line);

        calc.back().operation = Calculator::div;

        ui->lineEdit->setText(QString::fromStdString(line) + "/");

        tecka = false;
    }
}

/**
 * Funkce zajistujici kliknuti na (.
 */
void MainWindow::on_levazavorka_clicked()
{
    if (!system_.at(2))
        return;

    clear_display();

    std::string line = ui->lineEdit->text().toStdString();

    if (line.size())
    {
        if (isdigit(line.at(line.size()-1)) || line.at(line.size()-1) == ')')
        {
            on_krat_clicked();
            ui->lineEdit->setText(QString::fromStdString(line) + "*(");
        }
        else
            ui->lineEdit->setText(ui->lineEdit->text() + "(");
    }
    else
        ui->lineEdit->setText(ui->lineEdit->text() + "(");

    if (calc.back().first != Calculator::none_)
        new_calculator();

    calc.back().operation = Calculator::bracket;

    new_calculator();

    count_bracket++;
}

/**
 * Funkce zajistujici kliknuti na ).
 */
void MainWindow::on_pravazavorka_clicked()
{
    if (!system_.at(2))
        return;

    ui->lineEdit->setText(ui->lineEdit->text() + ")");

    on_rovnase_clicked(true);

    count_bracket--;
}

/**
 * Funkce zajistujici kliknuti na CLEAR.
 */
void MainWindow::on_clear_clicked()
{
      ui->lineEdit->setText("");
      calc.clear();
      new_calculator();
      tecka = false;
}

/**
 * Funkce zajistujici spravny chod operace ^.
 * @param number Zaklad mocniny.
 */
void MainWindow::mocnina(std::string number)
{
    if (calc.back().first == Calculator::none_)
        set_first(number);
    else
        if (calc.back().second == Calculator::none_)
        {
            if (calc.back().operation == Calculator::ypow_)
            {
                new_calculator();
                set_first(number);
            }
            else
            if (calc.back().operation == Calculator::yroot)
            {
                set_second(number);
                make_it();
                set_first_by_result();
            }
            else
            if (calc.back().operation == Calculator::fact_)
            {
                make_it();
                set_first_by_result();
            }
            else
            if (calc.back().operation == Calculator::pow_ || calc.back().operation == Calculator::root_)
                set_second(number);
            else
                if (calc.back().operation != Calculator::none)
                {
                    new_calculator();
                    set_first(number);
                }
        }
}

/**
 * Funkce zajistujici kliknuti na x^2.
 */
void MainWindow::on_mocnina_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    if (!isdigit(number.at(number.size()-1)) && number.at(number.size()-1) != ')')
        return;

    if (number.at(number.size()-1) == ')')
    {
        if (calc.back().operation == Calculator::none && calc.back().first != Calculator::none_)
        {
            if (calc.back().first == Calculator::int_)
                number = std::to_string(calc.back().first_int);
            else
                number = std::to_string(calc.back().first_double);
        }
        else
            number = number.substr(number.find_last_of("+*-/^ ")+1);
    }
    else
        number = number.substr(number.find_last_of("+*-/^) ")+1);

    mocnina(number);

    calc.back().second_int = 2;
    calc.back().second = Calculator::int_;

    calc.back().operation = Calculator::pow_;

    make_it();

    set_first_by_result();

    ui->lineEdit->setText(ui->lineEdit->text() + "^2");
}

/**
 * Funkce zajistujici kliknuti na x^y.
 */
void MainWindow::on_ymocnina_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    if (!isdigit(number.at(number.size()-1)) && (number.at(number.size()-1) != ')') && calc.back().operation != Calculator::fact_)
        return;

    number = number.substr(number.find_last_of("+*-/^) ")+1);

    mocnina(number);

    calc.back().operation = Calculator::ypow_;

    ui->lineEdit->setText(ui->lineEdit->text() + "^");
}

/**
 * Funkce zajistujici kliknuti na yroot.
 */
void MainWindow::on_yodmocnina_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    if (!isdigit(number.at(number.size()-1)) && (number.at(number.size()-1) != ')') && calc.back().operation != Calculator::fact_)
        return;

    number = number.substr(number.find_last_of("+*-/^) ")+1);

    mocnina(number);

    calc.back().operation = Calculator::yroot;

    ui->lineEdit->setText(ui->lineEdit->text() + " yroot ");
}

/**
 * Funkce zajistujici kliknuti na root.
 */
void MainWindow::on_odmocnina_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    bool was_pow = false;
    bool was_root = false;

    std::string number = ui->lineEdit->text().toStdString();

    if (!isdigit(number.at(number.size()-1)) && calc.back().operation != Calculator::root_)
        if (!isdigit(number.at(number.size()-1)) && (number.at(number.size()-1) != ')') && calc.back().operation != Calculator::fact_)
        return;

    if (calc.back().operation == Calculator::root_)
    {
        number = number.substr(number.find("sqrt(")+5);
        if (number.find(")"))
            number = number.substr(0, number.size()-1);
    }
    else

        if (number.at(number.size()-1) == ')')
        {
            if (calc.back().operation == Calculator::none && calc.back().first != Calculator::none_)
            {
                if (calc.back().first == Calculator::int_)
                    number = std::to_string(calc.back().first_int);
                else
                    number = std::to_string(calc.back().first_double);
            }
            else
                number = number.substr(number.find_last_of("+*-/^ ")+1);
        }
        else

        number = number.substr(number.find_last_of("+*-/^ ")+1);

    mocnina(number);

    std::string line = ui->lineEdit->text().toStdString();

    std::string line_pow;

    if (line.find_last_of("+*-/^! ") != std::string::npos)
    {
        if (calc.back().operation == Calculator::pow_ || calc.back().operation == Calculator::root_
                || calc.back().operation == Calculator::fact_)
        {
            line_pow = line.substr(line.find_last_of("+*-/ ")+1);
            line = line.substr(0, line.find_last_of("+*-/ ")+1);
            was_pow = true;
        }
        else

            if (calc.back().operation == Calculator::none)
            {
                number = line.substr(line.find_last_of("("));
                line = line.substr(0,line.find_last_of("("));
            }
            else

            line = line.substr(0, line.find_last_of("+*-/^ ")+1);
    }
    else
        if (line.find("sqrt(") != std::string::npos)
        {
            was_root = true;
            line_pow = line;
            line = "";
        }
        else
        {

            if (calc.back().operation == Calculator::none && line.find("(") != std::string::npos)
            {
                number = line.substr(line.find_last_of("("));
                line = line.substr(0,line.find_last_of("("));
            }

            line = "";
        }

    if (calc.size() >= 2)
        if (calc[calc.size()-2].operation == Calculator::fact_)
        {
            do_last_operation();
        }

    calc.back().operation = Calculator::root_;

    calc.back().second_int = 2;
    calc.back().second = Calculator::int_;

    make_it();

    set_first_by_result();

    ui->lineEdit->setText(QString::fromStdString(line) + "sqrt(");

    if (was_pow || was_root)
        ui->lineEdit->setText(ui->lineEdit->text() + QString::fromStdString(line_pow));
    else
        ui->lineEdit->setText(ui->lineEdit->text() + QString::fromStdString(number));

    ui->lineEdit->setText(ui->lineEdit->text() + ")");
}

/**
 * Funkce zajistujici kliknuti na x!.
 */
void MainWindow::on_faktorial_clicked()
{
    if (!system_.at(2))
        return;

    prepare_display();

    std::string number = ui->lineEdit->text().toStdString();

    if (!isdigit(number.at(number.size()-1)) && calc.back().operation != Calculator::root_ && number.at(number.size()-1) != ')')
        return;

    if (number.at(number.size()-1) == ')')
    {
        if (calc.back().operation == Calculator::none && calc.back().first != Calculator::none_)
        {
            if (calc.back().first == Calculator::int_)
                number = std::to_string(calc.back().first_int);
            else
                number = std::to_string(calc.back().first_double);

            calc.back().first = Calculator::none_;
        }
        else
            number = number.substr(number.find_last_of("+*-/^ ")+1);
    }
    else
        if (number.find_last_of("+*-/^ ") != std::string::npos)
            number = number.substr(number.find_last_of("+*-/^ ")+1);

    if (calc.back().first == Calculator::none_)
        set_first(number);
    else
        if (calc.back().second == Calculator::none_)
        {
            new_calculator();
            set_first(number);
        }

    calc.back().operation = Calculator::fact_;

    ui->lineEdit->setText(ui->lineEdit->text() + "!");
}


/**
 * Funkce pro vypocet posledni operace.
 */
void MainWindow::make_it()
{
    tecka = false;

    switch(calc.back().first)
    {
        case Calculator::int_:
            make_it_int();
        break;

        case Calculator::double_:
            make_it_double();
        break;

        default:
        break;
    }
}

/**
 * Funkce pro vypocet posledni operace na zaklade prvniho operandu - int.
 */
void MainWindow::make_it_int()
{
    switch(calc.back().second)
    {
        case Calculator::int_:
            do_example_int(calc.back().operation);
        break;

        case Calculator::double_:
            do_example_int_double(calc.back().operation);
        break;

        default:
            do_example_int(calc.back().operation);
        break;
    }
}

/**
 * Funkce pro vypocet posledni operace na zaklade prvniho operandu - double.
 *
 * Spusti vypocet operace po zjisteni typu druheho operandu.
 */
void MainWindow::make_it_double()
{
    switch(calc.back().second)
    {
        case Calculator::int_:
            do_example_double_int(calc.back().operation);
        break;

        case Calculator::double_:
            do_example_double(calc.back().operation);
        break;

        default:
            do_example_double(calc.back().operation);
        break;
    }
}

/**
 * Funkce pro vypocet operace celych cisel.
 *
 * @param op Operace, ktera se ma vypocitat.
 */
void MainWindow::do_example_int(Calculator::ops op)
{
    switch(op)
    {
        case Calculator::add:
            calc.back().addition(calc.back().first_int, calc.back().second_int);
        break;

        case Calculator::sub:
            calc.back().subtraction(calc.back().first_int, calc.back().second_int);
        break;

        case Calculator::mul:
            calc.back().multiplication(calc.back().first_int, calc.back().second_int);
        break;

        case Calculator::div:
            try
            {
                calc.back().division(calc.back().first_int, calc.back().second_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::pow_:
        case Calculator::ypow_:
            try
            {
                calc.back().power(calc.back().first_int, calc.back().second_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::root_:
        case Calculator::yroot:
            try
            {
                calc.back().root(calc.back().first_int, calc.back().second_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::fact_:
            try
            {
                calc.back().factorial(calc.back().first_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::bin_oct:
            try
            {
                calc.back().binary_to_octal(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::bin_dec:
            try
            {
                calc.back().binary_to_decimal(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::bin_hex:
            try
            {
                hex_result = calc.back().binary_to_hex(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::oct_bin:
            try
            {
                calc.back().octal_to_binary(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::oct_dec:
            try
            {
                calc.back().octal_to_decimal(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::oct_hex:
            try
            {
                hex_result = calc.back().octal_to_hex(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::dec_bin:
            try
            {
                calc.back().decimal_to_binary(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::dec_oct:
            try
            {
                calc.back().decimal_to_octal(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::dec_hex:
            try
            {
                hex_result = calc.back().decimal_to_hex(calc.back().first_int);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::hex_bin:
            try
            {
                calc.back().hex_to_binary(hex_result);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::hex_oct:
            try
            {
                calc.back().hex_to_octal(hex_result);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::hex_dec:
            try
            {
                calc.back().hex_to_decimal(hex_result);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        default:
            error("Nastala chyba pri operaci!");
        break;
    }
}

/**
 * Funkce pro vypocet operace celeho a desetinneho cisla.
 *
 * @param op Operace, ktera se ma vypocitat.
 */
void MainWindow::do_example_int_double(Calculator::ops op)
{
    switch(op)
    {
        case Calculator::add:
            calc.back().addition(calc.back().first_int, calc.back().second_double);
        break;

        case Calculator::sub:
            calc.back().subtraction(calc.back().first_int, calc.back().second_double);
        break;

        case Calculator::mul:
            calc.back().multiplication(calc.back().first_int, calc.back().second_double);
        break;

        case Calculator::div:
            try
            {
                calc.back().division(calc.back().first_int, calc.back().second_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::pow_:
        case Calculator::ypow_:
            try
            {
                calc.back().power(calc.back().first_int, calc.back().second_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::root_:
        case Calculator::yroot:
            try
            {
                calc.back().root(calc.back().first_int, calc.back().second_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        default:
            error("Nastala chyba pri operaci!");
        break;
    }
}

/**
 * Funkce pro vypocet operace desetinneho a celeho cisla.
 *
 * @param op Operace, ktera se ma vypocitat.
 */
void MainWindow::do_example_double_int(Calculator::ops op)
{
    switch(op)
    {
        case Calculator::add:
            calc.back().addition(calc.back().first_double, calc.back().second_int);
        break;

        case Calculator::sub:
            calc.back().subtraction(calc.back().first_double, calc.back().second_int);
        break;

        case Calculator::mul:
            calc.back().multiplication(calc.back().first_double, calc.back().second_int);
        break;

        case Calculator::div:
            try
            {
                calc.back().division(calc.back().first_double, calc.back().second_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::pow_:
        case Calculator::ypow_:
            try
            {
                calc.back().power(calc.back().first_double, calc.back().second_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::root_:
        case Calculator::yroot:
            try
            {
                calc.back().root(calc.back().first_double, calc.back().second_int);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        default:
            error("Nastala chyba pri operaci!");
        break;
    }
}

/**
 * Funkce pro vypocet operace desetinnych cisel.
 *
 * @param op Operace, ktera se ma vypocitat.
 */
void MainWindow::do_example_double(Calculator::ops op)
{
    switch(op)
    {
        case Calculator::add:
            calc.back().addition(calc.back().first_double, calc.back().second_double);
        break;

        case Calculator::sub:
            calc.back().subtraction(calc.back().first_double, calc.back().second_double);
        break;

        case Calculator::mul:
            calc.back().multiplication(calc.back().first_double, calc.back().second_double);
        break;

        case Calculator::div:
            try
            {
                calc.back().division(calc.back().first_double, calc.back().second_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::pow_:
        case Calculator::ypow_:
            try
            {
                calc.back().power(calc.back().first_double, calc.back().second_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::root_:
        case Calculator::yroot:
            try
            {
                calc.back().root(calc.back().first_double, calc.back().second_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::fact_:
            try
            {
                calc.back().factorial(calc.back().first_double);
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::bin_oct:
            try
            {
                calc.back().binary_to_octal(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::bin_dec:
            try
            {
                calc.back().binary_to_decimal(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::bin_hex:
            try
            {
                hex_result = calc.back().binary_to_hex(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::oct_bin:
            try
            {
                calc.back().octal_to_binary(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::oct_dec:
            try
            {
                calc.back().octal_to_decimal(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::oct_hex:
            try
            {
                hex_result = calc.back().octal_to_hex(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::dec_bin:
            try
            {
                calc.back().decimal_to_binary(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::dec_oct:
            try
            {
                calc.back().decimal_to_octal(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::dec_hex:
            try
            {
                hex_result = calc.back().decimal_to_hex(calc.back().first_double);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::hex_bin:
            try
            {
                calc.back().hex_to_binary(hex_result);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::hex_oct:
            try
            {
                calc.back().hex_to_octal(hex_result);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        case Calculator::hex_dec:
            try
            {
                calc.back().hex_to_decimal(hex_result);
                vysledek = true;
            }
            catch(std::runtime_error* e)
            {
                error(e->what());
            }
        break;

        default:
            error("Nastala chyba pri operaci!");
        break;
    }
}

/**
 * Funkce, ktera dokonci operaci +/-.
 *
 * @param last_result Vysledek posledni operace.
 * @param last Typ vysledku.
 */
void MainWindow::operation_add_sub(double last_result, Calculator::type last)
{
    if (calc.back().first == Calculator::none_)
        set_first_by_last(last_result, last);
    else
        if (calc.back().second == Calculator::none_)
            set_second_by_last(last_result, last);

    make_it();
}

/**
 * Funkce, ktera dokonci operaci * a /.
 *
 * @param last_result Vysledek posledni operace.
 * @param last Typ vysledku.
 */
void MainWindow::operation_mul_div(double last_result, Calculator::type last)
{
    if (calc.back().first == Calculator::none_)
        set_first_by_last(last_result, last);
    else
        if (calc.back().second == Calculator::none_ && last != Calculator::none_)
            set_second_by_last(last_result, last);
        else
            if (last == Calculator::none_)
            {
                std::string number = ui->lineEdit->text().toStdString();

                number = number.substr(number.find_last_of("+*-/^") + 1);

                set_second(number);
            }

    make_it();
}

/**
 * Funkce, ktera dokonci operaci x^y a x yroot z.
 *
 * @param last_result Vysledek posledni operace.
 * @param last Typ vysledku.
 */
void MainWindow::operation_ypow_yroot(double last_result, Calculator::type last)
{
    if (calc.back().second == Calculator::none_)
        set_second_by_last(last_result, last);
    else
        if (last == Calculator::none_)
        {
            std::string number = ui->lineEdit->text().toStdString();

            number = number.substr(number.find_last_of("+*-/^") + 1);

            set_second(number);
        }

    make_it();
}

/**
 * Funkce, ktera nastavi vysledek posledni operace.
 *
 * @param last_result Odkaz na vysledek, ktery bude zaznamenan.
 * @param last Typ vysledku.
 */
void MainWindow::set_result(double *last_result, Calculator::type *last)
{
    *last_result = calc.back().get_result();

    if ((*last_result - (int)*last_result) != 0.0)
        *last = Calculator::double_;
    else
        *last = Calculator::int_;

    calc.pop_back();
}

/**
 * Funkce, ktera ziska konecny vysledek celeho prikladu.
 *
 * Postupne prochazi vsechny podpriklady a vypocita vysledky.
 *
 * @param bracket Flag, ktery je nastaven, kdyz neni funkce spustena operaci =.
 */
void MainWindow::on_rovnase_clicked(bool bracket = false)
{
    if (!system_.at(2))
        return;

    double last_result = 0.0;

    Calculator::type last = Calculator::none_;

    std::string line = ui->lineEdit->text().toStdString();

    if (line.find_last_of("+*-/^ ") == line.size()-1)
        return;

    if (!bracket)
        while (count_bracket)
        {
            ui->lineEdit->setText(ui->lineEdit->text() + ")");
            count_bracket--;
        }

    if(line.size() != 0)
    {
        if(line.at(line.size() - 1) != '=')
        {
            std::string last_res_s;

            while (!calc.empty())
                switch(calc.back().operation)
                {
                    case Calculator::none:
                        if (calc.back().first == Calculator::none_)
                        {
                            if (line.at(line.size()-1) == ')')
                                line = line.substr(0, line.size()-1);

                            last_res_s = line.substr(line.find_last_of("+-*/^") + (line.find_last_of("+-*/^") != 0));

                            if (last_res_s.find(".") != std::string::npos)
                                last = Calculator::double_;
                            else
                                last = Calculator::int_;

                            last_result = std::stod(last_res_s.c_str(), NULL);

                            calc.pop_back();
                        }
                        else
                        {
                            last = calc.back().first;

                            if (last == Calculator::int_)
                                last_result = calc.back().first_int;
                            else
                                last_result = calc.back().first_double;

                            calc.pop_back();
                        }
                    break;

                    case Calculator::add:
                        operation_add_sub(last_result, last);

                        set_result(&last_result, &last);
                    break;

                    case Calculator::sub:
                        operation_add_sub(last_result, last);

                        set_result(&last_result, &last);
                    break;

                    case Calculator::mul:
                        operation_mul_div(last_result, last);

                        set_result(&last_result, &last);
                    break;

                    case Calculator::div:
                        operation_mul_div(last_result, last);

                        if (vysledek)
                            return;

                        set_result(&last_result, &last);
                    break;

                    case Calculator::ypow_:
                        operation_ypow_yroot(last_result, last);

                        set_result(&last_result, &last);
                    break;

                    case Calculator::pow_:
                        set_result(&last_result, &last);
                    break;

                    case Calculator::root_:
                        set_result(&last_result, &last);
                    break;

                    case Calculator::yroot:
                        operation_ypow_yroot(last_result, last);

                        set_result(&last_result, &last);
                    break;

                    case Calculator::fact_:
                        make_it();

                        set_result(&last_result, &last);
                    break;

                    case Calculator::bracket:
                        set_first_by_last(last_result, last);
                        calc.back().second = Calculator::none_;
                        calc.back().operation = Calculator::none;
                        if (bracket)
                            return;
                    break;

                    default:
                        break;
                }

            ui->lineEdit->setText(ui->lineEdit->text() + "=");

            if (!vysledek)
            {
                if (last == Calculator::int_)
                    update_text(std::to_string((int)last_result));
                else
                {
                    std::ostringstream out;
                    out << std::setprecision(7)<< last_result;
                    update_text(out.str());
                }

                new_calculator();

                vysledek = true;
            }
        }
    }
}

/**
 * Funkce, ktera spusti novou kalkulacku.
 *
 * Vytvori novou instanci kalkulacky a vlozi ji do vektoru.
 */
void MainWindow::new_calculator()
{
    Calculator c = Calculator();

    calc.push_back(c);

    tecka = false;
}

/**
 * Funkce aktualizuje vysledkovy radek textem v parametru.
 *
 * Text je pred zapsanim upraven, jsou odseknuty prebytecne nuly.
 * V pripade cisla x.0 je odseknuta i tecka, vysledek je pak cele cislo.
 *
 * @param text Text, kterym bude radek aktualizovan.
 */
void MainWindow::update_text(std::string text)
{
    if (text.find(".") != std::string::npos)
    {
        int size = text.size();

        while(--size != 0)
            if (text.at(size) != '0')
                break;

        text = text.substr(0, size + 1);

        if (text.substr(text.find(".")+1).size() == 0)
            text = text.substr(0, text.find("."));

        tecka = false;
    }

    ui->label->setText(QString::fromStdString(text));
}

/**
 * Funkce, ktera vypise chybovou hlasku.
 *
 * @param err Chybova hlaska, ktera bude vypsana.
 */
void MainWindow::error(std::string err)
{
    ui->label->setText(QString::fromStdString(err));
    ui->lineEdit->setText("");
    vysledek = true;
    calc.clear();
    new_calculator();
}
