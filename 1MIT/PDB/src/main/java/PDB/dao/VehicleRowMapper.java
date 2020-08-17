package PDB.dao;

import PDB.entity.PDBvehicle;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;
import oracle.sql.DATE;
import org.springframework.jdbc.core.RowMapper;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.Base64;
import java.util.Objects;

public class VehicleRowMapper implements RowMapper<PDBvehicle> {

    @Override
    public PDBvehicle mapRow(ResultSet resultSet, int i) throws SQLException {
        PDBvehicle vehicle = new PDBvehicle();
        vehicle.setId(resultSet.getLong("id"));
        vehicle.setNakup(new DATE(resultSet.getDate("nakup")));
        vehicle.setProdej(new DATE(resultSet.getDate("prodej")));
        vehicle.setZnacka(resultSet.getString("znacka"));
        vehicle.setModel(resultSet.getString("model"));
        vehicle.setMotor(resultSet.getString("motor"));
        vehicle.setRok(resultSet.getInt("rok"));
        vehicle.setStk(new DATE(resultSet.getDate("stk")));
        vehicle.setKupniCena(resultSet.getLong("kupni_cena"));
        vehicle.setCena(resultSet.getLong("cena"));
        vehicle.setKilometry(resultSet.getLong("kilometry"));
        vehicle.setStav(resultSet.getString("stav"));
        vehicle.setKategorie(resultSet.getInt("kategorie"));
        if(Objects.nonNull(resultSet.getObject("umisteni"))){
            vehicle.setUmisteni(resultSet.getInt("umisteni"));
        }
        vehicle.setValidFrom(new DATE(resultSet.getDate("valid_from")));
        vehicle.setValidTo(new DATE(resultSet.getDate("valid_to")));
        vehicle.setUserId(resultSet.getLong("user_id"));
        if(Objects.nonNull(resultSet.getObject("obrazek"))){
            vehicle.setObrazek(resultSet.getLong("obrazek"));
        }
        try {
            OrdImage imgProxy = (OrdImage) ((OracleResultSet) resultSet).getORAData("photo", OrdImage.getORADataFactory());
            if(!Objects.isNull(imgProxy))
                vehicle.setObrazekData(Base64.getEncoder().encodeToString(imgProxy.getDataInByteArray()));
        } catch (IOException |SQLException n) {
        }
        return vehicle;
    }
}