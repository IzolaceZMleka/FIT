package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;

import oracle.jdbc.OracleResultSet;

/**
 * Class BaseModel
 * Abstract base model class for communication with DB.
 *
 * @author Ondřej Zemánek
 */
public abstract class BaseModel {

    protected Connection dbConnection;

    public BaseModel(Connection dbConnection) {
        this.dbConnection = dbConnection;
    }

    /**
     * Method for simulating autoincrement in oracle DB
     *
     * @author Daniel Vosáhlo
     * @param table
     * @param idName
     * @return
     */
    public int generateNextId(String table, String idName){
        int id = -1;

        String query = "SELECT MAX(" + idName + ") AS id FROM " + table;

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(query);
            rs.next();
            id = rs.getInt("id");
            rs.close();
            stmt.close();
        }
        catch (SQLException ex) {

        }

        id = id + 1;

        return id;
    }
}
