package PDB.dao;

import PDB.entity.PDBArea;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.Collection;

@Repository
public interface AreaDao extends JpaRepository<PDBArea, Integer>, AreaCustomDao {
}

