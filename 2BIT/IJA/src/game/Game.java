package game;
import board.*;
import static java.lang.Thread.sleep;
import java.util.LinkedList;
import java.util.List;

import java.util.Random;
import java.util.logging.Level;
import java.util.logging.Logger;


/**
 * Reprezentuje hru. Při inicializaci se vždy přiřadí hrací deska.
 * @author Ondrej Janota - xjanot02
 */
public class Game extends java.lang.Object{
    
    private final Board board;
    private final Player[] player;
    private boolean currentPlayer;
    private int[] count;
    private final boolean isComputer;
    private final int alg;
    private LinkedList a, b;
    private Random rand;
    private boolean isEnd;
    private boolean canUnfreeze;
    private boolean canFreeze;
    
    /**
     * Inicializuje hru.
     * @param board Hrací deska, která je součástí hry.
     * @param isComputer
     * @param alg
     */
    public Game(Board board, boolean isComputer, int alg){
        this.board = board;
        
        this.player = new Player[2];
        this.player[0] = null;
        this.player[1] = null;

        this.currentPlayer = false;
        
        this.count = new int[2];
        this.count[0] = 0;
        this.count[1] = 0;
        
        this.isComputer = isComputer;
        this.alg = alg;
        
        this.canFreeze = false;
        this.canUnfreeze = true;
        
        this.isEnd = false;
    }
    
