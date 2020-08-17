package PDB.entity;

import javax.persistence.*;

/**
 * Trida pro praci s kategoriemi vozidel
 * @author David Koleckar
 */
@Entity
@Table(name = "kategorie")
public class PDBCategory {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String nazev;

    public PDBCategory() {
    }

    /**
     * Konstruktor kategorie
     * @param id
     * @param nazev
     */
    public PDBCategory(Long id, String nazev) {
        this.id = id;
        this.nazev = nazev;
    }

    /**
     * Vraceni id kategorie
     * @return
     */
    public Long getId() {
        return id;
    }

    /**
     * Nastaveni id kategorie
     * @param id
     */
    public void setId(Long id) {
        this.id = id;
    }

    /**
     * Vraceni nazvu kategorie
     * @return
     */
    public String getNazev() {
        return nazev;
    }

    /**
     * Nastaveni nazvu kategorie
     * @param nazev
     */
    public void setNazev(String nazev) {
        this.nazev = nazev;
    }
}
