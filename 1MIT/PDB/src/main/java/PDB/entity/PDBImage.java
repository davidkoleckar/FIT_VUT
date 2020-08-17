package PDB.entity;

import javax.persistence.*;

/**
 * Trida pro ulozeni informaci o obrazku
 */
@Entity
@Table(name = "obrazky")
public class PDBImage {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private Long vozidlo;

    public PDBImage() {
    }

    /**
     * Konstruktor tridy
     * @param id
     * @param vozidlo
     */
    public PDBImage(Long id, Long vozidlo) {
        this.id = id;
        this.vozidlo = vozidlo;
    }

    /**
     * Ziskani id obrazku
     * @return
     */
    public Long getId() {
        return id;
    }

    /**
     * Nastaveni id obrazku
     * @param id
     */
    public void setId(Long id) {
        this.id = id;
    }

    /**
     * Ziskani id vozidla
     * @return
     */
    public Long getVozidlo() {
        return vozidlo;
    }

    /**
     * Nastaveni id vozidla
     * @param vozidlo
     */
    public void setVozidlo(Long vozidlo) {
        this.vozidlo = vozidlo;
    }


}