    /**
     * Přidá hráče a současně vyvolá jeho inicializaci.
     * Pokud hráč dané barvy již existuje, nedělá nic a vrací false.
     * @param player Přidávaný hráč.
     * @return Úspěch akce.
     */
    public boolean addPlayer(Player player){
        if (this.player[0] == null){
            if (!player.isWhite()){
                this.player[0] = player;
                this.player[0].init(this.board);
                this.count[0] = this.player[0].disksOnBoard;
            }
            else 
                if (this.player[1] == null || isComputer){
                    this.player[1] = player;
                    this.player[1].init(this.board);
                    this.count[1] = this.player[1].disksOnBoard;
                }
                else
                    return false;
        }
        else
            if (player.isWhite() || isComputer){
                if (this.player[1] == null){
                    this.player[1] = player;
                    this.player[1].init(this.board);
                    this.count[1] = this.player[1].disksOnBoard;
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
    public Player currentPlayer(){
        if (this.currentPlayer)
            return this.player[1];
        else
            return this.player[0];
    }
    
    /**
     * Vrátí hrací desku.
     * @return Hrací deska.
     */
    public Board getBoard(){
        return this.board;
    }
    
    /**
     * Změní aktuálního hráče.
     * @return Aktuálně nastavený hráč.
     */
    public Player nextPlayer(){
        canUnfreeze = true;
        if (this.currentPlayer){
            this.currentPlayer = !this.currentPlayer;
            if (count[1] != this.player[1].onBoard())
            {
                this.count[0] -= this.player[1].onBoard() - this.count[1] - 1;
                this.player[0].disksOnBoard = this.count[0];
                this.count[1] = this.player[1].onBoard();
            }
            return this.player[0];
        }
        else{
            this.currentPlayer = !this.currentPlayer;
            if (count[0] != this.player[0].onBoard())
            {
                this.count[1] -= this.player[0].onBoard() - this.count[0] - 1;
                this.player[1].disksOnBoard = this.count[1];
                this.count[0] = this.player[0].onBoard();
            }
            return this.player[1];
        }
    }
	
    /**
     * @return Vrátí dalšího hráče na tahu.
     */
    public Player getNextPlayer(){
        if (this.currentPlayer)
            return this.player[0];
        else
            return this.player[1];
    }
	
    /**
     * Nastaví aktualního hráče. 
	 * @param isWhite barva hráče.
     */
    public void setPlayer(boolean isWhite){
        this.currentPlayer = isWhite;
    }
	
    /**
     * Nastaví hrači počet disku. 
	 * @param isWhite barva hráče.
	 * @param count počet disku
     */
    public void setDisks(boolean isWhite, int count){
        if (isWhite){
            this.player[1].disksOnBoard = count;
            this.count[1] = this.player[1].onBoard();
        }
        else{
            this.player[0].disksOnBoard = count;
            this.count[0] = this.player[0].onBoard();
        }
    }
	
    /**
     * Vloží disk na danou pozici 
	 * @param isWhite barva hráče.
     * @param i Řádek
     * @param j Sloupec
     */
    public void prefferPutDisk(boolean isWhite, int i, int j){
        this.board.getField(i, j).putDisk(new Disk(isWhite));
    }
    
    /**
	 * @return Vrací počet disků bílého hráče
     */
    public int getDisksWhite(){
        return this.count[1];
    }
    /**
	 * @return Vrací počet disků černého hráče
     */ 
    public int getDisksBlack(){
        return this.count[0];
    }
    /**
	 * Umělá inteligence pro automatický tah, při hře proti PC.
	 * První algoritmus na první možnou pozici vloží disk.
	 * Druhý algoritmus vybere náhodnou možnou pozici na který muže vložit disk.
	 */
    public void automaticTurn(){
        List is = new LinkedList();
        List js = new LinkedList();
        
        if (this.alg == 1)
        {
            for (int i = 1; i < this.board.getRules().getSize() + 1; i++)
                for (int j = 1; j < this.board.getRules().getSize() + 1; j++)
                    if (this.player[1].putDisk(this.board.getField(i, j)))
                        return;
        }
        else
            while(this.player[1].canPutAnywhere())
            {
                for (int i = 1; i < this.board.getRules().getSize() + 1; i++)
                    for (int j = 1; j < this.board.getRules().getSize() + 1; j++)
                        if (this.currentPlayer().canPutDisk(this.board.getField(i, j)))
                        {
                            is.add(i);
                            js.add(j);
                        }
                
                int index = new Random().nextInt(is.size());

                this.currentPlayer().putDisk(this.board.getField((int) is.get(index), (int) js.get(index)));
                return;
            }
    }
	
    /**
	 * Nastaví příznak zamrzaní disků.
     */    
    public void setFreeze(boolean can){
        this.canFreeze = can;
    }
    /**
	 * Zruší příznak zamrzaní disků.
     */   
    public void setUnfreeze(boolean can){
        this.canUnfreeze = can;
    }
	
    /**
	 * Zahájí zamrzání disků.
	 * @param i časový interval v sekundách
	 * @param b časový interval v sekundách
	 * @param c počet disků 
     */    
    public void doFreeze(int i, int b, int c){
        rand = new Random();
        
        Frozen f = new Frozen(i, b, c);
        
        Thread t = new Thread(f);
        t.start();
    }
    
    /**
	 * Zaznamená konec hry.
     */ 	
    public void setEnd(){
        this.isEnd = true;
    }
	
    /**
	 * @return Vrací true pokud je konec hry.
     */     
    public boolean getEnd(){
        return this.isEnd;
    }
    
 /**
 * Třída pro zamrzaní disků na hrací desce.
 * @author Ondrej Janota - xjanot02
 */   
    public class Frozen implements Runnable{
    
        int I; 
        int B;
        int C;
		/**
		 * Konstruktor
		 * @param i časový interval v sekundách
		 * @param b časový interval v sekundách
		 * @param c počet disků
		 */ 
        public Frozen(int i, int b, int c){
            this.I = i;
            this.B = b;
            this.C = c;
        }

        @Override
        public void run() {
            Random r = new Random();
            //System.out.print(I + " " + B + " " + C);
            while(!getEnd()){
                System.out.println(getEnd());
                
                if (canFreeze)
                    canFreeze = false;
                else
                    continue;
                
                try{
                    sleep(r.nextInt(this.I)*1000);
                } catch (InterruptedException ex) {
                    Logger.getLogger(Game.class.getName()).log(Level.SEVERE, null, ex);
                }
                
                doFreeze();
            }
            if (getEnd())
                System.out.print("SKONCIL JSEM");
        }
		
        /**
         * Provádí zamrzání a rozmrzání disků
         */ 
        private void doFreeze() {
            
            a = new LinkedList();
          
            Disk[] disks = new Disk[this.C];

            for (int i = 1; i < board.getRules().getSize(); i++)
                for (int j = 1; j < board.getRules().getSize(); j++)
                    if (board.getField(i, j).getDisk() != null)
                        a.add(board.getField(i, j));
            
            Random r = new Random();
            int index;
            
            for (int i = 0; i < this.C; i++)
            {
                index = r.nextInt(a.size());
                disks[i] = ((Field)a.get(index)).getDisk();
                disks[i].freeze();
                a.remove(index);
            }
            
            try {
                sleep(r.nextInt(this.B)*1000);
                canUnfreeze = false;
            } catch (InterruptedException ex) {
                Logger.getLogger(Game.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            
            while(disks[C-1].isFrozen())
            {
                System.out.print("");
                if(canUnfreeze)
                {
                    for(int i = 0; i < this.C; i++)
                        disks[i].unfreeze();
                }
            }
            
            canFreeze = true;
            
        }
    }
}
