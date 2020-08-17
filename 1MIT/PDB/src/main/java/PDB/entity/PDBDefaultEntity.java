package PDB.entity;

import java.util.ArrayList;
import java.util.Collection;

/**
 * Trida pro definovani zakladnich entit
 */
public class PDBDefaultEntity {

    private String typEntity;
    private Collection<PDBAreaPoints> tvar;
    private Long id;
    private String nazev;
        private String typ;

    /**
     * Konstruktor
     * @param typEntity
     */
    public PDBDefaultEntity(String typEntity) {
        this.typEntity = typEntity;
        this.tvar = new ArrayList<PDBAreaPoints>();
        switch (typEntity){
            case "kruh":
                this.tvar.add(new PDBAreaPoints(5.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 5.0));
                this.tvar.add(new PDBAreaPoints(5.0, 10.0));
                break;
            case "ctverec":
                this.tvar.add(new PDBAreaPoints(0.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 10.0));
                this.tvar.add(new PDBAreaPoints(0.0, 10.0));
                this.tvar.add(new PDBAreaPoints(0.0, 0.0));
                break;
            case "trojuhelnik":
                this.tvar.add(new PDBAreaPoints(0.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 0.0));
                this.tvar.add(new PDBAreaPoints(5.0, 5.0));
                this.tvar.add(new PDBAreaPoints(0.0, 0.0));
                break;
            case "elko":
                this.tvar.add(new PDBAreaPoints(0.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 5.0));
                this.tvar.add(new PDBAreaPoints(5.0, 5.0));
                this.tvar.add(new PDBAreaPoints(5.0, 10.0));
                this.tvar.add(new PDBAreaPoints(0.0, 10.0));
                this.tvar.add(new PDBAreaPoints(0.0, 0.0));
                break;
            case "oval":
                this.tvar.add(new PDBAreaPoints(0.0, 5.0));
                this.tvar.add(new PDBAreaPoints(5.0, 0.0));
                this.tvar.add(new PDBAreaPoints(10.0, 5.0));
                this.tvar.add(new PDBAreaPoints(10.0, 10.0));
                this.tvar.add(new PDBAreaPoints(5.0, 15.0));
                this.tvar.add(new PDBAreaPoints(0.0, 10.0));
                this.tvar.add(new PDBAreaPoints(0.0, 5.0));
        }
    }


    public PDBDefaultEntity() {
    }

    /**
     * Konstruktor
     * @param typEntity
     * @param tvar
     * @param id
     * @param nazev
     * @param typ
     */
    public PDBDefaultEntity(String typEntity, Collection<PDBAreaPoints> tvar, Long id, String nazev, String typ) {
        this.typEntity = typEntity;
        this.tvar = tvar;
        this.id = id;
        this.nazev = nazev;
        this.typ = typ;
    }

    /**
     * Ziskani typu
     * @return
     */
    public String getTyp() {
        return typ;
    }

    /**
     * Nastaveni typu
     * @param typ
     */
    public void setTyp(String typ) {
        this.typ = typ;
    }

    /**
     * Ziskani typu entity
     * @return
     */
    public String getTypEntity() {
        return typEntity;
    }

    /**
     * Nastaveni typu entity
     * @param typEntity
     */
    public void setTypEntity(String typEntity) {
        this.typEntity = typEntity;
    }

    /**
     * Ziskani tvaru
     * @return
     */
    public Collection<PDBAreaPoints> getTvar() {
        return tvar;
    }

    public void setTvar(Collection<PDBAreaPoints> tvar) {
        this.tvar = tvar;
    }

    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

    public String getNazev() {
        return nazev;
    }

    public void setNazev(String nazev) {
        this.nazev = nazev;
    }

    public String pointStringArray(){
        String points = "";
        for(PDBAreaPoints point: this.tvar){
            points = points + point.getX().intValue() + ", " + point.getY().intValue() + ",";
        }
        points = points.substring(0, points.length() -1);
        return points;
    }


    public Collection<Double> pointsToDoubleArray(){
        Collection<Double> points = new ArrayList<Double>();
        for(PDBAreaPoints point: this.tvar){
            points.add(point.getX());
            points.add(point.getY());
        }
        return points;
    }
}
