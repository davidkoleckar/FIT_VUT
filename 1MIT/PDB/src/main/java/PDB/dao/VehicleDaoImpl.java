package PDB.dao;

import PDB.entity.PDBUser;
import PDB.entity.PDBUserPrice;
import PDB.entity.PDBUserVehicle;
import PDB.entity.PDBvehicle;
import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;
import oracle.sql.DATE;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.security.core.context.SecurityContextHolder;

import java.io.IOException;
import java.sql.*;
import java.util.ArrayList;
import java.util.Base64;
import java.util.Collection;
import java.util.Objects;

public class VehicleDaoImpl implements VehicleCustomDao {


    private static class UserPriceRowMapper implements RowMapper<PDBUserPrice> {

        @Override
        public PDBUserPrice mapRow(ResultSet resultSet, int i) throws SQLException {
            PDBUserPrice pdbUserPrice = new PDBUserPrice();
            pdbUserPrice.setCena(resultSet.getLong("cena"));
            pdbUserPrice.setValidFrom(new DATE(resultSet.getDate("valid_from")));
            pdbUserPrice.setValidTo(new DATE(resultSet.getDate("valid_to")));
            pdbUserPrice.setJmeno(resultSet.getString("jmeno"));
            return pdbUserPrice;
        }
    }

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Autowired
    private UserDao userDao;

