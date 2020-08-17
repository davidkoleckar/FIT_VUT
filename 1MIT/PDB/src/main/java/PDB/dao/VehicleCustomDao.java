package PDB.dao;

import PDB.entity.PDBUserPrice;
import PDB.entity.PDBUserVehicle;
import PDB.entity.PDBvehicle;
import oracle.sql.DATE;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Collection;

public interface VehicleCustomDao {
    public PDBvehicle createVehicle(PDBvehicle vehicle) throws SQLException, IOException;
    public PDBvehicle updateVehicle(PDBvehicle vehicle) throws SQLException, IOException;
    public void deleteVehicle(Long id) throws SQLException, IOException;
    public Collection<PDBUserVehicle> getEditedVehicles(DATE from, DATE to) throws SQLException, IOException;
    public Collection<PDBvehicle> getVehicles(Boolean sold) throws SQLException, IOException;
    public PDBvehicle getVehicle(Long id) throws SQLException, IOException;
    public Collection<PDBUserPrice> getVehiclePrices(Long id) throws SQLException, IOException;
}
