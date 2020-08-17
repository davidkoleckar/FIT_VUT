package board;

/**
 * Rozhraní k polím, které lze umístit na hrací desku.
 * Pole zná své okolí, tj. pole, která ho obklopují ve všech osmi směrech.
 * Implementující třída respektuje při vkládání kamenů pravidla dané hry.
 * @author Ondrej Janota - xjanot02
 */
public interface Field {
    
    /**
     * Přidá sousední pole field v daném směru dirs.
     * @param dirs Směr ve kterém se přidává pole.
     * @param field Přidávané pole.
     */
    void addNextField(Field.Direction dirs, Field field);
    
    /**
     * Test, zda je možné vložit na pole kámen. Při testu se ověřuje soulad s pravidly hry.
     * @param disk Vkládaný kámen.
     * @return Vrací úspěšnost akce.
     */
    boolean canPutDisk(Disk disk);
    
    /**
     * Vrací kámen, který je vložen na pole.
     * @return Vložený kámen. Pokud je pole prázdné, vrací null.
     */
    Disk getDisk();
    
    /**
     * Vrátí sousední pole v daném směru dirs.
     * @param dirs Směr ve kterém se přidává pole.
     * @return Sousední pole v daném směru dirs.
     */
    Field nextField(Field.Direction dirs);
    
    /**
     * Vloží na pole kámen. Při vkládání se ověřuje soulad s pravidly hry.
     * @param disk Vkládaný kámen.
     * @return Vrací úspěšnost akce.
     */
    boolean putDisk(Disk disk);
    
    /**
     * Výčtový typ reprezentující okolí (směry) jednotlivých polí.
     */
    public enum Direction{
        /**
         * Nahoru (Up) od pole.
         */
        U,
        /**
         * Vpravo nahoru po diagonále (Right-Up) od pole.
         */
        RU,
        /**
         * Vpravo (Right) od pole.
         */
        R,
        /**
         * Vpravo dole po diagonále (Right-Down) od pole.
         */
        RD,
        /**
         * Dole (Down) od pole.
         */
        D,
        /**
         * Vlevo dole po diagonále (Left-Down) od pole.
         */
        LD,
        /**
         * Vlevo (Left) od pole.
         */
        L,
        /**
         * Vlevo nahoru po diagonále (Left-Up) od pole.
         */
        LU
    }
}
