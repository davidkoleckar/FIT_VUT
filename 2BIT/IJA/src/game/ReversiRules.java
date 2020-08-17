
package game;
import board.BoardField;
import board.Field;
import board.Rules;

/**
 * Třída implementující rozhraní Rules.
 * Reprezentuje pravidla inicializace hry Reversi.
 * @author Ondrej Janota - xjanot02
 */
public class ReversiRules extends java.lang.Object implements Rules{
    
    private final int size;
    
    /**
     * Inicializace pravidel.
     * @param size Velikost hrací desky.
     */
    public ReversiRules(int size){
        this.size = size;
    }
	
    /**
     * Vytvoří field na daných souřadnicích
     * @param row Řádek
     * @param col Sloupec
	 * @return Vytvořený field.
     */    
    @Override
    public Field createField(int row, int col){
        Field temp;
        temp = new BoardField(row, col);
        return temp;
    }
	
    /**
	 * @return Vrací velikost desky.
     */     
    @Override
    public int getSize(){
        return this.size;
    }
}
