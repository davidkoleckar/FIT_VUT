package game;
import board.Board;
import board.BoardField;
import board.Disk;
import board.Field;
import java.util.LinkedList;

/**
 * Reprezentuje hráče hry. Hráč má bílou nebo černou barvu. 
 * Po vytvoření reprezentuje volného hráče.
 * Součástí hráče je sada kamenů, které má k dispozici pro vkládání na desku.
 * Volný hráč musí být inicializován v rámci hrací desky
 * (vizte metodu init(ija.ija2015.homework2.board.Board)).
 * @author Ondrej Janota - xjanot02
 */
public class Player extends java.lang.Object{
    
    private final boolean isWhite;
    private Board board;
    protected int disksOnBoard;
    
    private LinkedList remove;
    private LinkedList turned;
    private String turn;
    
    /**
     * Inicializace hráče.
     * @param isWhite Určuje barvu hráče (true = bílý, false = černý)
     */
    public Player(boolean isWhite){
        this.isWhite = isWhite;
        this.disksOnBoard = 0;
        this.turn = "";
        this.remove = new LinkedList();
        this.turned = new LinkedList();
    }
    
    @Override
    public String toString(){
        return this.isWhite? "WHITE" : "BLACK";
    }

    
    private boolean testBorder(Field field, Field.Direction dirs, boolean canTurn, boolean first){
        Disk disk = null;
        if ((disk = field.getDisk()) != null && !disk.isFrozen()){
            if (disk.isWhite() == this.isWhite){
                if (!first)
                    return true;
            }
            else
                if (testBorder(field.nextField(dirs), dirs, canTurn, false)){
                    if (canTurn){
                        disk.turn();
                        this.disksOnBoard++;
                        turn += ((BoardField)field).getPosition() + System.lineSeparator();
                    }
                    return true;
                }
                else
                    return false;
        }
        return false;
    }
    private boolean border(Field field, boolean canTurn){
        boolean can = false;
        boolean isTrue = false;
        
        can = testBorder(field.nextField(Field.Direction.R), Field.Direction.R, canTurn, true);
            
        isTrue = testBorder(field.nextField(Field.Direction.RD), Field.Direction.RD, canTurn, true);
        
        if (isTrue == true && can == false)
            can = true;
        
        isTrue = testBorder(field.nextField(Field.Direction.D),Field.Direction.D, canTurn, true);
            
        if (isTrue == true && can == false)
            can = true;
        
        isTrue = testBorder(field.nextField(Field.Direction.LD),Field.Direction.LD, canTurn, true);
            
        if (isTrue == true && can == false)
            can = true;
        
        isTrue = testBorder(field.nextField(Field.Direction.L), Field.Direction.L, canTurn, true);
        
        if (isTrue == true && can == false)
            can = true;
        
        isTrue = testBorder(field.nextField(Field.Direction.LU), Field.Direction.LU, canTurn, true);
            
        if (isTrue == true && can == false)
            can = true;
        
        isTrue = testBorder(field.nextField(Field.Direction.U), Field.Direction.U, canTurn, true);
            
        if (isTrue == true && can == false)
            can = true;
        
        isTrue = testBorder(field.nextField(Field.Direction.RU), Field.Direction.RU, canTurn, true);
        
        if (isTrue == true && can == false)
            can = true;
        
        return can;
    }
    
    /**
     * Test, zda je možné vložit nový kámen hráče na dané pole. 
     * Kámen se sady nevybírá ani nevkládá na pole.
     * @param field Pole, na které se má vkládat kámen.
     * @return Zda je možné kámen vložit.
     */
    public boolean canPutDisk(Field field){
        if (field.getDisk() == null)
            if (border(field, false))
                return true;
        return false;
    }
    
