package PDB.service;

import PDB.dao.CategoryDao;
import PDB.entity.PDBCategory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.context.config.ResourceNotFoundException;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import java.util.Collection;

@Service
public class CategoryService {

    @Autowired
    private CategoryDao categoryDao;

    public ResponseEntity<Collection<PDBCategory>> getAllCategories() {
        Collection<PDBCategory> users = this.categoryDao.findAll();
        if(users != null){
            return ResponseEntity.ok().body(users);
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    public ResponseEntity<PDBCategory> getCategoryById(Long id) {
        PDBCategory category = this.categoryDao.findOne(id);
        if(category != null){
            return ResponseEntity.ok().body(category);
        } else {
            return ResponseEntity.notFound().build();
        }
    }

    public ResponseEntity<PDBCategory> removeCategoryById(Long id) {
        try {
            this.categoryDao.delete(id);
            return ResponseEntity.noContent().build();
        } catch (ResourceNotFoundException e) {
            return ResponseEntity.notFound().build();
        }
    }

    public ResponseEntity<PDBCategory> updateCategory(PDBCategory category) {
        PDBCategory c = this.categoryDao.save(category);
        return ResponseEntity.ok().body(c);
    }

    public ResponseEntity<PDBCategory> createCategory(PDBCategory category) {
        PDBCategory c = this.categoryDao.save(category);
        return ResponseEntity.ok().body(c);
    }

}
