package PDB.dao;

import PDB.entity.PDBCategory;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository
public interface CategoryDao extends JpaRepository<PDBCategory, Long> {
}
