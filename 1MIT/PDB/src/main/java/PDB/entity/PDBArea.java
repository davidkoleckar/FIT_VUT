package PDB.entity;

import javax.persistence.*;

import com.fasterxml.jackson.databind.annotation.JsonSerialize;
import oracle.spatial.geometry.JGeometry;

@Entity
@Table(name = "areal")
@JsonSerialize(using = PDBAreaSerializer.class)
public class PDBArea {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;
    private String nazev;
    private JGeometry tvar;
    private String typ;
    private String typEntity;

    public PDBArea(Integer id, String nazev, JGeometry tvar, String typ, String typEntity) {
        this.id = id;
        this.nazev = nazev;
        this.tvar = tvar;
        this.typ = typ;
        this.typEntity = typEntity;
    }

    public String getTyp() {
        return typ;
    }

    public void setTyp(String typ) {
        this.typ = typ;
    }

    public PDBArea() {
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public String getNazev() {
        return nazev;
    }

    public void setNazev(String nazev) {
        this.nazev = nazev;
    }

    public JGeometry getTvar() {
        return tvar;
    }

    public void setTvar(JGeometry tvar) {
        this.tvar = tvar;
    }

    public String getTypEntity() {
        return typEntity;
    }

    public void setTypEntity(String typEntity) {
        this.typEntity = typEntity;
    }
}
