
package board;

/**
 * Rozhraní reprezentující pravidla inicializace hry.
 * @author Ondrej Janota - xjanot02
 */
public interface Rules {
    
    /**
     * Vytvoří odpovídající pole na zadaných indexech.
     * @param row Řádek desky.
     * @param col Sloupec desky.
     * @return Vytvořené pole.
     */
    Field createField(int row, int col);
    
    /**
     * Vrací velikost desky.
     * @return Velikost desky.
     */
    int getSize();
}
