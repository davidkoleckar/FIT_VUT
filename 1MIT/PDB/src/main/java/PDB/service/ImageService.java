package PDB.service;


import PDB.dao.ImageDao;
import PDB.entity.ImageRequest;
import PDB.entity.ImageTheMostSimilar;
import PDB.entity.PDBImage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Collection;


@Service
public class ImageService {

    @Autowired
    private ImageDao imageDao;

    public ResponseEntity<String> getBase64FromDB(Long id) {
        String data = null;
        try {
            data = this.imageDao.getBase64FromDB(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(data);
    }

    public ResponseEntity<ImageRequest> getRotateImage(Long id) {
        ImageRequest data = null;
        try {
            data = this.imageDao.getRotateImage(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(data);
    }

    public ResponseEntity<Void> deleteImage(Long id) {
        try {
            this.imageDao.deleteImage(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok().build();
    }

    public ResponseEntity<PDBImage> saveBase64ToDb(Long vozidlo, String data) {
        try {
            this.imageDao.saveBase64ToDb(vozidlo, data);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok().build();
    }

    public ResponseEntity<Collection<ImageTheMostSimilar>> getTheMostSimilar(Long id){
        Collection<ImageTheMostSimilar> temp = null;
        try {
            temp = this.imageDao.getTheMostSimilar(id);
        } catch (SQLException e) {
            ResponseEntity.badRequest().build();
        } catch (IOException e) {
            ResponseEntity.badRequest().build();
        }
        return ResponseEntity.ok(temp);
    }

}
