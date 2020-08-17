#include "../include/newGame.h"

using namespace std;
/**
 * Třída reprezentuje novou hru. Vytvoří board, players, rules a game.
 * @author Ondrej Janota - xjanot02
 */

 /**
 * Konstruktor newGame
 * @param size
 * @param is_AI
 * @param new_algorithm
 * @param load
 */
newGame::newGame(int new_size, bool is_AI, int new_algorithm, pair<bool, string> load)
{
    //ctor
    if (load.first)
    {
        loadGame(load.second);
    }
    else
    {
        Rules *r = new ReversiRules(new_size);
        Board *b = new Board(r);
        game = new Game(b, is_AI, new_algorithm);

        game->addPlayer(new Player(false));
        game->addPlayer(new Player(true));
    }
}

/**
 * Nacteni hry
 * @param name nazev hry.
 */   
void newGame::loadGame(string name)
{
    ifstream file;

    string f;

    file.open(("examples/"+name+".txt").c_str(), ios::in);

    if (file.is_open())
    {
        vector<string> line;

        while(!file.eof())
        {
            getline(file, f);

            line.push_back(f);

            f.erase();
        }

        int index = 0;

        int size = atoi(line[index++].c_str());

        Rules *r = (Rules *) new ReversiRules(size);
        Board *b = new Board(r);

        int algorithm = atoi(line[index++].c_str());

        bool isAI = atoi(line[index++].c_str());

        game = new Game(b, isAI, algorithm);

        game->addPlayer(new Player(false));
        game->addPlayer(new Player(true));

        game->setCurrent(atoi(line[index++].c_str()));

        game->setDisks(false, atoi(line[index++].c_str()));

        int posI;
        int posJ;
        
        for (int i = 0; i < game->getDisksBlack(); i++)
        {
            f = line[index++];

            posI = atoi(f.substr(0, f.find(",")).c_str());
            posJ = atoi(f.substr(f.find(",")+1).c_str());


            if ((posI == 4 && posJ == 5) || (posI == 5 && posJ == 4))
                game->getBoard()->getField(posI, posJ)->getDisk()->turn();
            else
                game->getBoard()->getField(posI, posJ)->putDisk(new Disk(false));
        }

        game->setDisks(true, atoi(line[index++].c_str()));

        for (int i = 0; i < game->getDisksWhite(); i++)
        {
            f.erase();
            f = line[index++];

            posI = atoi(f.substr(0, f.find(",")).c_str());
            posJ = atoi(f.substr(f.find(",")+1).c_str());

            if ((posI == 4 && posJ == 4) || (posI == 5 && posJ == 5))
                game->getBoard()->getField(posI, posJ)->getDisk()->turn();
            else
                game->getBoard()->getField(posI, posJ)->putDisk(new Disk(true));
        }

        vector<pair<Field*, vector<Field*>>> undo;
        vector<Field*> turn;

        game->currentPlayer()->setIndex(atoi(line[index++].c_str()));

        int undoSize = atoi(line[index++].c_str());

        undo.resize(undoSize);
        undo.clear();

        for (int i = 0; i < undoSize; i++)
        {
            f = line[index++];

            if (f != "0,0")
            {
                posI = atoi(f.substr(0, f.find(",")).c_str());
                posJ = atoi(f.substr(f.find(",")+1).c_str());

                int turnSize = atoi(line[index++].c_str());

                turn.resize(turnSize);
                turn.clear();

                for (int j = 0; j < turnSize; j++)
                {
                    f = line[index++];

                    int tPosI = atoi(f.substr(0, f.find(",")).c_str());
                    int tPosJ = atoi(f.substr(f.find(",")+1).c_str());
                    turn.push_back(game->getBoard()->getField(tPosI, tPosJ));
                }

                undo.push_back(make_pair(game->getBoard()->getField(posI, posJ), turn));
            }
            else
                undo.push_back(make_pair(nullptr, turn));
        }

        if (!undo.empty())
            game->currentPlayer()->setUndo(undo);

        game->getNextPlayer()->setIndex(atoi(line[index++].c_str()));

        undoSize = atoi(line[index++].c_str());

        undo.resize(undoSize);
        undo.clear();

        for (int i = 0; i < undoSize; i++)
        {
            f = line[index++];
            if (f != "0,0")
            {
                posI = atoi(f.substr(0, f.find(",")).c_str());
                posJ = atoi(f.substr(f.find(",")+1).c_str());

                int turnSize = atoi(line[index++].c_str());

                turn.resize(turnSize);
                turn.clear();

                for (int j = 0; j < turnSize; j++)
                {
                    f = line[index++];

                    int tPosI = atoi(f.substr(0, f.find(",")).c_str());
                    int tPosJ = atoi(f.substr(f.find(",")+1).c_str());
                    turn.push_back(game->getBoard()->getField(tPosI, tPosJ));
                }

                undo.push_back(make_pair(game->getBoard()->getField(posI, posJ), turn));
            }
            else
                undo.push_back(make_pair(nullptr, turn));
        }

        if (undo.size() > 0)
            game->getNextPlayer()->setUndo(undo);
    }
}
/**
 * Uložení hry
 * @param name nazev hry.
 */   