    /**
     * Metoda pro vytvoreni vozidla
     * @param vehicle
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public PDBvehicle createVehicle(PDBvehicle vehicle) throws SQLException, IOException {
        PDBUser user = userDao.findByEmail(SecurityContextHolder.getContext().getAuthentication().getName());
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        connection.setAutoCommit(false);

        String insertSql = "INSERT INTO vozidla (id, nakup, znacka, model, motor, rok, stk, cena, kupni_cena, kilometry, stav,umisteni, kategorie, valid_from, user_id)" +
               "VALUES (vehicle_seq.nextval,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss'), ?)";
       OraclePreparedStatement preparedStatement = (OraclePreparedStatement) connection.prepareStatement(insertSql);

       try{
           preparedStatement.setDATE(1,vehicle.getNakup());
           preparedStatement.setString(2,vehicle.getZnacka());
           preparedStatement.setString(3,vehicle.getModel());
           preparedStatement.setString(4,vehicle.getMotor());
           preparedStatement.setInt(5,vehicle.getRok());
           preparedStatement.setDATE(6,vehicle.getStk());
           preparedStatement.setLong(7,vehicle.getCena());
           preparedStatement.setLong(8,vehicle.getKupniCena());
           preparedStatement.setLong(9,vehicle.getKilometry());
           preparedStatement.setString(10,vehicle.getStav());
           preparedStatement.setInt(11,vehicle.getUmisteni());
           preparedStatement.setInt(12,vehicle.getKategorie());
           preparedStatement.setLong(13,user.getId());
           preparedStatement.execute();
           preparedStatement.close();
       } catch (SQLException e){
           System.out.println(e);
       }

        Statement stmt = connection.createStatement();
        OracleResultSet rs = (OracleResultSet) stmt.executeQuery("SELECT * FROM vozidla ORDER BY id DESC");
        rs.next();
        PDBvehicle pdbVehicle = new VehicleRowMapper().mapRow(rs, 0);
        rs.close();
        stmt.close();
        connection.commit();
        connection.close();
        return pdbVehicle;
    }

    /**
     * Metoda pro upravu vozidla
     * @param vehicle
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public PDBvehicle updateVehicle(PDBvehicle vehicle) throws SQLException, IOException {
        PDBUser user = userDao.findByEmail(SecurityContextHolder.getContext().getAuthentication().getName());
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        connection.setAutoCommit(false);

        try{
            String endSql = "UPDATE vozidla set valid_to = TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss') WHERE id = ? AND valid_from = ?";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(endSql);
            oraclePreparedStatement.setLong(1,vehicle.getId());
            oraclePreparedStatement.setDATE(2,vehicle.getValidFrom());
            oraclePreparedStatement.executeUpdate();
            oraclePreparedStatement.close();
        } catch (SQLException e){
            System.out.println(e);
        }


        String insertSql = "INSERT INTO vozidla (id, nakup, znacka, model, motor, rok, stk, cena, kupni_cena, kilometry, stav, umisteni, kategorie, valid_from, user_id, obrazek)" +
                "VALUES (?,?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?,TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss'), ?, ?)";
        try{
            OraclePreparedStatement preparedStatement = (OraclePreparedStatement) connection.prepareStatement(insertSql);
            preparedStatement.setLong(1,vehicle.getId());
            preparedStatement.setDATE(2,vehicle.getNakup());
            preparedStatement.setString(3,vehicle.getZnacka());
            preparedStatement.setString(4,vehicle.getModel());
            preparedStatement.setString(5,vehicle.getMotor());
            preparedStatement.setInt(6,vehicle.getRok());
            preparedStatement.setDATE(7,vehicle.getStk());
            preparedStatement.setLong(8,vehicle.getCena());
            preparedStatement.setLong(9,vehicle.getKupniCena());
            preparedStatement.setLong(10,vehicle.getKilometry());
            preparedStatement.setString(11,vehicle.getStav());
            preparedStatement.setInt(12,vehicle.getUmisteni());
            preparedStatement.setInt(13,vehicle.getKategorie());
            preparedStatement.setLong(14,user.getId());
            preparedStatement.setObject(15,vehicle.getObrazek());
            preparedStatement.execute();
            preparedStatement.close();
        }catch (SQLException e){
        }
        PDBvehicle pdbVehicle = null;
        try{
            String getSql = "SELECT v.id,v.nakup,v.prodej,v.znacka,v.model,v.motor,v.rok,v.stk,v.kupni_cena,v.cena,v.kilometry," +
            "v.stav,v.kategorie,v.umisteni,v.valid_from,v.valid_to,v.user_id,v.obrazek,o.photo FROM " +
                    "vozidla v LEFT JOIN obrazky o ON v.obrazek = o.id  WHERE v.id = ? AND v.valid_to IS NULL";
            OraclePreparedStatement preparedStatement1 = (OraclePreparedStatement) connection.prepareStatement(getSql);
            preparedStatement1.setLong(1,vehicle.getId());
            OracleResultSet rs = (OracleResultSet) preparedStatement1.executeQuery();
            rs.next();

            pdbVehicle = new VehicleRowMapper().mapRow(rs, 0);
            preparedStatement1.close();
        } catch (SQLException e){
            System.out.println(e);
        }

        connection.commit();
        connection.close();
        return pdbVehicle;
    }

    /**
     * Metoda pro smazani vozidla
     * @param id
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public void deleteVehicle(Long id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        try{
            String endSql = "UPDATE vozidla set valid_to = TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss')," +
                    "prodej  = TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss') WHERE id = ? AND valid_to IS NULL";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(endSql);
            oraclePreparedStatement.setLong(1,id);
            oraclePreparedStatement.executeUpdate();
            oraclePreparedStatement.close();
        } catch (SQLException e){
            System.out.println(e);
            throw e;
        }
        connection.close();
    }

    /**
     * Metoda pro ziskani editovanych vozidel v danem obdobi
     * @param from
     * @param to
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<PDBUserVehicle> getEditedVehicles(DATE from, DATE to) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<PDBUserVehicle> data = new ArrayList<PDBUserVehicle>();
        try{
            String getql = "SELECT v.id, v.znacka, v.cena,  v.rok, v.kategorie, v.valid_from, u.email, u.jmeno FROM vozidla v JOIN uzivatele u on v.user_id = u.id and (v.valid_from between ? and ?)";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(getql);
            oraclePreparedStatement.setDATE(1,from);
            oraclePreparedStatement.setDATE(2,to);
            OracleResultSet rs = (OracleResultSet) oraclePreparedStatement.executeQuery();
            while(rs.next()){
                PDBUserVehicle pdbUserVehicle = new PDBUserVehicle();
                pdbUserVehicle.setId(rs.getLong("id"));
                pdbUserVehicle.setZnacka(rs.getString("znacka"));
                pdbUserVehicle.setRok(rs.getInt("rok"));
                pdbUserVehicle.setKategorie(rs.getInt("kategorie"));
                pdbUserVehicle.setValidFrom(rs.getDATE("valid_from"));
                pdbUserVehicle.setEditedBy(rs.getString("email"));
                pdbUserVehicle.setCena(rs.getLong("cena"));
                pdbUserVehicle.setJmeno(rs.getString("jmeno"));
                data.add(pdbUserVehicle);
            }
            rs.close();
            oraclePreparedStatement.close();
        }catch (SQLException e){
            System.out.println(e);
        }

        connection.close();
        return data;
    }

    /**
     * Metoda pro ziskani platnych vozidel
     * @param sold
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<PDBvehicle> getVehicles(Boolean sold) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<PDBvehicle> data = new ArrayList<PDBvehicle>();
        try{
            String sqlGet = null;
            if(sold){
                sqlGet = "SELECT v.id,v.nakup,v.prodej,v.znacka,v.model,v.motor,v.rok,v.stk,v.kupni_cena,v.cena,v.kilometry," +
                        "v.stav,v.kategorie,v.umisteni,v.valid_from,v.valid_to,v.user_id,v.obrazek,o.photo FROM vozidla v " +
                        "LEFT JOIN obrazky o ON v.obrazek = o.id WHERE v.prodej IS NOT NULL";
            } else {
                sqlGet = "SELECT v.id,v.nakup,v.prodej,v.znacka,v.model,v.motor,v.rok,v.stk,v.kupni_cena,v.cena,v.kilometry," +
                        "v.stav,v.kategorie,v.umisteni,v.valid_from,v.valid_to,v.user_id,v.obrazek,o.photo FROM " +
                        "vozidla v LEFT JOIN obrazky o ON v.obrazek = o.id  WHERE v.valid_to IS NULL";
            }
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sqlGet);
            OracleResultSet rs = (OracleResultSet) oraclePreparedStatement.executeQuery();
            while(rs.next()){
                VehicleRowMapper vehicleRowMapper = new VehicleRowMapper();
                PDBvehicle pdbVehicle = vehicleRowMapper.mapRow(rs, 0);
                data.add(pdbVehicle);
            }
            oraclePreparedStatement.close();
            rs.close();
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return data;
    }

    /**
     * Metoda pro ziskani specifickeho vozidla
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public PDBvehicle getVehicle(Long id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        PDBvehicle pdBvehicle = null;
        try{
            String sqlGet = "SELECT v.id,v.nakup,v.prodej,v.znacka,v.model,v.motor,v.rok,v.stk,v.kupni_cena,v.cena,v.kilometry," +
                    "v.stav,v.kategorie,v.umisteni,v.valid_from,v.valid_to,v.user_id,v.obrazek,o.photo FROM vozidla v "+
                    "LEFT JOIN obrazky o ON v.obrazek = o.id WHERE v.id = ? AND (v.valid_to IS NULL OR v.prodej IS NOT NULL)";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sqlGet);
            oraclePreparedStatement.setLong(1, id);
            OracleResultSet rs = (OracleResultSet) oraclePreparedStatement.executeQuery();
            rs.next();
            pdBvehicle = new VehicleRowMapper().mapRow(rs, 0);
            rs.close();
            oraclePreparedStatement.close();
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return pdBvehicle;
    }

    /**
     * Metoda pro ziskani vyvoje ceny vozidla
     * @param id
     * @return
     * @throws SQLException
     * @throws IOException
     */
    @Override
    public Collection<PDBUserPrice> getVehiclePrices(Long id) throws SQLException, IOException {
        Connection connection = jdbcTemplate.getDataSource().getConnection();
        Collection<PDBUserPrice> pdbUserPrices = new ArrayList<PDBUserPrice>();
        try{
            String sqlGet = "SELECT v.cena, v.valid_from, v.valid_to, u.jmeno FROM vozidla v JOIN uzivatele u ON v.user_id = u.id WHERE v.id = ?";
            OraclePreparedStatement oraclePreparedStatement = (OraclePreparedStatement) connection.prepareStatement(sqlGet);
            oraclePreparedStatement.setLong(1, id);
            OracleResultSet rs = (OracleResultSet) oraclePreparedStatement.executeQuery();
            while(rs.next()){
                UserPriceRowMapper userPriceRowMapper = new UserPriceRowMapper();
                PDBUserPrice pdbUserPrice = userPriceRowMapper.mapRow(rs, 0);
                pdbUserPrices.add(pdbUserPrice);
            }
            rs.close();
            oraclePreparedStatement.close();
        }catch (SQLException e){
            System.out.println(e);
        }
        connection.close();
        return pdbUserPrices;

    }


}
