package PDB.dao;


import PDB.entity.ImageRequest;
import PDB.entity.ImageTheMostSimilar;
import org.springframework.stereotype.Repository;

import java.io.IOException;
import java.sql.SQLException;
import java.util.Collection;

/**
 * Interface obsahujici metody pro praci s obrazky
 */
@Repository()
public interface ImageCustomDao {
    public void saveBase64ToDb(Long vozidlo, String data) throws SQLException, IOException;
    public String getBase64FromDB(Long id) throws SQLException, IOException;
    public ImageRequest getRotateImage(Long id) throws SQLException, IOException;
    public void deleteImage(Long id) throws SQLException, IOException;
    public Collection<ImageTheMostSimilar> getTheMostSimilar(Long id) throws SQLException, IOException;
}
