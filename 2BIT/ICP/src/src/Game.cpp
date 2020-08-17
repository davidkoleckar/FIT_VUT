#include "../include/Game.h"


using namespace std;
/**
 * Reprezentuje hru. Při inicializaci se vždy přiřadí hrací deska.
 * @author Ondrej Janota - xjanot02
 * @author David Koleckar - xkolec07
 */

/**
* Inicializuje hru.
* @param board Hrací deska, která je součástí hry.
* @param isComputer
* @param alg
*/
Game::Game(Board *new_board, bool isComp, int ai)
{
    //ctor
    board = new_board;
    player[0] = nullptr;
    player[1] = nullptr;

    current = false;

    disksCount[0] = 0;
    disksCount[1] = 0;

    algorithm = ai;
    isAI = isComp;
    isEnd = false;
}
    
/**
 * Přidá hráče a současně vyvolá jeho inicializaci.
 * Pokud hráč dané barvy již existuje, nedělá nic a vrací false.
 * @param player Přidávaný hráč.
 * @return Úspěch akce.
 */
bool Game::addPlayer(Player *new_player)
{
    if (player[0] == nullptr)
    {
        if (!new_player->is_white())
        {
            player[0] = new_player;
            player[0]->initialize(board);
            disksCount[0] = player[0]->getCount();
        }
        else
            if (player[1] == nullptr || isAI)
            {
                player[1] = new_player;
                player[1]->initialize(board);
                disksCount[1] = player[1]->getCount();
            }
            else
                return false;
    }
    else
        if (new_player->is_white() || isAI)
        {
            if (player[1] == nullptr)
            {
                player[1] = new_player;
                player[1]->initialize(board);
                disksCount[1] = player[1]->getCount();
            }
            else
                return false;
        }
        else
            return false;

    return true;
}
/**
 * Vrátí aktuálního hráče, který je na tahu.
 * @return Aktuální hráč.
 */
Player *Game::currentPlayer()
{
    return current ? player[1] : player[0];
}
/**
 * Změní aktuálního hráče.
 * @return Aktuálně nastavený hráč.
 */
Player *Game::nextPlayer()
{
    if (current)
    {
        current = !current;
        if (disksCount[1] != player[1]->getCount())
        {
            player[1]->removeRedo();
            player[0]->removeRedo();
            disksCount[0] -= player[1]->getCount() - disksCount[1] - 1;
            player[0]->disks = disksCount[0];
            disksCount[1] = player[1]->getCount();
        }
        return player[0];
    }
    else
    {
        current = !current;
        if (disksCount[0] != player[0]->getCount())
        {
            player[1]->removeRedo();
            player[0]->removeRedo();
            disksCount[1] -= player[0]->getCount() - disksCount[0] - 1;
            player[1]->disks = disksCount[1];
            disksCount[0] = player[0]->getCount();
        }
        return player[1];
    }
}
/**
 * Vrati hraci desku.
 * @return Hraci deska.
 */
Board *Game::getBoard()
{
    return board;
}
/**
 * Umela inteligence pro automaticky tah, pri hre proti PC.
 * Prvni algoritmus na prvni moznou pozici vloží disk.
 * Druhy algoritmus vybere náhodnou moznou pozici, na kterou muze vlozit disk.
 */
void Game::AImoves()
{
    switch(algorithm)
    {
        case 1:
        {
            for (int i = 1; i <= board->getRules()->getSize(); i++)
                for (int j = 1; j <= board->getRules()->getSize(); j++)
                    if (currentPlayer()->putDisk(board->getField(i, j)))
                        return;
            break;
        }
        case 2:
        {
            vector<pair<int,int>> positions;
            if (currentPlayer()->checkMoves())
            {
                for (int i = 1; i <= board->getRules()->getSize(); i++)
                    for (int j = 1; j <= board->getRules()->getSize(); j++)
                        if (currentPlayer()->canPutDisk(board->getField(i, j)))
                            positions.push_back(make_pair(i, j));
                int index = rand() % positions.size();
                currentPlayer()->putDisk(board->getField(positions[index].first, positions[index].second));
                return;
            }
            break;
        }
        //case 3: pripadne dalsi algoritmus
        default:
            cerr << "SOMETHING WENT WRONG" << endl;
            break;
    }
}


/**
 * ulozeni hry
 * @param name Nazev hry
 */
