package PDB.service;

import PDB.dao.UserDao;
import PDB.entity.PDBUser;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.context.config.ResourceNotFoundException;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
public class UserService {

    @Autowired
    private UserDao userDao;

    public ResponseEntity<Collection<PDBUser>> getAllUsers() {
        Collection<PDBUser> users = this.userDao.findAll();
        if(users != null){
            return ResponseEntity.ok().body(users);
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    public ResponseEntity<PDBUser> getUserById(Long id) {
        PDBUser user = this.userDao.findOne(id);
        if(user != null){
            return ResponseEntity.ok().body(user);
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    public ResponseEntity<PDBUser> removeUserById(Long id) {
        try {
            this.userDao.delete(id);
            return ResponseEntity.noContent().build();
        } catch (ResourceNotFoundException e) {
            return ResponseEntity.notFound().build();
        }
    }

    public ResponseEntity<PDBUser> updateUser(PDBUser user) {
        PDBUser usr = this.userDao.save(user);
        return ResponseEntity.ok().body(usr);
    }

    public ResponseEntity<PDBUser> createUser(PDBUser user) {
        PDBUser usr = this.userDao.save(user);
        return ResponseEntity.ok().body(usr);
    }
}
