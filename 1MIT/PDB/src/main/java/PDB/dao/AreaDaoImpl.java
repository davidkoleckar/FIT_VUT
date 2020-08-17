package PDB.dao;

import PDB.entity.PDBArea;
import PDB.entity.PDBAreaPoints;
import PDB.entity.PDBDefaultEntity;
import PDB.entity.PDBvehicle;
import io.swagger.models.auth.In;
import oracle.jdbc.OraclePreparedStatement;
import oracle.spatial.geometry.JGeometry;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;

import java.io.IOException;
import java.sql.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

/**
 * Trida zodpoveda za pristup k databazi pro prostorova data
 */
public class AreaDaoImpl implements AreaCustomDao {

    private static class AreaRowMapper implements RowMapper<PDBArea> {

        @Override
        public PDBArea mapRow(ResultSet resultSet, int i) throws SQLException {
            PDBArea areal = new PDBArea();
            byte[] image = resultSet.getBytes("tvar");

            try {
                JGeometry jGeometry = JGeometry.load(image);
                areal.setTvar(jGeometry);
            } catch (Exception e){
                System.out.println(e);
            }
            areal.setId(resultSet.getInt("id"));
            areal.setNazev(resultSet.getString("nazev"));
            areal.setTyp(resultSet.getString("typ"));
            areal.setTypEntity(resultSet.getString("typ_entity"));
            return areal;
        }
    }

    @Autowired
    private JdbcTemplate jdbcTemplate;

