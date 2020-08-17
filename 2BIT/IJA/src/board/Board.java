package board;

/**
 * Třída reprezentující hrací desku.
 * Deska má velikost (rozměr) N a rozlišuje aktivní a neaktivní pole.
 * Aktivní pole jsou umístěna na řádcích a sloupcích, 
 * které jsou číslovány od 1 do N. 
 * Neaktivní (okrajová) pole jsou na sloupcích a řádcích 
 * indexovaných hodnotami 0 a N+1.
 * @author Ondrej Janota - xjanot02
 */
public class Board extends java.lang.Object{

    private final Field[][] field;
    private final Rules rules;
    /**
     * Inicializuje desku. 
     * Vytvoří a uloží si všechna pole. 
     * Pro všechna pole nastaví jejich okolí. 
     * Na neaktivních polích jsou umístěny instance třídy BorderField.
     * Objekty aktivních polí jsou získány metodou Rules.createField(int, int).
     * @param rules Objekt reprezentující pravidla inicializace hry.
     */
    public Board(Rules rules) {
        this.rules = rules;
        this.field = new Field[rules.getSize()+2][rules.getSize()+2];
        
        for(int i = 1; i < rules.getSize() + 1; i++)
            for(int j = 1; j < rules.getSize() + 1; j++)
                this.field[i][j] = rules.createField(i, j);
        
        for(int i = 0; i <= rules.getSize() + 1; i += rules.getSize() + 1)
            for(int j = 0; j <= rules.getSize() + 1; j++)
                this.field[i][j] = (Field)new BorderField();
        
        for(int j = 0; j <= rules.getSize() + 1;  j += rules.getSize() + 1)
            for(int i = 0; i <= rules.getSize() + 1; i++)
                this.field[i][j] = (Field)new BorderField();
        
        for(int i = 1; i <= rules.getSize(); i++)
            for(int j = 1; j <= rules.getSize(); j++)
                this.okoli(i, j);
    }
    
    /**
     * Uložení si okolních polí.
     * @param row Řádek
     * @param col Sloupec
     */
    private void okoli(int row, int col){
        this.field[row][col].addNextField(Field.Direction.L, this.field[row][col-1]);
        this.field[row][col].addNextField(Field.Direction.LU, this.field[row-1][col-1]);
        this.field[row][col].addNextField(Field.Direction.U, this.field[row-1][col]);
        this.field[row][col].addNextField(Field.Direction.RU, this.field[row-1][col+1]);
        this.field[row][col].addNextField(Field.Direction.R, this.field[row][col+1]);
        this.field[row][col].addNextField(Field.Direction.RD, this.field[row+1][col+1]);
        this.field[row][col].addNextField(Field.Direction.D, this.field[row+1][col]);
        this.field[row][col].addNextField(Field.Direction.LD, this.field[row+1][col-1]);
    }
    /**
     * Vrátí pole umístěné na uvedených indexech row a col.
     * @param row Řádek
     * @param col Sloupec
     * @return Pole na pozici [row][col].
     */
    public Field getField(int row, int col){
        return this.field[row][col];
    }
    
    /**
     * Vrací objekt pravidel.
     * @return Pravidla inicializace hry.
     */
    public Rules getRules(){
        return this.rules;
    }
    
    /**
     * Vrací velikost (rozměr) desky.
     * @return Velikost desky.
     */
    int getSize(){
        return this.rules.getSize();
    }
}
