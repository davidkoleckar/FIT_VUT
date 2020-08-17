package PDB.entity;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import oracle.sql.DATE;

/**
 * Trida pro dotaz ceny vozidla
 */
public class PDBUserPrice {
    private Long cena;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE validFrom;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE validTo;
    private String jmeno;

    /**
     * Konstruktor tridy
     * @param cena
     * @param valid_from
     * @param valid_to
     * @param jmeno
     */
    public PDBUserPrice(Long cena, DATE valid_from, DATE valid_to, String jmeno) {
        this.cena = cena;
        this.validFrom = valid_from;
        this.validTo = valid_to;
        this.jmeno = jmeno;
    }

    public PDBUserPrice() {
    }

    /**
     * Ziskani ceny
     * @return
     */
    public Long getCena() {
        return cena;
    }

    /**
     * Nastaveni ceny
     * @param cena
     */
    public void setCena(Long cena) {
        this.cena = cena;
    }

    /**
     * Ziskani validity zaznamu
     * @return
     */
    public DATE getValidFrom() {
        return validFrom;
    }

    /**
     * Nastaveni validity zaznamu
     * @param validFrom
     */
    public void setValidFrom(DATE validFrom) {
        this.validFrom = validFrom;
    }

    /**
     * Ziskani konce platnosti zaznamu
     * @return
     */
    public DATE getValidTo() {
        return validTo;
    }

    /**
     * Nastaveni konce platnosti zaznamu
     * @param validTo
     */
    public void setValidTo(DATE validTo) {
        this.validTo = validTo;
    }

    /**
     * Ziskani jmena
     * @return
     */
    public String getJmeno() {
        return jmeno;
    }

    /**
     * Nstaveni jmena
     * @param jmeno
     */
    public void setJmeno(String jmeno) {
        this.jmeno = jmeno;
    }
}
