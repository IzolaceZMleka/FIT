package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import model.entity.VybehEntity;
import oracle.jdbc.OracleResultSet;

/**
 * Class VybehModel
 * Data model class for Vybeh table.
 *
 * @author Ondřej Zemánek
 */
public class VybehModel extends BaseModel {

    public VybehModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading Vybeh from Db by id.
     *
     * @author Ondřej Zemánek
     * @param id Input is the Vybeh id in DB.
     * @return Output VybehEntity object with vybeh info
     */
    public VybehEntity GetVybehById(int id) {
        VybehEntity enclosure = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Vybeh WHERE id = %d", id));

            rs.next();

            enclosure = new VybehEntity(rs.getInt("id"),
                    rs.getString("jmeno"),
                    rs.getString("info"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return enclosure;
    }

    /**
     * Method for loading Vybeh from Db by name.
     *
     * @author Ondřej Zemánek
     * @param name Input is the Vybeh name in DB.
     * @return Output VybehEntity object with vybeh info
     */
    public VybehEntity GetVybehEntityByName(String name) {
        VybehEntity enclosure = null;

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Vybeh WHERE jmeno = '%s'", name));

            rs.next();

            enclosure = new VybehEntity(Integer.parseInt(rs.getString("id")),
                    rs.getString("jmeno"),
                    rs.getString("info"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return enclosure;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all rows from vybeh table.
     */
    public List<VybehEntity> GetAllVybeh() {
        List<VybehEntity> list = new ArrayList<VybehEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM Vybeh");

            while(rs.next()) {
                list.add(new VybehEntity(rs.getInt("id"),
                        rs.getString("jmeno"),
                        rs.getString("info")));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return list;
    }
}
