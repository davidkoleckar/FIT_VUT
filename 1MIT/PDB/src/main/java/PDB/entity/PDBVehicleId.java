package PDB.entity;


import java.io.Serializable;
import java.sql.Date;

/**
 * Trida pro serializaci id vozidla
 */
public class PDBVehicleId implements Serializable {
    private Long id;
    private Date validFrom;
}
