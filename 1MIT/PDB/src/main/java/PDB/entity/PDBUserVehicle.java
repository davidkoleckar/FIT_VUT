package PDB.entity;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import oracle.sql.DATE;

/**
 * Trida pro dotaz jake vozidla dany uzivatel editoval
 */
public class PDBUserVehicle {
    private Long Id;
    private String znacka;
    private Integer rok;
    private Integer kategorie;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE validFrom;
    private String editedBy;
    private Long cena;
    private String jmeno;

    /**
     * Konstruktor tridy
     * @param id
     * @param znacka
     * @param rok
     * @param kategorie
     * @param validFrom
     * @param editedBy
     */
    public PDBUserVehicle(Long id, String znacka, Integer rok, Integer kategorie, DATE validFrom, String editedBy, Long cena, String jmeno) {
        Id = id;
        this.znacka = znacka;
        this.rok = rok;
        this.kategorie = kategorie;
        this.validFrom = validFrom;
        this.editedBy = editedBy;
        this.cena = cena;
        this.jmeno= jmeno;
    }

    public PDBUserVehicle() {
    }

    /**
     * Ziskani id vozidla
     * @return
     */
    public Long getId() {
        return Id;
    }

    /**
     * Nastaveni id vozidla
     * @param id
     */
    public void setId(Long id) {
        Id = id;
    }

    /**
     * Ziskani znacky vozidla
     * @return
     */
    public String getZnacka() {
        return znacka;
    }

    /**
     * Nastaveni znacky vozidla
     * @param znacka
     */
    public void setZnacka(String znacka) {
        this.znacka = znacka;
    }

    /**
     * Ziskani roku vyroby vozidla
     * @return
     */
    public Integer getRok() {
        return rok;
    }

    /**
     * Nastaveni roku vyroby vozidla
     * @param rok
     */
    public void setRok(Integer rok) {
        this.rok = rok;
    }

    /**
     * Ziskani id kategorie vozidla
     * @return
     */
    public Integer getKategorie() {
        return kategorie;
    }

    /**
     * Nastaveni kategorie vozidla
     * @param kategorie
     */
    public void setKategorie(Integer kategorie) {
        this.kategorie = kategorie;
    }

    /**
     * Ziskani od kdy zaznam je validni
     * @return
     */
    public DATE getValidFrom() {
        return validFrom;
    }

    /**
     * Nastaveni od kdy je zaznam validni
     * @param validFrom
     */
    public void setValidFrom(DATE validFrom) {
        this.validFrom = validFrom;
    }

    /**
     * Ziskani id uzivatele
     * @return
     */
    public String getEditedBy() {
        return editedBy;
    }

    /**
     * Nastaveni id uzivatele
     * @param editedBy
     */
    public void setEditedBy(String editedBy) {
        this.editedBy = editedBy;
    }

    public Long getCena() {
        return cena;
    }

    public void setCena(Long cena) {
        this.cena = cena;
    }

    public String getJmeno() {
        return jmeno;
    }

    public void setJmeno(String jmeno) {
        this.jmeno = jmeno;
    }
}
