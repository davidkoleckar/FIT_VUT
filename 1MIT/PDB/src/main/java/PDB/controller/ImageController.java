package PDB.controller;

import PDB.entity.*;
import PDB.service.CategoryService;
import PDB.service.ImageService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Collection;

/**
 * Trida pro obsluhu requestu obrazku
 */
@RestController
@RequestMapping("/image")
public class ImageController {

    @Autowired
    private ImageService imageService;

    /**
     * Ulozi base64 obrazek do DB
     * @param vozidlo
     * @param data
     * @return
     */
    @RequestMapping( value = "/{vozidlo}", method = RequestMethod.POST)
    public ResponseEntity<PDBImage> saveBase64ToDb(@PathVariable("vozidlo") long vozidlo, @RequestBody ImageRequest data){

        return imageService.saveBase64ToDb(vozidlo,data.getData());
    }

    /**
     * Ziskani obrazku dle ID
     * @param id
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.GET)
    public ResponseEntity<String> getImageById(@PathVariable("id") long id){
        return imageService.getBase64FromDB(id);
    }

    /**
     * Rotace obrazku dle ID
     * @param id
     * @return
     */
    @RequestMapping(value = "/rotate/{id}", method = RequestMethod.GET)
    public ResponseEntity<ImageRequest> getRotateImageById(@PathVariable("id") long id){
        return imageService.getRotateImage(id);
    }

    /**
     * Ziskani podobnyhc obrazku dle ID
     * @param id
     * @return
     */
    @RequestMapping(path = "/similar/{id}", method = RequestMethod.GET)
    public ResponseEntity<Collection<ImageTheMostSimilar>> getTheMostSimilar(@PathVariable("id") long id){
        return imageService.getTheMostSimilar(id);
    }

    /**
     * Smazani obrazku dle ID
     * @param id
     * @return
     */
    @RequestMapping(path = "/{id}", method = RequestMethod.DELETE)
    public ResponseEntity<Void> deleteImage(@PathVariable("id") long id){
        return imageService.deleteImage(id);
    }

}