#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>

#include <boost/filesystem.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include "include/newGame.h"
#include "include/Board.h"
/**
 * Trida cli - textové rozhraní hry
 *
 * @author	David Kolečkář - xkolec07
 * @author	Ondřej Janota - xjanot02
 *
 */

using namespace std;
using namespace boost::filesystem;

void introduction();

void drawText(Game *game)
{
    cout << "Current player: " << (game->currentPlayer()->is_white() ? (game->isAI ? "COMPUTER (O)" : "WHITE (O)") : "BLACK (X)") << endl;
    cout << endl << "Black (X) disks count: " << game->getDisksBlack() << endl;
    cout << endl << "White (O) disks count: " << game->getDisksWhite() << endl;

}


void drawBoard(Game *game)
{
    int row = 0;
    int col = 0;
    char col_name = 'a';

    cout << endl << string(4, ' ') << col_name;

    for (col = 1;col < game->getBoard()->getRules()->getSize(); col++)
        cout << string(5, ' ') << (char)(col_name + col);

    cout << endl;

    for (row = 0; row < game->getBoard()->getRules()->getSize(); row++)
    {
        cout << "  +";

        for (col = 0; col < game->getBoard()->getRules()->getSize(); col++)
            cout << "-----+";

        cout << endl << "  |";

        for (int i = 0; i < game->getBoard()->getRules()->getSize(); i++)
            cout << "     |";

        cout << endl << row+1 << ((row >= 9) ? "|" : " |");

        for (col = 0; col < game->getBoard()->getRules()->getSize(); col++)
            if (game->getBoard()->getField(row+1, col+1)->getDisk() != nullptr)
            {
                if (game->getBoard()->getField(row+1, col+1)->getDisk()->is_white())
                    cout << "  O  |";
                else
                    cout << "  X  |";
            }
            else
                if (game->currentPlayer()->canPutDisk(game->getBoard()->getField(row + 1, col + 1)))
                    cout << "  _  |";
                else
                    cout << "     |";

        cout << endl;

        cout << "  |";

        for (int i = 0; i < game->getBoard()->getRules()->getSize(); i++)
            cout << "     |";

        cout << endl;
    }

    cout << "  +";

    for (col = 0; col < game->getBoard()->getRules()->getSize(); col++)
        cout << "-----+";

    cout << endl;
}

