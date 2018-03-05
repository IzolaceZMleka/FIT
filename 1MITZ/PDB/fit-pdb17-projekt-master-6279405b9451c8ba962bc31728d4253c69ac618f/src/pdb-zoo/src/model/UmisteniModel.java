package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import model.entity.UmisteniEntity;
import oracle.jdbc.OracleResultSet;

/**
 * Class UmisteniModel
 * Data model class for Umisteni table.
 *
 * @author Ondřej Zemánek
 */
public class UmisteniModel extends BaseModel {

    public UmisteniModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading Umisteni from Db by id.
     *
     * @author Ondřej Zemánek
     * @param id Input is the Umisteni id in DB.
     * @return Output UmisteniEntity object with umisteni info
     */
    public UmisteniEntity GetUmisteniById(int id) {
        UmisteniEntity typ = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Umisteni WHERE id = %d", id));

            rs.next();

            typ = new UmisteniEntity(rs.getInt("id"),
                    rs.getInt("idVybehu"),
                    rs.getInt("idZvire"),
                    rs.getDate("umisteniOd"),
                    rs.getDate("umisteniDo"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return typ;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all rows from unisteni table.
     */
    public List<UmisteniEntity> GetAllUmisteni() {
        List<UmisteniEntity> list = new ArrayList<UmisteniEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM Umisteni");

            while(rs.next()) {
                list.add(new UmisteniEntity(rs.getInt("id"),
                        rs.getInt("idVybehu"),
                        rs.getInt("idZvirete"),
                        rs.getDate("umisteniOd"),
                        rs.getDate("umisteniDo")));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        	System.out.println(ex);
        }

        return list;
    }
}
