package PDB.dao;

import PDB.entity.PDBArea;
import PDB.entity.PDBDefaultEntity;
import PDB.entity.PDBvehicle;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Collection;

public interface AreaCustomDao {
    public Collection<PDBArea> getAreas() throws SQLException, IOException;
    public Collection<PDBArea> getParkingSpots(Integer id) throws SQLException, IOException;
    public PDBArea createArea(PDBDefaultEntity area) throws SQLException, IOException;
    public PDBArea updateArea(PDBDefaultEntity area) throws SQLException, IOException;
    public Collection<Integer> getEmptySpots() throws SQLException, IOException;
    public Double getAreaSpace(Integer id) throws SQLException, IOException;
    public Double getDistanceBetweenAreas(Integer idA,  Integer idB) throws SQLException, IOException;
    public Void removeArea(Integer id) throws SQLException, IOException;
    public Double getAverageCarPrice(Integer id) throws SQLException, IOException;
    public Double getLength(Integer id) throws SQLException, IOException;
    public Collection<PDBvehicle> getVehiclesInAreas(Integer idA, Integer idB) throws SQLException, IOException;
}