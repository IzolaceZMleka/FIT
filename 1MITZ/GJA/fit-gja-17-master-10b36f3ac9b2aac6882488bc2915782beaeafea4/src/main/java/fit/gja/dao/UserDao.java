package fit.gja.dao;

import fit.gja.model.User;
import java.util.List;
import javax.sql.DataSource;
import org.springframework.jdbc.core.JdbcTemplate;

public class UserDao {

    private JdbcTemplate template;

    public void setDataSource(DataSource dataSource) {
        this.template = new JdbcTemplate(dataSource);
    }
    
    public List<User> list() {
        return template.query("SELECT id, name, surname, username, password, email, telephone FROM user", new UserMapper());
    }
    
    public User getUser(int id) {
        return template.queryForObject("SELECT id, name, surname, username, password, email, telephone FROM user WHERE id = ?", new Object[]{id}, new UserMapper());
    }
    
    public void createUser(User user) {
        template.update("INSERT INTO user (name, surname, username, password, email, telephone)VALUES(?, ?, ?, ?, ?, ?)",
                user.getName(), user.getSurname(), user.getUsername(), user.getPassword(), user.getEmail(), user.getTelephone());
    }

    public void updateUser(User user) {
        template.update("UPDATE user SET name = ?, surname = ?, username = ?, password = ?, email = ?, telephone = ? WHERE id = ?",
                user.getName(), user.getSurname(), user.getUsername(), user.getPassword(), user.getEmail(), user.getTelephone(), user.getId());
    }

    public void deleteUser(int id) {
        template.update("DELETE FROM user WHERE id = ?", id);
    }

}
