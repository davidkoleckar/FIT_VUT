package board;

/**
 * Třída reprezentující okrajová (neaktivní) pole desky.
 * Pole neuchovává znalost o svém okolí a nelze ne něj umístit kámen.
 * 
 * @author Ondrej Janota - xjanot02
 */
public class BorderField extends java.lang.Object implements Field{
    /**
     * Konstruktor BorderField
     */
    public BorderField(){
        
    }
    
    /**
     * Nedělá nic.
     * @param dirs Směr ve kterém se přidává pole (nebere se v potaz).
     * @param field Přidávané pole (nebere se v potaz).
     */
    @Override
    public void addNextField(Field.Direction dirs, Field field){
        
    }
    
    /**
     * Vrátí sousední pole v daném směru dirs.
     * @param dirs Směr ve kterém se přidává pole (nebere se v potaz).
     * @return Vždy vrátí null.
     */
    @Override
    public Field nextField(Field.Direction dirs){
        return null;
    }
    
    /**
     * Vloží na pole kámen.
     * 
     * Jednou vložený kámen již nelze odebrat.
     * @param disk Vkládaný kámen.
     * @return Vrací úspěšnost akce. Pokud je pole již obsazeno, vrací false.
     */
    @Override
    public boolean putDisk(Disk disk){
        return false;
    }
    
    /**
     * Vrací kámen, který je vložen na pole.
     * @return Vložený kámen. Pokud je pole prázdné, vrací null.
     */
    @Override
    public Disk getDisk(){
        return (Disk)null;
    }
    
    /**
     * Test, zda je možné vložit na pole kámen. 
     * @param disk Vkládaný kámen.
     * @return Vrací úspěšnost akce.
     */
    @Override
    public boolean canPutDisk(Disk disk){
        return false;
    }
}
