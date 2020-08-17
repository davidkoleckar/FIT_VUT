package PDB;

import PDB.service.InitService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.context.ApplicationContext;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Primary;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.datasource.DriverManagerDataSource;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import sun.misc.IOUtils;
import sun.nio.ch.IOUtil;

import javax.sql.DataSource;
import java.io.File;
import java.io.FileInputStream;
import java.util.Arrays;
import java.util.Objects;

@SpringBootApplication
public class Application {

    public static void main(String[] args) {
        System.setProperty("login", "xnavra53");
        System.setProperty("password", "7tws6q2n");
        System.setProperty("hostname", "gort.fit.vutbr.cz");
        System.setProperty("port", "1521");

        for(String item: args){
            String tmp[] = item.split("=");
            switch(tmp[0]){
                case "-login":
                    System.setProperty("login", tmp[1]);
                    break;
                case "-password":
                    System.setProperty("password", tmp[1]);
                    break;
                case "-hostname":
                    System.setProperty("hostname", tmp[1]);
                    break;
                case "-port":
                    System.setProperty("port", tmp[1]);
                case "-init":
                    System.setProperty("init", "");
                    break;
                     default:
                        System.out.println("NEPLATNE ARGUMENTY");
                        return;
            }
        }
        SpringApplication.run(Application.class, args);
    }

    @Bean
    public CommandLineRunner commandLineRunner(ApplicationContext ctx) {
        return args -> {
            if(Objects.nonNull(System.getProperty("init"))){
                InitService initService = new InitService();
                initService.init();

            }
        };
    }

    @Bean
    @Primary
    public DataSource DataSource() {
//        TODO: user login/password from cmd
        DriverManagerDataSource dataSource = new DriverManagerDataSource();
        dataSource.setDriverClassName("oracle.jdbc.OracleDriver");
        dataSource.setUrl("jdbc:oracle:thin:@//"+System.getProperty("hostname")+":"+System.getProperty("port")+"/gort.fit.vutbr.cz");
        dataSource.setUsername(System.getProperty("login"));
        dataSource.setPassword(System.getProperty("password"));
        return dataSource;
    }

//    @Configuration
//    @EnableSwagger2
//    public class SwaggerConfig {
//        @Bean
//        public Docket api() {
//            return new Docket(DocumentationType.SWAGGER_2)
//                    .select()
//                    .apis(RequestHandlerSelectors.any())
//                    .paths(PathSelectors.any())
//                    .build();
//        }
//    }

    @Bean
    public BCryptPasswordEncoder bCryptPasswordEncoder() {
        return new BCryptPasswordEncoder();
    }



}