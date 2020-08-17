
package game;
import board.*;
import java.util.ArrayList;

/**
 * Třída reprezentuje novou hru. Vytvoří board, players, rules a game.
 * @author Ondrej Janota - xjanot02
 */
public class newGame{
    
    private final int size;
    private final Board board;
    private final Game game;
    
    /**
     * Konstruktor newGame
     * @param size
     * @param isComputer
     */
    public newGame(int size, boolean isComputer, int alg){
        this.size = size;
        ReversiRules rules = new ReversiRules(this.size);
        this.board = new Board(rules);
        this.game = new Game(this.board, isComputer, alg);
        
        Player p1 = new Player(false);
        Player p2 = new Player(true);
        
        game.addPlayer(p1);
        game.addPlayer(p2);
        
    }
    /**
     * @return Vrací aktualni hru.
     */    
    public Game getGame()
    {
        return this.game;
    }
	
    /**
	 * Nastaví aktualniho hrace na tahu.
     * @param player Hráč.
     */     
    public void currentPlayer(boolean player){
        this.game.setPlayer(player);
    }
	
    /**
	 * Nastaví počet disku hráči
     * @param isWhite Barva hráče.
	 * @param count Počet disků.
     */    
    public void countDisks(boolean isWhite, int count){
        this.game.setDisks(isWhite, count);
    }
	
    /**
	 * Pro load game nastaví disky.
     * @param isWHite Barva hráče.
	 * @param listDisks List disků.
     */     
    public void disks(boolean isWhite, ArrayList listDisks){
        int count;
        
        if (isWhite)
            count = this.game.getDisksWhite();
        else
            count = this.game.getDisksBlack();
        
        for (int x = 0; x < count; x++)
        {
            String positions = (String)listDisks.get(x);
            int index = positions.indexOf(",");
            int i = Integer.parseInt(positions.substring(0, index));
            int j = Integer.parseInt(positions.substring(index+1, positions.length()));

            this.game.prefferPutDisk(isWhite, i,j);
        }
        
    }
	
    /**
	 * Uloží disky hráče do listu.
     * @param isWhite Barva Hráče.
	 * @return List disků.
     */    
    public ArrayList getDisks(boolean isWhite){
        
        ArrayList disks = new ArrayList<String>();
        
        for (int i = 1; i < size + 1; i++)
            for (int j = 1; j < size + 1; j++)
                if (this.board.getField(i, j).getDisk() != null)
                    if (this.board.getField(i, j).getDisk().isWhite() == isWhite)
                        disks.add(i + "," + j);
                        
        return disks;
    }
}