void Game::saveGame(string name)
{
    ofstream file;

    ostringstream outData;

    file.open(("examples/"+name+".txt").c_str(), ios::out);

    if (file.is_open())
    {
        outData << board->getRules()->getSize() << endl;
        outData << algorithm << endl;
        outData << isAI << endl;
        outData << currentPlayer()->is_white() << endl;
        outData << player[0]->getCount() << endl;

        vector<pair<int,int>> positions;

        getDisks(false, &positions);

        for (int i = 0; i < player[0]->getCount(); i++)
            outData << positions[i].first << "," << positions[i].second << endl;

        positions.clear();

        outData << player[1]->getCount() << endl;

        getDisks(true, &positions);

        for (int i = 0; i < player[1]->getCount(); i++)
            outData << positions[i].first << "," << positions[i].second << endl;

        vector<pair<Field*, vector<Field*>>> undo = currentPlayer()->getUndo();

        int undoIndex = currentPlayer()->getIndex();

        outData << undoIndex << endl;

        outData << undo.size() << endl;

        for (unsigned int i = 0; i < undo.size(); i++)
        {
            if (undo[i].first != nullptr)
            {
                outData << ((BoardField*)undo[i].first)->getPosition().first << ",";
                outData << ((BoardField*)undo[i].first)->getPosition().second << endl;
                outData << ((vector<Field *>)undo[i].second).size() << endl;

                for (unsigned int j = 0; j < ((vector<Field *>)undo[i].second).size(); j++)
                {
                    outData << ((BoardField *)((vector<Field *>)undo[i].second)[j])->getPosition().first << ",";
                    outData << ((BoardField *)((vector<Field *>)undo[i].second)[j])->getPosition().second << endl;
                }
            }
            else
                outData << 0 << "," << 0 << endl;
        }

        undo = getNextPlayer()->getUndo();

        undoIndex = getNextPlayer()->getIndex();

        outData << undoIndex << endl;

        outData << undo.size() << endl;

        for (unsigned int i = 0; i < undo.size(); i++)
        {
            if (undo[i].first != nullptr)
            {
                outData << ((BoardField*)undo[i].first)->getPosition().first << ",";
                outData << ((BoardField*)undo[i].first)->getPosition().second << endl;
                
                outData << ((vector<Field *>)undo[i].second).size() << endl;

                for (unsigned int j = 0; j < ((vector<Field *>)undo[i].second).size(); j++)
                {
                    outData << ((BoardField *)((vector<Field *>)undo[i].second)[j])->getPosition().first << ",";
                    outData << ((BoardField *)((vector<Field *>)undo[i].second)[j])->getPosition().second << endl;
                }
            }
            else
                outData << 0 << "," << 0 << endl;
        }


        file << outData.str();

        file.close();
    }
}

void Game::getDisks(bool isWhite, vector<pair<int,int>> *positions)
{
    //vector<pair<int,int>> positions;

    for (int i = 1; i <= board->getRules()->getSize(); i++)
        for (int j = 1; j <= board->getRules()->getSize(); j++)
            if (board->getField(i, j)->getDisk() != nullptr)
                if (board->getField(i, j)->getDisk()->is_white() == isWhite)
                    positions->push_back(make_pair(i,j));
}
/**
 * @return Vrací true pokud je konec hry.
 */    
bool Game::getEnd()
{
    return isEnd;
}
/**
 * Zaznamená konec hry.
 */ 
void Game::setEnd()
{
    isEnd = true;
}
/**
 * Nastaví hrači počet disku. 
 * @param isWhite barva hráče.
 * @param count počet disku
 */
void Game::setDisks(bool isWhite, int count){
    player[isWhite]->disks = count;
    disksCount[isWhite] = count;
}
    
/**
 * @return Vrací počet disků bílého hráče
 */
int Game::getDisksWhite()
{
    return disksCount[1];
}
/**
 * @return Vrací počet disků černého hráče
 */ 
int Game::getDisksBlack()
{
    return disksCount[0];
}
/**
 * @return Vrátí dalšího hráče na tahu.
 */
Player *Game::getNextPlayer()
{
    return player[!current];
}
/**
 * Nastaví aktualního hráče. 
 * @param isWhite barva hráče.
 */
void Game::setCurrent(bool isWhite)
{
    current = isWhite;
}