    /**
     * Metoda pro ziskani prostorovych entit, krome parkovist
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<PDBArea> getAreas() throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<PDBArea> pdbAreas = new ArrayList<PDBArea>();
        AreaRowMapper areaRowMapper = new AreaRowMapper();
        try{
            String sql = "SELECT *  from areal WHERE typ <> 'parkoviste'";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            ResultSet resultSet = oraclePreparedStatement.executeQuery();
            while(resultSet.next()){
                pdbAreas.add(areaRowMapper.mapRow(resultSet, 0));
            }
            resultSet.close();
            oraclePreparedStatement.close();
        } catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return pdbAreas;
    }

    /**
     * Metoda pro ziskani parkovacich mist v dane sekci
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<PDBArea> getParkingSpots(Integer id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<PDBArea> pdbAreas = new ArrayList<PDBArea>();
        AreaRowMapper areaRowMapper = new AreaRowMapper();
        try{
            String sql = "SELECT b.id, b.typ, b.tvar, b.typ_entity, b.nazev  from areal a, areal b WHERE b.typ = 'parkoviste' AND a.id = ? AND a.id <> b.id AND SDO_RELATE(b.tvar, a.tvar, 'MASK=inside+coveredby') = 'TRUE'";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            oraclePreparedStatement.setInt(1,id);
            ResultSet resultSet = oraclePreparedStatement.executeQuery();
            while(resultSet.next()){
                pdbAreas.add(areaRowMapper.mapRow(resultSet,0));
            }
            resultSet.close();
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return pdbAreas;
    }

    /**
     * Metoda pro vytvoreni entity
     * @param area
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public PDBArea createArea(PDBDefaultEntity area) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        connection.setAutoCommit(false);
        PDBArea pdbArea = null;
        String insertSql;
        OraclePreparedStatement oraclePreparedStatement;
        try{
            switch (area.getTypEntity()){
                case "kruh":
                    // first is center of circle
                    // second is r
                    PDBAreaPoints bottom = new PDBAreaPoints();
                    PDBAreaPoints right = new PDBAreaPoints();
                    PDBAreaPoints top = new PDBAreaPoints();
                    List<PDBAreaPoints> points = new ArrayList<>(area.getTvar());
                    PDBAreaPoints tmp =  points.get(0);

                    Double r = points.get(1).getX();
                    bottom.setX(tmp.getX());
                    bottom.setY(tmp.getY() - r);

                    right.setX(tmp.getX() + r);
                    right.setY(tmp.getY());

                    top.setX(tmp.getX());
                    top.setY(tmp.getY() + r);

                    insertSql = "INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES(?, ?, SDO_GEOMETRY(2003, NULL, NULL," +
                            "SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(?,?,?,?,?,?)), ?)";
                    oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(insertSql);
                    oraclePreparedStatement.setString(1, area.getNazev());
                    oraclePreparedStatement.setString(2, area.getTyp());
                    oraclePreparedStatement.setDouble(3, bottom.getX());
                    oraclePreparedStatement.setDouble(4, bottom.getY());
                    oraclePreparedStatement.setDouble(5, right.getX());
                    oraclePreparedStatement.setDouble(6, right.getY());
                    oraclePreparedStatement.setDouble(7, top.getX());
                    oraclePreparedStatement.setDouble(8, top.getY());
                    oraclePreparedStatement.setString(9, area.getTypEntity());
                    oraclePreparedStatement.executeQuery();
                    oraclePreparedStatement.close();
                    break;
                case "ctverec":
                case "trojuhelnik":
                case "elko":
                    insertSql = "INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES(?, ?, SDO_GEOMETRY(2003, NULL, NULL," +
                            "SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY("+ area.pointStringArray() +")),?)";
                    oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(insertSql);
                    oraclePreparedStatement.setString(1, area.getNazev());
                    oraclePreparedStatement.setString(2, area.getTyp());
                    oraclePreparedStatement.setString(3, area.getTypEntity());
                    oraclePreparedStatement.executeQuery();
                    oraclePreparedStatement.close();
                    break;
                case "oval":
                    insertSql = "INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES(?, ?, SDO_GEOMETRY(2003, NULL, NULL," +
                            "SDO_ELEM_INFO_ARRAY(1,1005,4, 1,2,2, 5,2,1, 7,2,2, 11,2,1),SDO_ORDINATE_ARRAY("+ area.pointStringArray() +")), ?)";
                    oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(insertSql);
                    oraclePreparedStatement.setString(1, area.getNazev());
                    oraclePreparedStatement.setString(2, area.getTyp());
                    oraclePreparedStatement.setString(3, area.getTypEntity());
                    oraclePreparedStatement.executeQuery();
                    oraclePreparedStatement.close();
                    break;
            }
            Statement statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT * FROM areal ORDER BY id DESC");
            resultSet.next();
            pdbArea = new AreaRowMapper().mapRow(resultSet, 0);
        } catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return pdbArea;
    }

    /**
     * Metoda pro aktualizaci entity
     * @param area
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public PDBArea updateArea(PDBDefaultEntity area) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        connection.setAutoCommit(false);
        PDBArea pdbArea = null;
        String updateSql;
        OraclePreparedStatement oraclePreparedStatement;
        try{
            switch (area.getTypEntity()){
                case "kruh":

                    // first is center of circle
                    // second is r
                    PDBAreaPoints bottom = new PDBAreaPoints();
                    PDBAreaPoints right = new PDBAreaPoints();
                    PDBAreaPoints top = new PDBAreaPoints();
                    List<PDBAreaPoints> points = new ArrayList<>(area.getTvar());
                    PDBAreaPoints tmp =  points.get(0);

                    Double r = points.get(1).getX();

                    bottom.setX(tmp.getX());
                    bottom.setY(tmp.getY() - r);

                    right.setX(tmp.getX() + r);
                    right.setY(tmp.getY());

                    top.setX(tmp.getX());
                    top.setY(tmp.getY() + r);

                    updateSql = "UPDATE areal SET tvar = SDO_GEOMETRY(2003, NULL, NULL," +
                            " SDO_ELEM_INFO_ARRAY(1,1003, 4), SDO_ORDINATE_ARRAY(?,?,?,?,?,?)), nazev = ? WHERE id = ?";
                    oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(updateSql);
                    oraclePreparedStatement.setDouble(1, bottom.getX());
                    oraclePreparedStatement.setDouble(2, bottom.getY());
                    oraclePreparedStatement.setDouble(3, right.getX());
                    oraclePreparedStatement.setDouble(4, right.getY());
                    oraclePreparedStatement.setDouble(5, top.getX());
                    oraclePreparedStatement.setDouble(6, top.getY());
                    oraclePreparedStatement.setString(7, area.getNazev());
                    oraclePreparedStatement.setLong(8, area.getId());
                    oraclePreparedStatement.executeQuery();
                    oraclePreparedStatement.close();
                    break;
                case "ctverec":
                case "trojuhelnik":
                case "elko":
                    updateSql = "UPDATE areal SET tvar = SDO_GEOMETRY(2003, NULL, NULL," +
                            " SDO_ELEM_INFO_ARRAY(1,1003, 1), SDO_ORDINATE_ARRAY(" + area.pointStringArray() + ")), nazev = ? WHERE id = ?";
                    oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(updateSql);
                    oraclePreparedStatement.setString(1, area.getNazev());
                    oraclePreparedStatement.setLong(2, area.getId());
                    oraclePreparedStatement.executeQuery();
                    oraclePreparedStatement.close();
                    break;
                case "oval":
                    updateSql = "UPDATE areal SET tvar = SDO_GEOMETRY(2003, NULL, NULL," +
                            " SDO_ELEM_INFO_ARRAY(1,1005,4, 1,2,2, 5,2,1, 7,2,2, 11,2,1), SDO_ORDINATE_ARRAY(" + area.pointStringArray() + ")), nazev = ? WHERE id = ?";
                    oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(updateSql);
                    oraclePreparedStatement.setString(1, area.getNazev());
                    oraclePreparedStatement.setLong(2, area.getId());
                    oraclePreparedStatement.executeQuery();
                    oraclePreparedStatement.close();
                    break;
            }
            Statement statement = connection.createStatement();
            ResultSet resultSet = statement.executeQuery("SELECT * FROM areal ORDER BY id DESC");
            resultSet.next();
            pdbArea = new AreaRowMapper().mapRow(resultSet, 0);
        } catch (SQLException e){
            System.out.println(e);
        }
        connection.close();

        return pdbArea;
    }

    /**
     * Metoda pro ziskani volnych parkovacich mist
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<Integer> getEmptySpots() throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<Integer> spots = new ArrayList<Integer>();
        try{
            String sql = "SELECT id from areal WHERE typ = 'parkoviste' AND id NOT IN " +
            "(SELECT umisteni as id FROM vozidla WHERE valid_to IS NULL AND umisteni IS NOT NULL)";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            ResultSet rs = oraclePreparedStatement.executeQuery();
            while(rs.next()){
                spots.add(rs.getInt("id"));
            }
            rs.close();
            oraclePreparedStatement.close();
        } catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return spots;
    }


    /**
     * Metoda pro ziskani obsahu entity
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Double getAreaSpace(Integer id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        try{
            String sql = "SELECT SDO_GEOM.SDO_AREA(tvar, 1) as area FROM areal WHERE id = ?";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            oraclePreparedStatement.setInt(1, id);
            ResultSet rs = oraclePreparedStatement.executeQuery();
            rs.next();
            Double area = rs.getDouble("area");
            rs.close();
            return area;
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return null;
    }

    /**
     * Metoda pro ziskani vzdalenosti mezi entitami
     * @param idA
     * @param idB
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Double getDistanceBetweenAreas(Integer idA, Integer idB) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        try{
            String sql = "SELECT SDO_GEOM.SDO_DISTANCE(a.tvar, b.tvar, 1) as distance FROM areal a, areal b WHERE a.id = ? AND b.id = ?";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            oraclePreparedStatement.setInt(1, idA);
            oraclePreparedStatement.setInt(2, idB);
            ResultSet rs = oraclePreparedStatement.executeQuery();
            rs.next();
            Double distance = rs.getDouble("distance");
            rs.close();
            return distance;
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return null;

    }

    /**
     * Metoda pro odstraneni entity
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Void removeArea(Integer id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        try{
            String sql = "DELETE from areal WHERE id  = ?";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            oraclePreparedStatement.setInt(1, id);
            oraclePreparedStatement.executeQuery();
            oraclePreparedStatement.close();
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return null;
    }

    /**
     * Metoda pro ziskani prumerne ceny vozidla v dane sekci
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Double getAverageCarPrice(Integer id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Double prumernaCena = 0.0;
        try{
            String sql = "SELECT avg(cena) as averagePrice from vozidla where valid_to IS NULL AND umisteni IN (SELECT b.id  from areal a, areal b WHERE b.typ = 'parkoviste' AND a.id = ? AND a.id <> b.id AND SDO_RELATE(b.tvar, a.tvar, 'MASK=inside+coveredby') = 'TRUE' )";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            oraclePreparedStatement.setInt(1, id);
            ResultSet resultSet = oraclePreparedStatement.executeQuery();
            resultSet.next();
            prumernaCena = resultSet.getDouble("averagePrice");
            resultSet.close();
            oraclePreparedStatement.close();
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return prumernaCena;
    }

    /**
     * Metoda pro ziskani obvodu dane sekce
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Double getLength(Integer id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        try{
            String sql = "SELECT SDO_GEOM.SDO_LENGTH(tvar, 1) as areaLength FROM areal WHERE id = ?";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sql);
            oraclePreparedStatement.setInt(1, id);
            ResultSet rs = oraclePreparedStatement.executeQuery();
            rs.next();
            return rs.getDouble("areaLength");
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return null;
    }

    /**
     * Metoda ori ziskani vozidel v danych sekcich na spadajicich parkovistich
     * @param idA
     * @param idB
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<PDBvehicle> getVehiclesInAreas(Integer idA, Integer idB) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<PDBvehicle> pdBvehicles = new ArrayList<PDBvehicle>();
        VehicleRowMapper vehicleRowMapper = new VehicleRowMapper();

        try{
            String ids = "";
            String vehicleIDSql = "SELECT b.id FROM areal b WHERE b.typ = 'parkoviste' AND SDO_RELATE(b.tvar, (SELECT SDO_GEOM.SDO_UNION(a1.tvar, a2.tvar, 0.1) as ar FROM areal a1, areal a2 WHERE a1.id = ? AND a2.id = ?), 'MASK=inside') = 'TRUE'";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(vehicleIDSql);
            oraclePreparedStatement.setInt(1, idA);
            oraclePreparedStatement.setInt(2, idB);
            ResultSet resultSet = oraclePreparedStatement.executeQuery();
            while(resultSet.next()){
                ids = ids + resultSet.getString("id") + ",";
            }
            resultSet.close();
            ids = ids.substring(0, ids.length() - 1);
            oraclePreparedStatement.close();
            String vehiclesSql = "SELECT v.id,v.nakup,v.prodej,v.znacka,v.model,v.motor,v.rok,v.stk,v.kupni_cena,v.cena,v.kilometry," +
                    "v.stav,v.kategorie,v.umisteni,v.valid_from,v.valid_to,v.user_id,v.obrazek,o.photo FROM " +
                    "vozidla v LEFT JOIN obrazky o ON v.obrazek = o.id  WHERE v.valid_to IS NULL AND v.umisteni IN(" + ids +")" ;
            oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(vehiclesSql);
            resultSet = oraclePreparedStatement.executeQuery();
            while(resultSet.next()){
                pdBvehicles.add(vehicleRowMapper.mapRow(resultSet, 0));
            }
            resultSet.close();
            oraclePreparedStatement.close();

        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return pdBvehicles;
    }
}
