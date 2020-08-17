package PDB.dao;

import PDB.entity.ImageRequest;
import PDB.entity.ImageTheMostSimilar;
import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;

import java.io.IOException;
import java.sql.*;
import java.util.ArrayList;
import java.util.Base64;
import java.util.Collection;

/**
 * Trida implementuje operace nad multimedialnimi daty
 * @author David Koleckar
 */
public class ImageDaoImpl implements ImageCustomDao {

    @Autowired
    private JdbcTemplate jdbcTemplate;

    /**
     * Ulozi obrazek z BASE64 do DB
     * @param vozidlo
     * @param data
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public void saveBase64ToDb(Long vozidlo, String data)  throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();

        connection.setAutoCommit(false);
        OracleResultSet rs;

        //insert a mew record with an empty ORDImage object
        String insertSQL = "INSERT INTO obrazky (photo) VALUES (ordsys.ordimage.init())";
        Statement stmt1 = connection.createStatement();
        stmt1.executeUpdate(insertSQL);
        stmt1.close();

        //retrieve the previously created ORDImage object for future updating
        String selectSQL = "SELECT id, photo FROM obrazky ORDER BY id DESC FOR UPDATE";
        Statement stmt2 = connection.createStatement();
        rs = (OracleResultSet) stmt2.executeQuery(selectSQL);
        rs.next();
        Integer id = rs.getInt("id");
        OrdImage imgProxy = (OrdImage) rs.getORAData("photo", OrdImage.getORADataFactory());;
        rs.close();
        stmt2.close();

        //load the media data from a file to the ORDImage JAVA object
        byte[] scanBytes = Base64.getDecoder().decode(data);
        imgProxy.loadDataFromByteArray(scanBytes);
        imgProxy.setProperties();

        //update table with ORDImage Java object
        String updateSQL = "UPDATE obrazky SET photo=? WHERE id ="+id;
        OraclePreparedStatement pstmt = (OraclePreparedStatement) connection.prepareStatement(updateSQL);
        pstmt.setORAData(1,imgProxy);
        pstmt.executeUpdate();
        pstmt.close();

        //update the table with StillImage object and features
        Statement stmt3 = connection.createStatement();
        String updateSQL2 = "UPDATE obrazky o SET o.photo_si=SI_StillImage(o.photo.getContent()) WHERE id=" + id;
        stmt3.executeUpdate(updateSQL2);
        String updateSQL3 = "update obrazky o set"
                + " o.photo_ac=SI_AverageColor(o.photo_si),"
                + " o.photo_ch=SI_ColorHistogram(o.photo_si),"
                + " o.photo_pc=SI_PositionalColor(o.photo_si),"
                + " o.photo_tx=SI_Texture(o.photo_si)"
                + " where id = " + id;
        stmt3.executeUpdate(updateSQL3);
        stmt3.close();

        //update vozidlo image id
        Statement stmt4 = connection.createStatement();
        String updateSQL4 = "UPDATE vozidla SET obrazek = " + id + " WHERE id=" + vozidlo;
        stmt4.executeUpdate(updateSQL4);
        stmt4.close();

        connection.commit();

        connection.close();
    }

    /**
     * Dostane obrazek z databaze a prekoduje na BASE64
     * @param id vozidla
     * @return base64 image
     * @throws SQLException
     * @throws IOException
     */
    public String getBase64FromDB(Long id) throws SQLException, IOException{

        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Integer id_obrazek = 0;
        String data = null;

        //get id obrazku from DB
        try {
            String selectSQL = "SELECT obrazek FROM vozidla WHERE id = " + id;
            Statement stmt = connection.createStatement();
            OracleResultSet rs = (OracleResultSet) stmt.executeQuery(selectSQL);
            rs.next();
            id_obrazek = rs.getInt("obrazek");
            rs.close();
            stmt.close();
        }catch (SQLException e){
            e.printStackTrace();
        }
        //get photo base64
        try {
            Statement stmt2 = connection.createStatement();
            OracleResultSet rs2 = (OracleResultSet) stmt2.executeQuery("SELECT photo FROM obrazky WHERE id="+id_obrazek);
            rs2.next();
            OrdImage imgProxy = (OrdImage) rs2.getORAData("photo", OrdImage.getORADataFactory());
            data = Base64.getEncoder().encodeToString(imgProxy.getDataInByteArray());
            rs2.close();
            stmt2.close();
        }catch (SQLException e){
            e.printStackTrace();
        }

        connection.close();

        return data;
    }

