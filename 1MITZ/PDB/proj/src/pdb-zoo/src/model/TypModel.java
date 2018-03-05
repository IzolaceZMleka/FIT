package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import model.entity.TypEntity;
import oracle.jdbc.OracleResultSet;

/**
 * Class TypModel
 * Data model class for Typ table.
 *
 * @author Ondřej Zemánek
 */
public class TypModel extends BaseModel {

    public TypModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading Typ from Db by id.
     *
     * @author Ondřej Zemánek
     * @param id Input is the Typ id in DB.
     * @return Output TypEntity object with Typ info
     */
    public TypEntity GetTypById(int id) {
        TypEntity typ = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Typ WHERE id = %d", id));

            rs.next();

            typ = new TypEntity(Integer.parseInt(rs.getString("id")),
                    rs.getString("jmeno"),
                    rs.getString("barva"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return typ;
    }

    /**
     * Method for loading Typ from Db by name.
     *
     * @author Ondřej Zemánek
     * @param name Input is the Typ name in DB.
     * @return Output TypEntity object with typ info
     */
    public TypEntity GetTypEntityByName(String name) {
        TypEntity type = null;

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Typ WHERE jmeno = '%s'", name));

            rs.next();

            type = new TypEntity(Integer.parseInt(rs.getString("id")),
                    rs.getString("jmeno"),
                    rs.getString("barva"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return type;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all rows from typ table.
     */
    public List<TypEntity> GetAllTyp() {
        List<TypEntity> list = new ArrayList<TypEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM Typ");

            while(rs.next()) {
                list.add(new TypEntity(Integer.parseInt(rs.getString("id")),
                        rs.getString("jmeno"),
                        rs.getString("barva")));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return list;
    }
}
