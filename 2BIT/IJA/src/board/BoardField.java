package board;

/**
 * Třída reprezentující aktivní pole na hrací desce.
 * @author Ondrej Janota - xjanot02
 */
public class BoardField extends java.lang.Object implements Field{
    
    private int row = 0;
    private int col = 0;
    private Disk fDisk;
    private final Field[] okoli;
    
    /**
     * Inicializuje pole.
     * 
     * Pole zná informaci o svém umístění.
     * @param row Řádek, na kterém je pole umístěno.
     * @param col Sloupec, na kterém je pole umístěno.
     */
    public BoardField(int row, int col){
        this.row = row;
        this.col = col;
        this.fDisk = null;
        this.okoli = new Field[Field.Direction.values().length];
    }

    /**
     * Přidá sousední pole field v daném směru dirs.
     * @param dirs Směr ve kterém se přidává pole.
     * @param field Přidávané pole.
     */
    @Override
    public void addNextField(Field.Direction dirs, Field field){
        okoli[dirs.ordinal()] = field;
    } 
    
    /**
     * Vrátí sousední pole v daném směru dirs.
     * @param dirs Směr ve kterém se přidává pole.
     * @return Sousední pole v daném směru dirs.
     */
    @Override
    public Field nextField(Field.Direction dirs){
        return okoli[dirs.ordinal()];
    }

    /**
     * Vloží na pole kámen. Při vkládání se ověřuje soulad s pravidly hry.
     * @param disk Vkládaný kámen.
     * @return Vrací úspěšnost akce.
     */
    @Override
    public boolean putDisk(Disk disk){
        if (canPutDisk(disk))
            this.fDisk = disk;
        else
            return false;
        
        return true;
    }
    
    /**
     * Vrací kámen, který je vložen na pole.
     * @return Vložený kámen. Pokud je pole prázdné, vrací null.
     */
    @Override
    public Disk getDisk(){
        return this.fDisk;
    }

    
    /**
     * Test, zda je možné vložit na pole kámen. Při testu se ověřuje soulad s pravidly hry.
     * @param disk Vkládaný kámen.
     * @return Vrací úspěšnost akce.
     */
    @Override
    public boolean canPutDisk(Disk disk){
        return true;
    }
    
    /**
     * Vrací pozici ve stringu row,col
     * @return Vrací pozici 
     */	
    public String getPosition(){
        return this.row + "," + this.col;
    }
	
    /**
     * Odstraní disk
     */
    public void removeDisk(){
        this.fDisk = null;
    }
}
