package PDB.entity;


import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import oracle.sql.DATE;

import javax.persistence.*;
import java.io.Serializable;

/**
 * Trida reprezentujici dane vozidlo v autobazaru
 */
@Entity
@Table(name= "vozidla")
@IdClass(PDBVehicleId.class)
public class PDBvehicle implements Serializable {

    @Id
    private Long id;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE nakup;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE prodej;
    private String znacka;
    private String model;
    private String motor;
    private Integer rok;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE stk;
    private Long kupniCena;
    private Long cena;
    private Long kilometry;
    private String stav;
    private Integer kategorie;
    private Integer umisteni;
    @Id
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE validFrom;
    @JsonSerialize(using = DATECustomSerializer.class)
    private DATE validTo;

    private Long userId;
    private Long obrazek;

    private String obrazekData;

    public PDBvehicle(){}

    /**
     * Kontruktor tridy vozidlo
     * @param id
     * @param nakup
     * @param prodej
     * @param znacka
     * @param model
     * @param motor
     * @param rok
     * @param stk
     * @param kupniCena
     * @param cena
     * @param kilometry
     * @param stav
     * @param kategorie
     * @param umisteni
     * @param validFrom
     * @param validTo
     * @param userId
     * @param obrazek
     * @param obrazekData
     */
    public PDBvehicle(Long id, DATE nakup, DATE prodej, String znacka, String model, String motor, Integer rok,
                      DATE stk, Long kupniCena, Long cena, Long kilometry, String stav, Integer kategorie,
                      Integer umisteni, DATE validFrom, DATE validTo, Long userId, Long obrazek, String obrazekData) {
        this.id = id;
        this.nakup = nakup;
        this.prodej = prodej;
        this.znacka = znacka;
        this.model = model;
        this.motor = motor;
        this.rok = rok;
        this.stk = stk;
        this.kupniCena = kupniCena;
        this.cena = cena;
        this.kilometry = kilometry;
        this.stav = stav;
        this.kategorie = kategorie;
        this.umisteni = umisteni;
        this.validFrom = validFrom;
        this.validTo = validTo;
        this.userId = userId;
        this.obrazek = obrazek;
        this.obrazekData = obrazekData;
    }

    /**
     * Metoda vraci id obrazku
     * @return
     */
    public Long getObrazek() {
        return obrazek;
    }

    /**
     * Metoda nastavi id obrazku
     * @param obrazek
     */
    public void setObrazek(Long obrazek) {
        this.obrazek = obrazek;
    }

    /**
     * Metoda vrati data obrazku base64
     * @return
     */
    public String getObrazekData() {
        return obrazekData;
    }

    /**
     * Metoda nastavi obrazku data base64
     * @param obrazekData
     */
    public void setObrazekData(String obrazekData) {
        this.obrazekData = obrazekData;
    }

    /**
     * Metoda pro vraceni id uzivatele
     * @return
     */
    public Long getUserId() {
        return userId;
    }

    /**
     * Metoda pro nastaveni id uzivatele
     * @param userId
     */
    public void setUserId(Long userId) {
        this.userId = userId;
    }

    /**
     * Metoda pro vraceni ceny
     * @return
     */
    public Long getCena() {
        return cena;
    }

    /**
     * Metoda pro nastaveni ceny
     * @param cena
     */
    public void setCena(Long cena) {
        this.cena = cena;
    }

    /**
     * Metoda pro vraceni id vozidla
     * @return
     */
    public Long getId() {
        return id;
    }

    /**
     * Metoda pro nastaveni id vozidla
     * @param id
     */
    public void setId(Long id) {
        this.id = id;
    }

    /**
     * Metoda pro vraceni data nakupu
     * @return
     */
    public DATE getNakup() {
        return nakup;
    }

    /**
     * Metoda pro nastaveni data nakupu
     * @param nakup
     */
    public void setNakup(DATE nakup) {
        this.nakup = nakup;
    }

    /**
     * Metoda pro vraceni data prodeje
     * @return
     */
    public DATE getProdej() {
        return prodej;
    }

    /**
     * Metoda pro nastaveni data prodeje
     * @param prodej
     */
    public void setProdej(DATE prodej) {
        this.prodej = prodej;
    }

    /**
     * Metoda pro vraceni znacky vozidla
     * @return
     */
    public String getZnacka() {
        return znacka;
    }