void play(newGame *game)
{
    int posI = 0;
    char posJ = 0;
    while(!game->game->getEnd())
    {
        if (!game->game->currentPlayer()->checkMoves())
        {
            if(!game->game->getNextPlayer()->checkMoves())
            {
                cout << endl << endl << "End of the game" << endl;
                if (game->game->getDisksBlack() > game->game->getDisksWhite())
                {
                    cout << "BLACK won!" << endl;
                    cout << "SCORE IS ";
                    if (game->game->getDisksBlack() + game->game->getDisksWhite() < game->game->getBoard()->getRules()->getSize()*game->game->getBoard()->getRules()->getSize())
                        cout << game->game->getBoard()->getRules()->getSize() - game->game->getDisksWhite();
                    else
                        cout << game->game->getDisksBlack() << " : " << game->game->getDisksWhite() << endl;
                }
                else
                    if (game->game->getDisksBlack() < game->game->getDisksWhite())
                    {
                        cout << "WHITE won!" << endl;
                        cout << "SCORE IS ";
                        if (game->game->getDisksBlack() + game->game->getDisksWhite() < game->game->getBoard()->getRules()->getSize()*game->game->getBoard()->getRules()->getSize())
                            cout << game->game->getBoard()->getRules()->getSize() - game->game->getDisksBlack();
                        else
                            cout << "BLACK " << game->game->getDisksBlack() << " : " << game->game->getDisksWhite() << " WHITE" << endl;
                    }
                    else
                        cout << "DRAW!" << endl;

                cout << endl << "Play new game?" << endl;
                do
                {
                    cout << "[y/n]: ";
                    cin.clear();
                    cin >> posJ;

                    if (posJ == 'y')
                    {
                        delete[] game;
                        introduction();
                    }
                    else
                        if (posJ == 'n')
                        {
                            delete[] game;
                            return;
                        }
                }
                while(true);
            }

            cout << endl << "Player cannot move" << endl << endl;
            game->game->nextPlayer();
        }

        if (game->game->isAI && game->game->currentPlayer()->is_white())
        {
            drawBoard(game->game);
            drawText(game->game);
            game->game->AImoves();
            boost::this_thread::sleep(boost::posix_time::milliseconds(2000));
            game->game->nextPlayer();
        }

        if (!game->game->currentPlayer()->checkMoves())
        {
            if(!game->game->getNextPlayer()->checkMoves())
            {
                cout << endl << endl << "End of the game" << endl;

                drawBoard(game->game);

                if (game->game->getDisksBlack() > game->game->getDisksWhite())
                {
                    cout << "BLACK won!" << endl;
                    cout << "SCORE IS ";
                    if (game->game->getDisksBlack() + game->game->getDisksWhite() < game->game->getBoard()->getRules()->getSize()*game->game->getBoard()->getRules()->getSize())
                        cout << game->game->getBoard()->getRules()->getSize() - game->game->getDisksWhite();
                    else
                        cout << game->game->getDisksBlack() << " : " << game->game->getDisksWhite() << endl;
                }
                else
                    if (game->game->getDisksBlack() < game->game->getDisksWhite())
                    {
                        cout << "WHITE won!" << endl;
                        cout << "SCORE IS ";
                        if (game->game->getDisksBlack() + game->game->getDisksWhite() < game->game->getBoard()->getRules()->getSize()*game->game->getBoard()->getRules()->getSize())
                            cout << game->game->getBoard()->getRules()->getSize() - game->game->getDisksBlack();
                        else
                            cout << "BLACK " << game->game->getDisksBlack() << " : " << game->game->getDisksWhite() << " WHITE" << endl;
                    }
                    else
                        cout << "DRAW!" << endl;

                cout << endl << "Play new game?" << endl;
                do
                {
                    cout << "[y/n]: ";
                    cin.clear();
                    cin >> posJ;

                    if (posJ == 'y')
                    {
                        delete[] game;
                        introduction();
                    }
                    else
                        if (posJ == 'n')
                        {
                            delete[] game;
                            return;
                        }
                }
                while(true);
            }

            cout << endl << "Player cannot move" << endl << endl;
            game->game->nextPlayer();
        }

        if (!game->game->isAI || !game->game->currentPlayer()->is_white())
        {
            drawBoard(game->game);
            drawText(game->game);
            do
            {
                do
                {
                    cout << endl << "Play, Undo, Redo, Save, End" << endl;
                    cout << endl << "Choose one option [p,u,r,s,e]: ";
                    cin.clear();
                    cin >> posJ;

                    switch(posJ)
                    {
                        case 'p':
                            break;
                        case 'u':
                            {
                                game->doUndo();
                                drawBoard(game->game);
                                drawText(game->game);
                                continue;
                            }
                        case 'r':
                            {
                                game->doRedo();
                                drawBoard(game->game);
                                drawText(game->game);
                                continue;
                            }
                        case 's':
                            {
                                cout << endl << "Name of game: ";
                                fflush(stdin);
                                string save;
                                cin >> save;
                                game->saveGame(save);
                                continue;
                            }
                        case 'e':
                            {
                                cout << endl << endl << "End of the game" << endl;
                                cout << "Play new game?" << endl;
                                do
                                {
                                    cout << "[y/n]: ";
                                    fflush(stdin);
                                    scanf("%c", &posJ);

                                    if (posJ == 'y')
                                    {
                                        delete[] game;
                                        introduction();
                                        return;
                                    }
                                    else
                                        if (posJ == 'n')
                                        {
                                            delete[] game;
                                            return;
                                        }
                                }
                                while(true);

                                delete[] game;
                                return;
                            }
                        default:
                            {
                                cout << endl << "BAD INPUT!";
                                break;
                            }
                    }
                    if (posJ == 'p')
                        break;

                }
                while(true);
                cout << endl << "Choose position" << endl;
                cout << endl << "Choose row 1 - " << game->game->getBoard()->getRules()->getSize() << ": ";
                fflush(stdin);
                cin.clear();
                scanf("%d", &posI);
                if (posI < 1 || posI > game->game->getBoard()->getRules()->getSize())
                {
                    cout << endl << "Not valid position" << endl;
                    continue;
                }

                cout << "Choose column a - " << (char)('a' - 1 + game->game->getBoard()->getRules()->getSize()) << ": ";
                fflush(stdin);
                cin.clear();
                fflush(stdout);
                cin >> posJ;
                //scanf("%c", &posJ);

                posJ = tolower(posJ) - 'a' + 1;


                cout << posJ << " positionJ" << endl;
                if (posJ < 1 || posJ > game->game->getBoard()->getRules()->getSize())
                {
                    cout << endl << "Not valid position" << endl << endl;
                    continue;
                }
                if (game->game->currentPlayer()->putDisk(game->game->getBoard()->getField(posI, posJ)))
                    break;
                else
                    cout << endl << "You cannot put disk here" << endl << endl;
            }
            while(true);
            game->game->nextPlayer();
        }
    }
}


