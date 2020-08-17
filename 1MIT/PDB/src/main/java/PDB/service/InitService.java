package PDB.service;

import oracle.jdbc.pool.OracleDataSource;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;

import javax.sql.DataSource;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

public class InitService {

    @Autowired
    JdbcTemplate jdbcTemplate;

    @Autowired
    DataSource dataSource;

    public void init(){
        try {
            // create a OracleDataSource instance
            OracleDataSource ods = new OracleDataSource();
            ods.setURL("jdbc:oracle:thin:@//"+System.getProperty("hostname")+":"+System.getProperty("port")+"/gort.fit.vutbr.cz");
            ods.setUser(System.getProperty("login"));
            ods.setPassword(System.getProperty("password"));

            File file = new File("./sql/database.sql");
            FileInputStream fis = new FileInputStream(file);
            byte[] data = new byte[(int) file.length()];
            fis.read(data);
            fis.close();
            String str[] = new String(data).split("\n");
            String fin = "";
            for(String tmp: str){
                if(tmp.startsWith("--")) {
                    continue;
                }
                fin = fin + tmp;
            }
            str = fin.split(";");
            Connection connection = ods.getConnection();
            for(String tm: str){
                try{
                    System.out.println(tm);
                    Statement statement = connection.createStatement();
                    statement.executeUpdate(tm);
                    statement.close();
                } catch (SQLException e){
                    System.out.println(e);
                }
            }
            file = new File("./sql/procedures.sql");
            fis = new FileInputStream(file);
             data = new byte[(int) file.length()];
            fis.read(data);
            fis.close();
            str = new String(data).split("\n");
            fin = "";
            for(String tmp: str){
                fin = fin + tmp + " ";
            }
            try{
                System.out.println(fin);
                Statement statement = connection.createStatement();
                statement.executeUpdate(fin);
                statement.close();
            } catch (SQLException e){
                System.out.println(e);
            }
            connection.close();
        } catch (SQLException| IOException sqlEx) {
            System.err.println("SQLException: " + sqlEx.getMessage());
        }

    }
}