    /**
     * Vykona operaci rotace nad danym obrazkem
     * @param id vozidla
     * @return base64 image
     * @throws SQLException
     * @throws IOException
     */
    public ImageRequest getRotateImage(Long id) throws SQLException, IOException{
        Connection connection = jdbcTemplate.getDataSource().getConnection();

        //get id obrazku from DB
        String selectSQL = "SELECT obrazek FROM vozidla WHERE id = " + id;
        Statement stmt = connection.createStatement();
        OracleResultSet rs = (OracleResultSet) stmt.executeQuery(selectSQL);
        rs.next();
        Integer id_obrazek = rs.getInt("obrazek");
        rs.close();
        stmt.close();

        PreparedStatement stmt2 = connection.prepareStatement("CALL rotate(?)");
        stmt2.setLong(1,id_obrazek);
        stmt2.execute();
        stmt2.close();

        //update the table with StillImage object and features
        Statement stmt3 = connection.createStatement();
        String updateSQL = "UPDATE obrazky o SET o.photo_si=SI_StillImage(o.photo.getContent()) WHERE id=" + id_obrazek;
        stmt3.executeUpdate(updateSQL);
        String updateSQL2 = "update obrazky o set"
                + " o.photo_ac=SI_AverageColor(o.photo_si),"
                + " o.photo_ch=SI_ColorHistogram(o.photo_si),"
                + " o.photo_pc=SI_PositionalColor(o.photo_si),"
                + " o.photo_tx=SI_Texture(o.photo_si)"
                + " where id =" + id_obrazek;
        stmt3.executeUpdate(updateSQL2);
        stmt3.close();

        connection.close();

        ImageRequest i = new ImageRequest();
        i.setData(getBase64FromDB(id));

        return i;
    }

    /**
     * Smaze obrazek z DB
     * @param id vozidla
     * @throws SQLException
     * @throws IOException
     */
    public void deleteImage(Long id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();

        String selectSQL = "SELECT obrazek FROM vozidla v WHERE v.id = " + id;
        Statement stmt = connection.createStatement();
        OracleResultSet rs = (OracleResultSet) stmt.executeQuery(selectSQL);
        rs.next();
        Integer id_obrazek = rs.getInt("obrazek");
        rs.close();
        stmt.close();

        String updateSQL = "UPDATE vozidla SET obrazek=null WHERE id = " + id;
        Statement stmt2 = connection.createStatement();
        stmt2.executeQuery(updateSQL);
        stmt2.close();

        String deleteSQL = "DELETE FROM obrazky WHERE id = " + id_obrazek;
        Statement stmt3 = connection.createStatement();
        stmt3.executeQuery(deleteSQL);
        stmt3.close();

        connection.close();
    }

    /**
     * Metoda pro nalezeni nejpodobnejsich automobilu dle obrazku
     * @param id vybraneho vozidla
     * @return Seznam obsahujici id vozidla a base64 obrazek
     * @throws SQLException
     * @throws IOException
     */
    public Collection<ImageTheMostSimilar> getTheMostSimilar(Long id) throws SQLException, IOException{
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        double weightAC = 0.3;
        double weightCH = 0.3;
        double weightPC = 0.3;
        double weightTX = 0.1;

        //get id obrazku from DB
        String selectSQL = "SELECT obrazek FROM vozidla WHERE id = " + id;
        Statement stmt = connection.createStatement();
        OracleResultSet rs = (OracleResultSet) stmt.executeQuery(selectSQL);
        rs.next();
        Integer id_obrazek = rs.getInt("obrazek");
        rs.close();
        stmt.close();
        Collection<ImageTheMostSimilar> data = new ArrayList<ImageTheMostSimilar>();

        try {
            PreparedStatement pstmtSelect = connection.prepareStatement(
                    "SELECT dst.photo, v.id,  SI_ScoreByFtrList("
                            + "new SI_FeatureList(src.photo_ac,?,src.photo_ch,?,src.photo_pc,?,src.photo_tx,?),dst.photo_si)"
                            + " as similarity FROM obrazky src, obrazky dst JOIN vozidla v ON v.obrazek = dst.id "
                            + "WHERE src.id = ? AND dst.id <> src.id AND (v.valid_to IS NULL OR v.prodej IS NOT NULL) ORDER BY similarity ASC");

            pstmtSelect.setDouble(1, weightAC);
            pstmtSelect.setDouble(2, weightCH);
            pstmtSelect.setDouble(3, weightPC);
            pstmtSelect.setDouble(4, weightTX);
            pstmtSelect.setLong(5, id_obrazek);

            OracleResultSet rs2 = (OracleResultSet) pstmtSelect.executeQuery();
            while(rs2.next()){


                ImageTheMostSimilar temp = new ImageTheMostSimilar();
                OrdImage imgProxy = (OrdImage) rs2.getORAData("photo", OrdImage.getORADataFactory());
                temp.setId(rs2.getLong("id"));
                temp.setData(Base64.getEncoder().encodeToString(imgProxy.getDataInByteArray()));
                data.add(temp);
            }
            pstmtSelect.close();
            rs2.close();
        }catch (SQLException n)
        {
            n.printStackTrace();
        }


        connection.close();

        return data;
    }

}
