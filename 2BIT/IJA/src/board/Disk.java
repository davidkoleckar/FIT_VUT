package board;

/**
 * Třída reprezentuje jeden kámen.
 * Kámen může nabývat dvou barev - bílá nebo černá.
 * 
 * @author Ondrej Janota - xjanot02
 */
public class Disk {
    
    private boolean isWhite;
    private boolean isFrozen;
    /**
     * Inicializace kamene.
     * @param isWhite Nastavení barvy - true je bílá, false je černá.
     */
    public Disk(boolean isWhite){
        this.isWhite = isWhite;
        this.isFrozen = false;
    }
    
    /**
     * Test, zda je kámen bílý.
     * @return Vrací true, pokud je kámen bílý.
     */
    public boolean isWhite(){
        return this.isWhite;
    }
    
    /**
     * Otočení (změna barvy) kamene.
     */
    public void turn(){
        this.isWhite = !this.isWhite;
    }
    /**
     * Nastaví zamrzání disku
     */
    public void freeze(){
        this.isFrozen = true;
    }
    /**
     * Odebere zamrzání disku
     */
    public void unfreeze(){
        this.isFrozen = false;
    }
    /**
     * @return Vrací true, pokud je nastaveno zamrzání.
     */
    public boolean isFrozen(){
        return this.isFrozen;
    }
}
