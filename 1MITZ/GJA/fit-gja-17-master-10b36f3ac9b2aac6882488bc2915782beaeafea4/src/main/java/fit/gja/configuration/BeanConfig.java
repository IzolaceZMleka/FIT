package fit.gja.configuration;

import fit.gja.dao.UserDao;
import javax.sql.DataSource;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.jdbc.datasource.DriverManagerDataSource;

@Configuration
public class BeanConfig {

    @Bean
    public DataSource getDataSource() {
        DriverManagerDataSource dataSource = new DriverManagerDataSource();
        dataSource.setDriverClassName("com.mysql.jdbc.Driver");
        dataSource.setUrl("jdbc:mysql://localhost:3306/springjdbc");
        dataSource.setUsername("root");
        dataSource.setPassword("heslo");
         
        return dataSource;
    }
     
    @Bean
    public UserDao getUserDao() {
        UserDao dao = new UserDao();
        dao.setDataSource(getDataSource());
        
        return dao;
    }
}
