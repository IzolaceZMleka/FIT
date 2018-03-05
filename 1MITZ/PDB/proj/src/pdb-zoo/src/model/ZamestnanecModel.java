package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import model.entity.ZamestnanecEntity;
import oracle.jdbc.OracleResultSet;

/**
 * Class ZamestnanecModel
 * Data model class for Zamestnanec table.
 *
 * @author Ondřej Zemánek
 */
public class ZamestnanecModel extends BaseModel {

    public ZamestnanecModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading zamestnanec from Db by id.
     *
     * @author Ondřej Zemánek
     * @param id Input is the zamestnanec id in DB.
     * @return Output ZamestnanecEntity object with zamestnanec info
     */
    public ZamestnanecEntity GetZamestnanecById(int id) {
        ZamestnanecEntity zamestnanec = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Zamestnanec WHERE id = %d", id));

            rs.next();

            zamestnanec = new ZamestnanecEntity(Integer.parseInt(rs.getString("id")),
                    rs.getString("jmeno"),
                    rs.getString("prijmeni"),
                    rs.getString("login"),
                    rs.getString("heslo"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return zamestnanec;
    }

    /**
     * Method for loading zamestnanec from Db by login.
     *
     * @author Ondřej Zemánek
     * @param login Input is the zamestnanec login name in DB.
     * @return Output ZamestnanecEntity object with zamestnanec info
     */
    public ZamestnanecEntity GetZamestnanecByLogin(String login) {
        ZamestnanecEntity zamestnanec = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Zamestnanec WHERE login = '%s'", login));

            rs.next();

            zamestnanec = new ZamestnanecEntity(Integer.parseInt(rs.getString("id")),
                    rs.getString("jmeno"),
                    rs.getString("prijmeni"),
                    rs.getString("login"),
                    rs.getString("heslo"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return zamestnanec;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all rows from zamestnanec table.
     */
    public List<ZamestnanecEntity> GetAllZamestnanec() {
        List<ZamestnanecEntity> list = new ArrayList<ZamestnanecEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM Zamestnanec");

            while(rs.next()) {
                list.add(new ZamestnanecEntity(rs.getInt("id"),
                        rs.getString("jmeno"),
                        rs.getString("prijmeni"),
                        rs.getString("login"),
                        rs.getString("heslo")));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return list;
    }
}