    /**
     * Inicializace hráče v rámci hrací desky.
     * Vytvoří sadu kamenů o příslušné velikosti a umístí 
     * své počáteční kameny na desku.
     * @param board Deska, v rámci které se hráč inicializuje.
     */
    protected void init(Board board){
        this.board = board;
        
        if (!this.isWhite){
            board.getField(board.getRules().getSize()/2, board.getRules().getSize()/2).putDisk(new Disk(this.isWhite));
            board.getField(board.getRules().getSize()/2+1, board.getRules().getSize()/2+1).putDisk(new Disk(this.isWhite));
        }
        else{
            board.getField(board.getRules().getSize()/2+1, board.getRules().getSize()/2).putDisk(new Disk(this.isWhite));
            board.getField(board.getRules().getSize()/2, board.getRules().getSize()/2+1).putDisk(new Disk(this.isWhite));
        }
        this.disksOnBoard += 2;
    }
    
    /**
     * Test barvy hráče.
     * @return Zda je hráč bílý.
     */
    public boolean isWhite(){
        return this.isWhite;
    }
    
    /**
	 * @return Vrací počet disků hráče na desce.
     */     
    public int onBoard(){
        return this.disksOnBoard;
    }
    /**
     * Vloží nový kámen hráče na dané pole, pokud to pravidla umožňují. 
     * Pokud lze vložit, vybere kámen ze sady a vloží na pole.
     * @param field Pole, na které se vkládá kámen.
     * @return Úspěch akce.
     */
    public boolean putDisk(Field field){
        if (field.getDisk() != null)
            return false;
        
        if (canPutDisk(field)){
            border(field,true);
            field.putDisk(new Disk(this.isWhite));
            this.disksOnBoard++;
            turned.add(turn);
            turn = "";
            remove.add(((BoardField)field).getPosition());
        }
        else
            return false;
        
        return true;
    }
	
    /**
	 * @return Vrací true, pokud hráč má nějaký tah.
     */     
    public boolean canPutAnywhere()
    {
        for (int i = 1; i < this.board.getRules().getSize() + 1; i++)
            for (int j = 1; j < this.board.getRules().getSize() + 1; j++)
                if (canPutDisk(this.board.getField(i, j)))
                    return true;
        remove.add("null");
        turned.add("null");
        return false;
    }
	
    /**
	 * Funkce provede tah zpět.
     */    
    public int doUndo(){
        if (remove.isEmpty())
            return 0;
        String last = (String)remove.getLast();
        if (last == "null")
        {
            remove.removeLast();
            turned.removeLast();
            return -1;
        }
        int index = last.indexOf(",");
        int i = Integer.parseInt(last.substring(0, index));
        int j = Integer.parseInt(last.substring(index+1, last.length()));
        
        remove(i, j);
        remove.removeLast();
        this.disksOnBoard--;
        
        last = (String)turned.getLast();
        turned.removeLast();
        
        int newLine;
        
        int count = 0;
        
        while (!last.isEmpty())
        {
            index = last.indexOf(",");
            newLine = last.indexOf(System.lineSeparator());
            i = Integer.parseInt(last.substring(0, index));
            j = Integer.parseInt(last.substring(index+1, newLine));
            
            this.disksOnBoard--;
            
            this.board.getField(i, j).getDisk().turn();
          
            count++;
            
            last = last.replaceFirst("[^\n]*[\n]", "");
        }
        
        return count;
    }

    /**
	 * Odstraní disk na zadaných souřadnicích.
     * @param i Řádek
     * @param j Sloupec
     */     
    public void remove(int i, int j){
        ((BoardField)this.board.getField(i, j)).removeDisk();
    }

    /**
	 * @return Vrací list s odebranými disky
     */     
    public LinkedList getRemove(){
        return remove;
    }
    
	/**
	 * @return Vrací list s otočenými disky.
     */ 
    public LinkedList getTurned(){
        return turned;
    }
	
    /**
	 * Nastaví list remove
	 * @param r List se smazanými disky.
     */     
    public void setRemove(LinkedList r){
        remove = r;
    }
	
    /**
	 * Nastaví list turned
	 * @param t List se otočenými disky.
     */     
    public void setTurned(LinkedList t){
        turned = t;
    }
}