    /**
     * Metoda pro nastaveni znacky
     * @param znacka
     */
    public void setZnacka(String znacka) {
        this.znacka = znacka;
    }

    /**
     * Metoda pro vraceni modelu vozidla
     * @return
     */
    public String getModel() {
        return model;
    }

    /**
     * Metoda pro nastaveni modelu vozidla
     * @param model
     */
    public void setModel(String model) {
        this.model = model;
    }

    /**
     * Metoda pro vraceni motoru vozidla
     * @return
     */
    public String getMotor() {
        return motor;
    }

    /**
     * Metoda pro nastaveni motoru vozidla
     * @param motor
     */
    public void setMotor(String motor) {
        this.motor = motor;
    }

    /**
     * Metoda pro ziskani roku vyroby vozidla
     * @return
     */
    public Integer getRok() {
        return rok;
    }

    /**
     * Metoda pro nastaveni roku vyroby vozidla
     * @param rok
     */
    public void setRok(Integer rok) {
        this.rok = rok;
    }

    /**
     * Metoda pro ziskani data stk
     * @return
     */
    public DATE getStk() {
        return stk;
    }

    /**
     * Nastaveni STK
     * @param stk
     */
    public void setStk(DATE stk) {
        this.stk = stk;
    }

    /**
     * Ziskani kupni ceny
     * @return
     */
    public Long getKupniCena() {
        return kupniCena;
    }

    /**
     * Nastaveni kupni ceny
     * @param kupniCena
     */
    public void setKupniCena(Long kupniCena) {
        this.kupniCena = kupniCena;
    }

    /**
     * Vraceni poctu ujetych kilometru
     * @return
     */
    public Long getKilometry() {
        return kilometry;
    }

    /**
     * Nastaveni poctu ujetych kilometru
     * @param kilometry
     */
    public void setKilometry(Long kilometry) {
        this.kilometry = kilometry;
    }

    /**
     * Ziskani technickeho stavu vozidla
     * @return
     */
    public String getStav() {
        return stav;
    }

    /**
     * Ziskani technickeho stavu vozidla
     * @param stav
     */
    public void setStav(String stav) {
        this.stav = stav;
    }

    /**
     * Ziskani id kategorie vozidla
     * @return
     */
    public Integer getKategorie() {
        return kategorie;
    }

    /**
     * Nastaveni id kaegorie vozidla
     * @param kategorie
     */
    public void setKategorie(Integer kategorie) {
        this.kategorie = kategorie;
    }

    /**
     * Ziskani id umisteni v arealu
     * @return
     */
    public Integer getUmisteni() {
        return umisteni;
    }

    /**
     * Nastaveni id umisteni v arealu
     * @param umisteni
     */
    public void setUmisteni(Integer umisteni) {
            this.umisteni = umisteni;
    }


    /**
     * Vypis hodnot
     * @return
     */
    @Override
    public String toString() {
        return "Znacka: " + this.znacka + '\n' +
                "ID: " + this.id + "\n" +
                "ROK " + this.rok + "\n" +
                "KATEGOIE " + this.kategorie + "\n" +
                "Model: " + this.model + '\n' +
                "Motor: " + this.motor + '\n' +
                "Stav: " + this.stav + '\n' +
                "Kupni cena: " + this.kupniCena + '\n' +
                "Cena: " + this.cena + '\n' +
                "Kilometry: " + this.kilometry + '\n' +
                "Umisteni: " + this.umisteni + '\n' +
                "Nakup: " + this.nakup + '\n' +
                "Prodej: " + this.prodej + '\n' +
                "Stk: " + this.stk + '\n';
    }

    /**
     * Ziskani data pocatku validity zaznamu
     * @return
     */
    public DATE getValidFrom() {
        return validFrom;
    }

    /**
     * Nastaveni data pocatku validity zaznamu
     * @param validFrom
     */
    public void setValidFrom(DATE validFrom) {
        this.validFrom = validFrom;
    }

    /**
     * Ziskani data konce validity
     * @return
     */
    public DATE getValidTo() {
        return validTo;
    }

    /**
     * Nastaveni validity zaznamu
     * @param validTo
     */
    public void setValidTo(DATE validTo) {
        this.validTo = validTo;
    }
}
