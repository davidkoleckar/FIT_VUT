package PDB.dao;

import PDB.entity.PDBImage;
import PDB.entity.PDBvehicle;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository()
public interface ImageDao extends JpaRepository<PDBImage, Long>, ImageCustomDao {
}