newGame *algorithm(int size)
{
    string in;
    cout << endl << "Set the algorithm" << endl;
    cout << endl << "1 - First move choosing" << endl;
    cout << "2 - Random move choosing" << endl;
    cout << endl << "Choose: ";

    fflush(stdin);
    getline(cin, in);
    int a = atoi(in.c_str());

    switch(a)
    {
        case 1:
        case 2:
        {
            newGame *g = new newGame(size, true, a, make_pair(false, ""));
            return g;
        }
        //case 3:
            //other algorithm
        default:
        {
            cout << "Bad choise" << endl << endl;
            return algorithm(size);
        }
    }
}

void computer()
{
    cout.flush();
    string in;
    char *ptr[255];
    cout << endl << "Player vs. Computer" << endl;
    cout << endl << "Size of board: ";
    fflush(stdin);
    getline(cin, in);
    unsigned int a = strtoul(in.c_str(), ptr, 10);

    if (a == 6 || a == 8 || a == 10 || a == 12)
        play(algorithm(a));
    else
        if (*ptr[0] == '\0')
            play(algorithm(8));
        else
            computer();
}

void player()
{
    cout.flush();
    string in;
    char *ptr[255];
    cout << endl << "Player vs. Player" << endl;
    cout << endl << "Size of board: ";
    fflush(stdin);
    getline(cin, in);
    unsigned int a = strtoul(in.c_str(), ptr, 10);

    if (a == 6 || a == 8 || a == 10 || a == 12)
        play(new newGame(a, false, 0, make_pair(false, "")));
    else
        player();
}

void load()
{
    cout << "You can load these games" << endl;
    path p("examples");
    vector<string> files;

    int j = 0;

    for (auto i = directory_iterator(p); i != directory_iterator(); i++)
    {
        if (!is_directory(i->path())) //we eliminate directories
        {
            cout << j++ << " - " << i->path().filename().string().substr(0, i->path().filename().string().find(".")) << endl;
            files.push_back(i->path().filename().string().substr(0, i->path().filename().string().find(".")));
        }
        else
            continue;
    }
    unsigned int a;

    while(true)
    {
        cout << endl << endl << "Choose game [1, 2, ...] ";

        cin >> a;

        for (unsigned int i = 0; i < files.size(); i++)
        {
            
            if (a < files.size())
            {
                play(new newGame(0, 0, 0, make_pair(true, files[a])));
                return;
            }
            else
                cout << endl << endl << "FILE DOES NOT EXIST" << endl;
        }
    }
}

void introduction()
{
    string in;
    cout << "Othello - choose the option" << endl;
    cout << "1 - Player vs. Computer" << endl;
    cout << "2 - Player vs. Player" << endl;
    cout << "3 - Load game - load" << endl;
    cout << endl << "Choose: ";

    fflush(stdin);
    getline(cin, in);
    int a = atoi(in.c_str());
    switch(a)
    {
        case 1:
            cout << flush;
            computer();
            return;
        case 2:
            player();
            return;
        case 3:
            load();
            return;
        default:
            cout << "Bad choise" << endl << endl;
            introduction();
    }
}

int main()
{
    introduction();


    return 0;
}
