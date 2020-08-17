package PDB.dao;

import PDB.entity.PDBUser;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

@Repository()
public interface UserDao extends JpaRepository<PDBUser, Long> {
    PDBUser findByEmail(String email);
}
