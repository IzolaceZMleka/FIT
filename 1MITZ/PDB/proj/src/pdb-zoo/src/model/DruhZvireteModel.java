package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import model.entity.DruhZvireteEntity;
import oracle.jdbc.OracleResultSet;

/**
 * Class DruhZvireteModel
 * Data model class for DruhZvirete table.
 *
 * @author Ondřej Zemánek
 */
public class DruhZvireteModel extends BaseModel {

    public DruhZvireteModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading Druh zvirete from Db by id.
     *
     * @author Ondřej Zemánek
     * @param id Input is the DruhZvirete id in DB.
     * @return Output DruhZvireteEntity object with Druh zvirete info
     */
    public DruhZvireteEntity DruhZvireteById(int id) {
        DruhZvireteEntity druhZvirete = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM DruhZvirete WHERE id = %d", id));

            rs.next();

            druhZvirete = new DruhZvireteEntity(rs.getInt("id"),
                    rs.getString("jmenoDruhu"),
                    rs.getString("celed"),
                    rs.getString("oblast"),
                    rs.getString("info"));

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return druhZvirete;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all rows from vybeh table.
     */
    public List<DruhZvireteEntity> GetAllDruhZvirete() {
        List<DruhZvireteEntity> list = new ArrayList<DruhZvireteEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM DruhZvirete");

            while(rs.next()) {
                list.add(new DruhZvireteEntity(rs.getInt("id"),
                        rs.getString("jmenoDruhu"),
                        rs.getString("celed"),
                        rs.getString("oblast"),
                        rs.getString("info")));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return list;
    }
}
