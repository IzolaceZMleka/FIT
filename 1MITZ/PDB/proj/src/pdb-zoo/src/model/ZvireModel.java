package model;

import java.io.IOException;
import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.Format;
import java.text.SimpleDateFormat;
import java.time.LocalDate;
import java.util.ArrayList;
import java.util.List;

import model.entity.HistorieUmisteni;
import model.entity.ZvireEntity;
import oracle.jdbc.OraclePreparedStatement;
import oracle.jdbc.OracleResultSet;
import oracle.ord.im.OrdImage;
/**
 * Class ZvireModel
 * Data model class for Zvire table.
 *
 * @author Ondřej Zemánek
 */
public class ZvireModel extends BaseModel {

    private Format formatter = new SimpleDateFormat("yyyy/MM/dd");

    public ZvireModel(Connection dbConnection) {
        super(dbConnection);
    }

    /**
     * Method for loading animal from Db.
     *
     * @author Ondřej Zemánek
     * @param id Input is the animal id in DB.
     * @return Output ZvireEntity object with animal info
     */
    public ZvireEntity GetZvireById(int id) {
        ZvireEntity zvire = null;
        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(String.format("SELECT * FROM zvire WHERE id = %d", id));

            rs.next();

            zvire = new ZvireEntity(rs.getInt("id"),
                    rs.getInt("idDruhu"),
                    rs.getString("jmeno"),
                    rs.getString("pohlavi"),
                    rs.getDate("datumNarozeni"),
                    rs.getDate("datumUmrti"),
                    rs.getDate("datumPorizeni"),
                    (OrdImage)rs.getORAData("fotka", OrdImage.getORADataFactory()));

            rs.close();
            stmt.close();

        } catch (SQLException ex) {

        }