void newGame::saveGame(string name)
{
    game->saveGame(name);
}

/**
 * Undo
 */ 
void newGame::doUndo()
{
    bool isNull = false;

    pair<Field *, vector<Field *>> *lastMove = game->getNextPlayer()->getUndoMove();

    if (lastMove != nullptr)
    {
        Field *remove = lastMove->first;

        if (remove != nullptr)
        {
            ((BoardField *)remove)->removeDisk();
            game->setDisks(game->getNextPlayer()->is_white(), game->getNextPlayer()->getCount() - 1);

            vector<Field *> turn = lastMove->second;

            for(auto& field : turn)
            {
                field->getDisk()->turn();
                game->setDisks(game->getNextPlayer()->is_white(), game->getNextPlayer()->getCount() - 1);
                game->setDisks(game->currentPlayer()->is_white(), game->currentPlayer()->getCount() + 1);
            }
        }
    }
    else
        isNull = true;

    pair<Field *, vector<Field *>> *currentUndo = game->currentPlayer()->getUndoMove();

    if (currentUndo != nullptr)
    {
        Field *remove = currentUndo->first;

        if (remove != nullptr)
        {
            ((BoardField *)remove)->removeDisk();
            game->setDisks(game->currentPlayer()->is_white(), game->currentPlayer()->getCount() - 1);

            vector<Field *> turn = currentUndo->second;

            for(auto& field : turn)
            {
                field->getDisk()->turn();
                game->setDisks(game->getNextPlayer()->is_white(), game->getNextPlayer()->getCount() + 1);
                game->setDisks(game->currentPlayer()->is_white(), game->currentPlayer()->getCount() - 1);
            }
        }
    }
    else
        isNull = !isNull;

    if (isNull)
        game->nextPlayer();
}
/**
 * Redo
 */ 
void newGame::doRedo()
{
    bool isNull = true;

    pair<Field *, vector<Field *>> *lastMove = game->currentPlayer()->getRedoMove();

    if (lastMove != nullptr)
    {
        Field *put = lastMove->first;

        if (put != nullptr)
        {
            put->putDisk(new Disk(game->currentPlayer()->is_white()));
            game->setDisks(game->currentPlayer()->is_white(), game->currentPlayer()->getCount() + 1);

            vector<Field *> turn = lastMove->second;

            for(auto& field : turn)
            {
                field->getDisk()->turn();
                game->setDisks(game->getNextPlayer()->is_white(), game->getNextPlayer()->getCount() - 1);
                game->setDisks(game->currentPlayer()->is_white(), game->currentPlayer()->getCount() + 1);
            }
        }
    }
    else
        isNull = true;


    lastMove = game->getNextPlayer()->getRedoMove();

    if (lastMove != nullptr)
    {
        Field *put = lastMove->first;

        if (put != nullptr)
        {
            put->putDisk(new Disk(game->getNextPlayer()->is_white()));
            game->setDisks(game->getNextPlayer()->is_white(), game->getNextPlayer()->getCount() + 1);

            vector<Field *> turn = lastMove->second;

            for(auto& field : turn)
            {
                field->getDisk()->turn();
                game->setDisks(game->getNextPlayer()->is_white(), game->getNextPlayer()->getCount() + 1);
                game->setDisks(game->currentPlayer()->is_white(), game->currentPlayer()->getCount() - 1);
            }
        }
    }
    else
        isNull = !isNull;

    if (isNull)
        game->nextPlayer();
}
