package PDB.controller;

import PDB.entity.PDBCategory;
import PDB.entity.PDBUser;
import PDB.service.CategoryService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;

/**
 * Trida pro zpracovani requestu kategorie
 */
@RestController
@RequestMapping("/category")
public class CategoryController {

    @Autowired
    private CategoryService categoryService;

    /**
     * Ziskani vsech kategorii
     * @return
     */
    @RequestMapping(method = RequestMethod.GET)
    public ResponseEntity<Collection<PDBCategory>> getAllCategories() {
        return categoryService.getAllCategories();
    }

    /**
     * Vytvoreni nove kategorie
     * @param category
     * @return
     */
    @RequestMapping(method = RequestMethod.POST)
    public ResponseEntity<PDBCategory> createCategory(@RequestBody PDBCategory category){
        return categoryService.createCategory(category);
    }

    /**
     * Ziskani kategorie na zaklade id
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.GET)
    public ResponseEntity<PDBCategory> getCategoryById(@PathVariable("id") long id){
        return categoryService.getCategoryById(id);
    }

    /**
     * Upraveni kategorie na zaklade id
     * @param category
     * @return
     */
    @RequestMapping(method = RequestMethod.PUT)
    public ResponseEntity<PDBCategory> editCategory(@RequestBody PDBCategory category){
        return categoryService.updateCategory(category);
    }

    /**
     * Smazani kategorie na zaklade id
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.DELETE)
    public ResponseEntity<PDBCategory> deleteCategoryById(@PathVariable("id") long id){
        return categoryService.removeCategoryById(id);
    }
}
