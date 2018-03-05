package model;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Struct;
import java.util.ArrayList;
import java.util.List;

import model.entity.ZooEntity;
import oracle.jdbc.OracleResultSet;
import oracle.spatial.geometry.JGeometry;


/**
 * Class ZooModel
 * Data model class for Zoo table.
 *
 * @author Ondřej Zemánek
 */
public class ZooModel extends BaseModel {

    public ZooModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading zoo region from Db.
     *
     * @author Ondřej Zemánek
     * @param id Input is the zoo region id in DB.
     * @return Output ZooEntity object with region info
     */
    public ZooEntity GetAreaById(int id) {
        ZooEntity zoo = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM Zoo WHERE id = %d", id));

            rs.next();

            Struct obj = (Struct) rs.getObject("prostorovaData");
            JGeometry area = JGeometry.loadJS(obj);

            zoo = new ZooEntity(rs.getInt("id"),
                    rs.getInt("idVybehu"),
                    rs.getInt("idTyp"),
                    area);

            rs.close();
            stmt.close();
        } catch (SQLException ex) {

        }

        return zoo;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all zoo regions
     */
    public List<ZooEntity> GetAllArea() {
        List<ZooEntity> list = new ArrayList<ZooEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM Zoo");

            while(rs.next()) {
                Struct obj = (Struct) rs.getObject("prostorovaData");
                JGeometry area = JGeometry.loadJS(obj);

                list.add(new ZooEntity(rs.getInt("id"),
                        rs.getInt("idVybehu"),
                        rs.getInt("idTyp"),
                        area));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return list;
    }

    /**
     * Method for update the zoo object.
     * Update Zoo object to DB.
     *
     * @author Ondřej Zemánek
     * @param id zoo id.
     * @return updated zoo object id if success, otherwise -1.
     */
    public int UpdateZoo(int id, Integer idVybehu, Integer idTyp, JGeometry area) {
        int result = -1;

        try {
            PreparedStatement pstmt = dbConnection.prepareStatement("UPDATE Zoo SET idVybehu = ?, idTyp = ? , prostorovaData = ? WHERE id = ?");
            //pstmt.setInt(1, idVybehu);
            pstmt.setObject(1, idVybehu, java.sql.Types.INTEGER);
            pstmt.setInt(2, idTyp);
            pstmt.setObject(3, JGeometry.storeJS(area, dbConnection));
            pstmt.setInt(4, id);

            result = pstmt.executeUpdate();

            pstmt.close();

            dbConnection.commit();
        } catch (SQLException ex) {
        	System.out.println(ex);
        }

        return result;
    }

    /**
     * Method for inserting the zoo object.
     * Insert Zoo object to DB.
     *
     * @author Ondřej Zemánek
     * @param id zoo id.
     * @return new zoo object id if success, otherwise -1.
     */
    public int InsertZoo(int id, Integer idVybehu, Integer idTyp, JGeometry area) {
        int result = -1;

        try {
            PreparedStatement pstmt = dbConnection.prepareStatement("INSERT INTO Zoo (id, idVybehu, idTyp, prostorovaData) VALUES (?, ?, ?, ?)");
            pstmt.setInt(1, id);
            pstmt.setInt(2, idVybehu != null ? idVybehu : java.sql.Types.NULL);
            pstmt.setInt(3, idTyp != null ? idTyp : java.sql.Types.NULL);

            Struct obj = JGeometry.storeJS(area, dbConnection);
            pstmt.setObject(4, obj);

            result = pstmt.executeUpdate();

            pstmt.close();

            dbConnection.commit();
        } catch (SQLException ex) {
        }

        return result;
    }

    /**
     * Method for removing the zoo object.
     * Delete Zoo object from DB.
     *
     * @author Ondřej Zemánek
     * @param id Input is the zoo id in DB.
     * @return true if success, otherwise false.
     */
    public boolean RemoveZoo(int id) {
        boolean result = false;

        try {
            PreparedStatement pstmt = dbConnection.prepareStatement("DELETE FROM Zoo WHERE Id = ?");
            pstmt.setInt(1, id);
            result = pstmt.execute();

            pstmt.close();

            dbConnection.commit();
        }
        catch(SQLException ex){

        }

        return result;
    }

    /**
     * Method Finds neighboring Zoo objects.
     * Finds all Zoo objects closer then 'distance' to Zoo object with id 'zooId'.
     *
     * @author Ondřej Zemánek
     * @param zooId center zoo id
     * @param distance is maximum distance from center Zoo object.
     * @return List of neighboring Zoo objects.
     */
    public List<ZooEntity> GetAllZooCloserThen(int zooId, int distance) {
        List<ZooEntity> list = new ArrayList<ZooEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT g2.* " +
                            "FROM zoo g1, zoo g2 " +
                            "WHERE " +
                            "g1.Id != g2.Id AND " +
                            "g1.Id = %d AND " +
                            "SDO_WITHIN_DISTANCE(g1.prostorovadata, g2.prostorovadata, 'distance=%d') = 'TRUE'",
                    zooId,
                    distance));

