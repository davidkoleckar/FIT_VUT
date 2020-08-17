package PDB.dao;

import PDB.entity.PDBvehicle;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository()
public interface VehicleDao extends JpaRepository<PDBvehicle, Long>, VehicleCustomDao {
}