        return zvire;
    }

    /**
     * Method to get all items.
     *
     * @author Ondřej Zemánek
     * @return Output is list of all animals
     */
    public List<ZvireEntity> GetAllZvire() {
        List<ZvireEntity> list = new ArrayList<ZvireEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery("SELECT * FROM Zvire");

            while(rs.next()) {
                list.add(new ZvireEntity(rs.getInt("id"),
                        rs.getInt("idDruhu"),
                        rs.getString("jmeno"),
                        rs.getString("pohlavi"),
                        rs.getDate("datumNarozeni"),
                        rs.getDate("datumUmrti"),
                        rs.getDate("datumPorizeni"),
                        (OrdImage)rs.getORAData("fotka", OrdImage.getORADataFactory())));
            }

            rs.close();
            stmt.close();
        }
        catch (SQLException ex) {

        }

        return list;
    }

    /**
     * Method to get all animals current living in specified Enclosure.
     *
     * @author Daniel Vosáhlo
     * @param id Input is Enclosure id
     * @return Output is list of all animals
     */
    public List<ZvireEntity> GetAllZvireInEnclosure(int id) {
        List<ZvireEntity> list = new ArrayList<ZvireEntity>();

        try {
            Statement stmt = dbConnection.createStatement();
            String query = "SELECT zvire.id, zvire.idDruhu, zvire.jmeno, zvire.pohlavi, zvire.datumNarozeni, zvire.datumUmrti, zvire.datumPorizeni, zvire.fotka " +
                    "FROM umisteni INNER JOIN zvire ON umisteni.idZvirete = zvire.id " +
                    "WHERE umisteni.idVybehu = "+ id +" AND umisteni.umisteniDo IS NULL";
            
            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(query);

            while(rs.next()) {
                list.add(new ZvireEntity(rs.getInt("id"),
                        rs.getInt("idDruhu"),
                        rs.getString("jmeno"),
                        rs.getString("pohlavi"),
                        rs.getDate("datumNarozeni"),
                        rs.getDate("datumUmrti"),
                        rs.getDate("datumPorizeni"),
                        (OrdImage)rs.getORAData("fotka", OrdImage.getORADataFactory())));
            }

            rs.close();
            stmt.close();
        }
        catch (SQLException ex) {
            System.out.println(ex);
        }

        return list;
    }

    /**
     * Method for creating an animal
     *
     * @author daniel Vosáhlo
     * @param zEn Input is zvireEntity without id
     * @return Method is returning the id of the created animal
     */
    public int CreateNewAnimal(ZvireEntity zEn, int idvybehu){
        int nextId = -1;

        String query = "INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) " +
                "VALUES(?, ?, ?, ?, TO_DATE(?, 'yyyy/mm/dd'), NULL,  NULL, ORDSYS.ORDIMAGE.init())";

        try{
            OraclePreparedStatement pstm = (OraclePreparedStatement) dbConnection.prepareStatement(query);

            nextId = this.generateNextId("zvire", "id");

            pstm.setInt(1, nextId);
            pstm.setString(2, zEn.jmeno);
            pstm.setInt(3,zEn.idDruh);
            pstm.setString(4,zEn.pohlavi);
            pstm.setString(5,formatter.format(zEn.datumNarozeni));

            pstm.executeQuery();
            pstm.close();

            //We must put him into karantena first for 1 day
            int nextUmId = this.generateNextId("umisteni", "id");
            String from = formatter.format(zEn.datumNarozeni);
            String to = LocalDate.parse(from).plusDays(1).toString();

            String queryUm = "BEGIN updateUmisteni(" + nextId + ", TO_TIMESTAMP('" + from + "', 'yyyy/mm/dd hh24:mi:ss') , TO_TIMESTAMP('" + to + "', 'yyyy/mm/dd hh24:mi:ss')," + 5 +" ," + nextUmId +"); END;";

            Statement stmt = dbConnection.createStatement();

            stmt.executeQuery(queryUm);
            stmt.close();

            //Then we can move the animal to its vybeh
            nextUmId = this.generateNextId("umisteni", "id");
            from = to;
            to = "9999/01/01";

            queryUm = "BEGIN updateUmisteni(" + nextId + ", TO_TIMESTAMP('" + from + "', 'yyyy/mm/dd hh24:mi:ss') , TO_TIMESTAMP('" + to + "', 'yyyy/mm/dd hh24:mi:ss')," + idvybehu +" ," + nextUmId +"); END;";

            Statement stmt2 = dbConnection.createStatement();
            stmt2.executeQuery(queryUm);
            stmt2.close();
        }
        catch (SQLException ex) {
            System.out.println(ex);
        }

        return nextId;
    }

    /**
     * Method for updating Animal info
     *
     * @author Daniel Vosáhlo
     * @param zEn Input is a zvireEntity
     */
    public void UpdateAnimalInfo(ZvireEntity zEn){
        String query = "UPDATE zvire SET jmeno = ?, idDruhu = ?, pohlavi = ?, datumNarozeni = TO_DATE(?, 'yyyy/mm/dd') WHERE id = ?";
        try {
            OraclePreparedStatement pstm = (OraclePreparedStatement) dbConnection.prepareStatement(query);

            pstm.setString(1,zEn.jmeno);
            pstm.setInt(2,zEn.idDruh);
            pstm.setString(3,zEn.pohlavi);
            pstm.setString(4,formatter.format(zEn.datumNarozeni));
            pstm.setInt(5,zEn.id);
            pstm.executeUpdate();
            pstm.close();

            if (zEn.datumUmrti != null){
                this.AnimalDied(zEn.id,  formatter.format(zEn.datumUmrti));
            }
        }
        catch (SQLException ex){
            System.out.println(ex);
        }
    }


    /**
     * Method for loading OrdImage from Db and changing it to a new Image.
     *
     * @author Daniel Vosáhlo
     * @param id Input is the animal id in DB.
     * @param photo Input is the path to Img file.
     */
    public void UploadPhoto(int id, String photo){
        OrdImage imgProxy = null;

        try {
            dbConnection.setAutoCommit(false);
            //Loading OrdImage obj from DB
            Statement stmt = dbConnection.createStatement();

            String selQuery = "SELECT fotka, datumPorizeni FROM zvire where id =" + id + " FOR UPDATE";

            OracleResultSet rs = (OracleResultSet) stmt.executeQuery(selQuery);

            rs.next();
            imgProxy = (OrdImage) rs.getORAData("fotka", OrdImage.getORADataFactory());

            rs.close();
            stmt.close();

            //Loading new image data to Java obj
            try {
                imgProxy.loadDataFromFile(photo);
                imgProxy.setProperties();

            }
            catch (IOException ex) {
                System.out.println(ex);

            }
            //Updating the Img in DB
            this.UpdatePhoto(id, imgProxy);
            dbConnection.setAutoCommit(true);
        }
        catch (SQLException ex) {
            System.out.println(ex);

        }

    }

    /**
     * Method for updating animal image and its metadata
     *
     * @author Daniel Vosáhlo
     * @param id Input is the animal id in DB.
     * @param photo Input is ORGImage.
     */
    private void UpdatePhoto(int id, OrdImage photo){
        try {
            //Updating the photo in DB
            String updateQuery = "UPDATE zvire SET fotka = ?, datumPorizeni = SYSDATE WHERE id =?";

            OraclePreparedStatement pstm = (OraclePreparedStatement) dbConnection.prepareStatement(updateQuery);

            pstm.setORAData(1, photo);
            pstm.setInt(2, id);

            pstm.executeUpdate();
            pstm.close();

            //Updating StillImage data
            String updateStillData = "UPDATE zvire z SET z.fotkaSi = SI_StillImage(z.fotka.getContent()) WHERE id = ?";

            pstm = (OraclePreparedStatement) dbConnection.prepareStatement(updateStillData);
            pstm.setInt(1, id);
            pstm.executeUpdate();

            String updateStillData2 = "update zvire set "
                    + "fotkaAc = SI_AverageColor(fotkaSi), "
                    + "fotkaCh = SI_ColorHistogram(fotkaSi), "
                    + "fotkaPc = SI_PositionalColor(fotkaSi), "
                    + "fotkaTx = SI_Texture(fotkaSi) "
                    + "where id = ? ";

            pstm = (OraclePreparedStatement) dbConnection.prepareStatement(updateStillData2);
            pstm.setInt(1, id);
            pstm.executeUpdate();
            pstm.close();

            dbConnection.commit();
        }
        catch (SQLException ex) {
            System.out.println(ex);
        }
    }

    /**
     * Method for removing the animal picture.
     * It sets all the img metadata to null and sets fotka to ORDSYS.ORDIMAGE.init().
     *
     * @author Daniel Vosáhlo
     * @param id Input is the animal id in DB.
     */
    public void RemovePhoto(int id){
        try {
            Statement stmt = dbConnection.createStatement();

            String updateQuery = "UPDATE zvire SET fotka = ORDSYS.ORDIMAGE.init(), datumPorizeni = NULL, fotkaSi = NULL, fotkaAc = NULL, fotkaCh = NULL, fotkaPc = NULL, fotkaTx = NULL WHERE id =" + id;

            stmt.executeQuery(updateQuery);
            stmt.close();

            dbConnection.commit();
        }
        catch(SQLException ex){

        }
    }

    /**
     * Method for loading OrdImage from Db, scaling it and update it.
     *
     * @author Daniel Vosáhlo
     * @param id Input is the animal id in DB.
     */
    public void ScalePhoto(int id, int scaleX, int scaleY){
        OrdImage imgProxy = null;

        try {
            dbConnection.setAutoCommit(false);
            //Loading OrdImage obj from DB
            Statement stmt = dbConnection.createStatement();

            String selQuery = "SELECT fotka FROM zvire where id =" + id + " FOR UPDATE";

            OracleResultSet rs = (OracleResultSet) stmt.executeQuery(selQuery);

            //Get next result
            rs.next();
            imgProxy = (OrdImage) rs.getORAData("fotka", OrdImage.getORADataFactory());

            //Get Image format
            String format = imgProxy.getFormat();

            rs.close();
            stmt.close();

            //Scale the image
            imgProxy.processCopy ("maxscale = " + scaleX +" "+ scaleY + " fileformat = " + format, imgProxy);

            //Update the image
            this.UpdatePhoto(id, imgProxy);
            dbConnection.setAutoCommit(true);
        }
        catch (SQLException ex) {
            System.out.println(ex);
        }
    }

    /**
     * Method for finding the most similar image.
     *
     * @author Daniel Vosáhlo
     * @param id Input is the animal id in DB.
     * @return Output is the animal id with the most similar image.
     */
    public int GetTheMostSimilar(int id){
        //Default values
        double weightAC = 0.3;
        double weightCH = 0.3;
        double weightPC = 0.1;
        double weightTX = 0.3;

        int destId = -1;

        try {
            String query = "SELECT src.id AS source , dst.id AS destination , " +
                    "(SI_ScoreByFtrList (new SI_FeatureList (src.fotkaAc , ? , src.fotkaCh , ? ," +
                    "src.fotkaPc , ? , src.fotkaTx , ? ) ," +
                    "dst.fotkaSi)) as similarity " +
                    "FROM zvire src , zvire dst " +
                    "WHERE src.id <> dst.id AND src.id = ? " +
                    "ORDER BY similarity ASC ";

            OraclePreparedStatement pstm = (OraclePreparedStatement) dbConnection.prepareStatement(query);

            pstm.setDouble(1, weightAC);
            pstm.setDouble(2, weightCH);
            pstm.setDouble(3, weightPC);
            pstm.setDouble(4, weightTX);
            pstm.setInt(5, id);

            OracleResultSet rs = (OracleResultSet) pstm.executeQuery();

            //Get next result
            rs.next();

            //Get the best dest ID by similarity
            destId = rs.getInt("destination");

            rs.close();
            pstm.close();

        }
        catch (SQLException ex){

        }

        return destId;
    }

    /**
     * Method for updating animals DATUMUMRTI and UMISTENIDO columns after they died.
     *
     * @author Daniel Vosáhlo
     * @param id Id of the dead animal.
     * @param date In yyyy/mm/dd hh24:mi:ss format.
     */
    public void AnimalDied(int id, String date){

        String timeStamp = date;
        //Split by space to get valid Date format yyyy/mm/dd
        String[] splitStr = date.split("\\s+");
        date = splitStr[0];

        try {
            //We must update DatumUmrti
            String updateQuery = "UPDATE zvire SET datumUmrti = TO_DATE(?, 'yyyy/mm/dd') WHERE id = ?";
            OraclePreparedStatement ps = (OraclePreparedStatement) dbConnection.prepareStatement(updateQuery);

            ps.setString(1, date);
            ps.setInt(2, id);

            ps.executeQuery();
            ps.close();

            //We must update umisteni (End the current)
            String updateUmisteniDoQuery = "UPDATE umisteni up SET up.UMISTENIDO = TO_TIMESTAMP(?, 'yyyy/mm/dd hh24:mi:ss') WHERE up.ID = (SELECT id FROM umisteni us WHERE (us.IDZVIRETE = ? AND us.UMISTENIDO IS NULL))";
            OraclePreparedStatement pstm = (OraclePreparedStatement) dbConnection.prepareStatement(updateUmisteniDoQuery);

            pstm.setString(1, timeStamp);
            pstm.setInt(2, id);

            pstm.executeQuery();
            pstm.close();

            dbConnection.commit();
        }
        catch (SQLException ex){

        }
    }

    /**
     * Method for counting all the animals which were in the specified Enclosure in the specific year.
     *
     * @author Daniel Vosáhlo
     * @param year In format YYYY
     * @param id  Enclosure id
     * @return Animal count in param year
     */
    public int AnimalCountInEnclosureInYear(String year, int id){
        int count = 0;

        try {
            Statement stmt = dbConnection.createStatement();

            String query = "SELECT COUNT(DISTINCT idZvirete) AS animalCount FROM umisteni WHERE " +
                    "(" +
                        " idvybehu = " + id +" AND" +
                        " (" +
                            " (umisteniOd BETWEEN TO_DATE('" + year +"/01/01', 'yyyy/mm/dd') AND TO_DATE('" + year +"/12/31', 'yyyy/mm/dd')) OR" + //From 1.1 - 31.12 OR
                            " (umisteniDo BETWEEN TO_DATE('" + year +"/01/01', 'yyyy/mm/dd') AND TO_DATE('" + year +"/12/31', 'yyyy/mm/dd')) OR" + //To 1.1 - 31.12 OR
                            " ((umisteniDo > TO_DATE('" + year +"/12/31', 'yyyy/mm/dd') OR umisteniDo IS NULL) AND umisteniOd < TO_DATE('" + year +"/01/01', 'yyyy/mm/dd'))"+ //(To > 31.12 OR animal is still in) AND From <1.1
                        " )" +
                    " )";

            OracleResultSet rs = (OracleResultSet)stmt.executeQuery(query);

            rs.next();
            count = rs.getInt("animalCount");

            rs.close();
            stmt.close();
        }
        catch(SQLException ex){

        }

        return count;
    }

    /**
     * Method for selecting all the Enclosures where the animal was kept.
     *
     * @author Daniel Vosáhlo
     * @param id Animal Id
     * @return List of HistorieUmisteni
     */
    public List<HistorieUmisteni> GetAnimalEnclosureHistory(int id) {
        List<HistorieUmisteni> list = new ArrayList<>();

            try{
                String query = "SELECT vybeh.id, vybeh.jmeno AS jmenoV, zvire.jmeno AS jmenoZ, umisteni.umisteniOd, umisteni.umisteniDo " +
                        "FROM zvire INNER JOIN umisteni ON zvire.id = umisteni.idZvirete INNER JOIN vybeh ON umisteni.idVybehu = vybeh.id " +
                        "WHERE zvire.id = ? " +
                        "ORDER BY umisteni.umisteniOd";

                OraclePreparedStatement ps = (OraclePreparedStatement) dbConnection.prepareStatement(query);
                ps.setInt(1, id);

                OracleResultSet rs = (OracleResultSet)ps.executeQuery();

                while(rs.next()) {
                    list.add(new HistorieUmisteni(
                            rs.getInt("id"),
                            rs.getString("jmenoV"),
                            rs.getString("jmenoZ"),
                            rs.getTimestamp("umisteniOd"),
                            rs.getTimestamp("umisteniDo")
                           ));
                }

                rs.close();
                ps.close();
            }
            catch(SQLException ex){

            }

        return list;
    }

    /**
     * Method for selecting all the animals which were kept in the Enclosures.
     *
     * @author Daniel Vosáhlo
     * @param id Enclosures Id
     * @return List of animal Ids
     */
    public List<Integer> GetEnclosureAnimalHistory(int id) {
        List<Integer> list = new ArrayList<>();

        try{
            Statement stmt = dbConnection.createStatement();

            String query = "SELECT DISTINCT umisteni.idZvirete FROM umisteni WHERE umisteni.idVybehu = " +id;

            OracleResultSet rs = (OracleResultSet) stmt.executeQuery(query);

            while(rs.next()) {
                list.add(rs.getInt(1));
            }

            rs.close();
            stmt.close();
        }
        catch(SQLException ex){

        }

        return list;
    }

    /**
     * Method for counting all the males/females in the specified Enclosure in specified year.
     *
     * @author Daniel Vosáhlo
     * @param id Enclosure id
     * @param year In YYYY format
     * @param gender inputGender True for males (samec) False for female (samice)
     * @return Number of animals
     */
    public int GetAnimalCountByGenderAndYearInEnclosure(int id, String year, String gender){
        int count = 0;
        try {
            String query = "SELECT COUNT(DISTINCT idZvirete) AS animalCount FROM " +
                        "(SELECT * FROM " +
                        "zvire INNER JOIN umisteni ON zvire.id = umisteni.idZvirete INNER JOIN vybeh ON umisteni.idVybehu = vybeh.id " +
                        "WHERE " +
                            "(" +
                                "idvybehu = " + id + " AND " +
                                " (" +
                                    " (umisteniOd BETWEEN TO_DATE('" + year +"/01/01', 'yyyy/mm/dd') AND TO_DATE('" + year +"/12/31', 'yyyy/mm/dd')) OR" + //From 1.1 - 31.12 OR
                                    " (umisteniDo BETWEEN TO_DATE('" + year +"/01/01', 'yyyy/mm/dd') AND TO_DATE('" + year +"/12/31', 'yyyy/mm/dd')) OR" + //To 1.1 - 31.12 OR
                                    " (" +
                                        "(umisteniDo > TO_DATE('" + year +"/12/31', 'yyyy/mm/dd') OR " +
                                        "(umisteniDo IS NULL AND " + year +" <= extract(year from sysdate))" +
                                    ") AND " +
                                    "umisteniOd < TO_DATE('" + year +"/01/01', 'yyyy/mm/dd'))"+ //(To > 31.12 OR animal is still in) AND From <1.1
                                " )" +
                            ")) " +
                    "WHERE pohlavi LIKE '"+ gender +"'";

            Statement stmt = dbConnection.createStatement();

            OracleResultSet rs = (OracleResultSet) stmt.executeQuery(query);

            rs.next();

            count = rs.getInt("animalCount");

            rs.close();
            stmt.close();
        }
        catch (SQLException ex){

        }

        return count;
    }

    /**
     * Method for deleting intervals in umisteni by period and idZvirete
     *
     * @author Daniel Vosáhlo
     * @param id Animal id
     * @param from Start of interval ideal in yyyy/mm/dd hh24:mi:ss format
     * @param to End of interval ideal in yyyy/mm/dd hh24:mi:ss format
     */
    public void DeleteTimeInterval(int id, String from, String to){
        int newId = this.generateNextId("umisteni", "id");

        try {
            String query = "BEGIN deleteUmisteni(" + id + ", TO_TIMESTAMP('" + from + "', 'yyyy/mm/dd hh24:mi:ss') , TO_TIMESTAMP('" + to + "', 'yyyy/mm/dd hh24:mi:ss'), " + newId +"); END;";

            Statement stmt = dbConnection.createStatement();

            stmt.executeQuery(query);
        }
        catch (SQLException ex){

        }
    }

    /**
     * Method for update idVybehu in spedified interval for specified idZvirete
     *
     * @author Daniel Vosáhlo
     * @param id Animal id
     * @param from In format yyyy/mm/dd hh24:mi:ss
     * @param to In format yyyy/mm/dd hh24:mi:ss OR null
     * @param newUmisteni New Enclosure (idvybehu)
     */
    public void UpdateUmisteniInInterval(int id, String from, String to, int newUmisteni){
        int newId = this.generateNextId("umisteni", "id");

        try {

            if (to == null){
                to = "9999/01/01";
            }
            String query = "BEGIN updateUmisteni(" + id + ", TO_TIMESTAMP('" + from + "', 'yyyy/mm/dd hh24:mi:ss') , TO_TIMESTAMP('" + to + "', 'yyyy/mm/dd hh24:mi:ss')," + newUmisteni +" ," + newId +"); END;";
            
            Statement stmt = dbConnection.createStatement();

            stmt.executeQuery(query);
        }
        catch (SQLException ex){

        }
    }
}