            while(rs.next()) {
                Struct obj = (Struct)rs.getObject("prostorovaData");
                JGeometry prostorovaData = JGeometry.loadJS(obj);

                list.add(new ZooEntity(rs.getInt("id"),
                        rs.getInt("idVybehu"),
                        rs.getInt("idTyp"),
                        prostorovaData));
            }

            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return list;
    }

    /**
     * Method check if two Zoo objects is in intersection.
     * Check if Zoo object with id 'zooFirstId' is in intersection with Zoo object with id 'zooSecondId'.
     *
     * @author Ondřej Zemánek
     * @param zooFirstId first Zoo object id.
     * @param zooSecondId second Zoo object id.
     * @return true if objects is in intersection.
     */
    public boolean Intersection(int zooFirstId, int zooSecondId) {
        boolean intersection = false;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT 1 AS result FROM zoo g1, zoo g2 " +
                            "WHERE g1.Id = %d AND " +
                            "g2.Id = %d AND " +
                            "SDO_FILTER(g1.prostorovadata, g2.prostorovadata) = 'TRUE'",
                    zooFirstId,
                    zooSecondId));

            rs.next();
            intersection = rs.getBoolean("result");
            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return intersection;
    }

    /**
     * Method check if first Zoo object contains second Zoo object.
     * Check if Zoo object with id 'zooFirstId' contains Zoo object with id 'zooSecondId'.
     *
     * @author Ondřej Zemánek
     * @param zooFirstId first Zoo object id.
     * @param zooSecondId second Zoo object id.
     * @return true if first zoo Object contains second one, otherwise false.
     */
    public boolean ZooObjectContainsZooObject(int zooFirstId, int zooSecondId) {
        boolean anyInteract = false;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT 1 AS result FROM zoo g1, zoo g2\n" +
                            "WHERE g1.Id = %d AND\n" +
                            "g2.Id = %d AND\n" +
                            "SDO_RELATE(g1.prostorovadata, g2.prostorovadata, 'mask=CONTAINS') = 'TRUE'",
                    zooFirstId,
                    zooSecondId));

            rs.next();
            anyInteract = rs.getBoolean("result");
            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return anyInteract;
    }

    /**
     * Method calculates Zoo Object area.
     *
     * @author Ondřej Zemánek
     * @param zooId Zoo object id.
     * @return Zoo object area.
     */
    public double GetZooArea(int zooId) {
        double area = -1.0;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT SDO_GEOM.SDO_AREA(prostorovadata) AS area FROM zoo WHERE Id = %d", zooId));

            rs.next();
            area = rs.getDouble("area");
            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        	System.out.println(ex);
        }

        return area;
    }

    /**
     * Method calculates Zoo Object edges length.
     *
     * @author Ondřej Zemánek
     * @param zooId Zoo object id.
     * @return Zoo object edges length.
     */
    public double GetZooLength(int zooId) {
        double length = -1.0;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT SDO_GEOM.SDO_LENGTH(prostorovadata) AS length FROM zoo WHERE Id = %d", zooId));

            rs.next();
            length = rs.getDouble("length");
            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return length;
    }

    /**
     * Method calculates distance between two Zoo objects.
     *
     * @author Ondřej Zemánek
     * @param zooFirstId first Zoo object id.
     * @param zooSecondId second Zoo object id.
     * @return distance between Zoo objects.
     */
    public double GetZooObjectsDistance(int zooFirstId, int zooSecondId) {
        double distance = -1.0;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT SDO_GEOM.SDO_DISTANCE(g1.prostorovadata, g2.prostorovadata) AS distance " +
                    "FROM zoo g1, zoo g2 " +
                    "WHERE g1.Id = %d AND g2.Id = %d", zooFirstId, zooSecondId));

            rs.next();
            distance = rs.getDouble("distance");
            rs.close();
            stmt.close();
        } catch (SQLException ex) {
        }

        return distance;
    }
}
