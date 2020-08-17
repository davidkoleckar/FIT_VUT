package PDB.entity;

import com.fasterxml.jackson.annotation.JsonIgnoreProperties;

import javax.persistence.*;

/**
 * Entita pro vypis ceny danym uzivatelem
 */
@Entity
@Table(name = "uzivatele")
//@JsonIgnoreProperties(value = {"heslo"})
public class PDBUser {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Long id;
    private String email;
    private String heslo;
    private String adresa;
    private String jmeno;

    public PDBUser(){}

    /**
     * Konstruktor
     * @param id
     * @param email
     * @param heslo
     * @param adresa
     * @param jmeno
     */
    public PDBUser(Long id, String email, String heslo, String adresa, String jmeno) {
        this.id = id;
        this.email = email;
        this.heslo = heslo;
        this.adresa = adresa;
        this.jmeno = jmeno;
    }

    /**
     * Ziskani id
     * @return
     */
    public Long getId() {
        return id;
    }

    /**
     * Nastaveni id
     * @param id
     */
    public void setId(Long id) {
        this.id = id;
    }

    /**
     * Ziskani emailu
     * @return
     */
    public String getEmail() {
        return email;
    }

    /**
     * Nastaveni emailu
     * @param email
     */
    public void setEmail(String email) {
        this.email = email;
    }

    /**
     * Ziskani heslo
     * @return
     */
    public String getHeslo() {
        return heslo;
    }

    /**
     * Nastaveni hesla
     * @param heslo
     */
    public void setHeslo(String heslo) {
        this.heslo = heslo;
    }

    /**
     * Ziskani adresy
     * @return
     */
    public String getAdresa() {
        return adresa;
    }

    /**
     * Nastaveni adresy
     * @param adresa
     */
    public void setAdresa(String adresa) {
        this.adresa = adresa;
    }

    /**
     * Ziskani jmena
     * @return
     */
    public String getJmeno() {
        return jmeno;
    }

    /**
     * Nastaveni jmena
     * @param jmeno
     */
    public void setJmeno(String jmeno) {
        this.jmeno = jmeno;
    }
}
