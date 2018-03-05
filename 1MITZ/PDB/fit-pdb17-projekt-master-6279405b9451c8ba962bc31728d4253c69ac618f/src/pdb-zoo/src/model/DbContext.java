package model;

import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;

import oracle.jdbc.pool.OracleDataSource;

/**
 * Class DbContext
 * Class representing DB context. Establish/Close DB connection.
 *
 * @author Ondřej Zemánek
 */
public class DbContext {

    private OracleDataSource oracleDataSource;
    private Connection dbConnection;

    private String connectionString;

    public DbContext(String connectionString)
    {
        this.connectionString = connectionString;
    }

    /**
     * Method for opening db connection.
     *
     * @author Ondřej Zemánek
     * @param userName User credentials to access the database.
     * @param userPassword User credentials to access the database.
     * @return returns db connection if success, otherwise null.
     */
    public Connection OpenConnection(String userName, String userPassword) {
        try {
            oracleDataSource = new OracleDataSource();
            oracleDataSource.setURL(connectionString);
            oracleDataSource.setUser(userName);
            oracleDataSource.setPassword(userPassword);

            dbConnection = oracleDataSource.getConnection();
            dbConnection.setAutoCommit(true);
        } catch (SQLException ex) {
            dbConnection = null;
        }

        return dbConnection;
    }

    /**
     * Method returns db connection.
     *
     * @author Ondřej Zemánek
     * @return returns db connection.
     */
    public Connection GetConnection() {
        return dbConnection;
    }

    /**
     * Method for closing db connection.
     *
     * @author Ondřej Zemánek
     * @return returns true if success, otherwise false.
     */
    public boolean CloseConnection() {
        if (dbConnection == null) {
            return true;
        }

        try {
            dbConnection.close();

            return dbConnection.isClosed();
        } catch(SQLException ex) {
        }

        return false;
    }

    /**
     * Method for running init sql script.
     *
     * @author Ondřej Zemánek
     */
    public void RunScript() throws SQLException {
        dbConnection.setAutoCommit(false);
        Statement stmt = dbConnection.createStatement();
        ArrayList<String> initScripts = new ArrayList<>();

        initScripts.add("DROP TABLE zamestnanec");
        initScripts.add("DROP TABLE typ");
        initScripts.add("DROP TABLE umisteni");
        initScripts.add("DROP TABLE vybeh");
        initScripts.add("DROP TABLE zvire");
        initScripts.add("DROP TABLE druhZvirete");
        initScripts.add("DROP TABLE zoo");
        initScripts.add("DROP INDEX indexZoo");
        initScripts.add("DELETE FROM USER_SDO_GEOM_METADATA WHERE TABLE_NAME = 'ZOO' AND COLUMN_NAME = 'PROSTOROVADATA'");

        for (String query : initScripts) {
            try {
                stmt.execute(query);
            } catch (SQLException ex) {
            }
        }

        initScripts.clear();
        dbConnection.commit();

        initScripts.add("CREATE TABLE zamestnanec (id INTEGER primary key NOT NULL, jmeno VARCHAR2(40) NOT NULL, prijmeni VARCHAR2(40) NOT NULL, login VARCHAR2(40) NOT NULL UNIQUE, heslo CHAR(16) NOT NULL)");
        initScripts.add("CREATE TABLE zvire (id INTEGER primary key NOT NULL, jmeno VARCHAR2(40) NOT NULL, idDruhu INTEGER NULL, pohlavi VARCHAR2(6) NOT NULL, datumNarozeni DATE NOT NULL, datumUmrti DATE NULL, datumPorizeni DATE NULL, fotka ORDSYS.ORDImage NOT NULL, fotkaSi ORDSYS.SI_StillImage ,fotkaAc ORDSYS.SI_AverageColor, fotkaCh ORDSYS.SI_ColorHistogram , fotkaPc ORDSYS.SI_PositionalColor , fotkaTx ORDSYS.SI_Texture)");
        initScripts.add("CREATE TABLE druhZvirete (id INTEGER primary key NOT NULL, jmenoDruhu VARCHAR2(50) NOT NULL, celed VARCHAR2(50) NOT NULL, oblast VARCHAR2(50) NOT NULL, info NCLOB)");
        initScripts.add("CREATE TABLE zoo (id INTEGER primary key NOT NULL, prostorovaData SDO_GEOMETRY NULL, idVybehu INTEGER NULL, idTyp INTEGER NOT NULL)");
        initScripts.add("CREATE TABLE typ (id INTEGER primary key NOT NULL, jmeno VARCHAR2(15), barva VARCHAR(6))");
        initScripts.add("CREATE TABLE umisteni (id INTEGER primary key NOT NULL, idVybehu INTEGER NOT NULL, idZvirete INTEGER NOT NULL, umisteniOd TIMESTAMP NOT NULL, umisteniDo TIMESTAMP)");
        initScripts.add("CREATE TABLE vybeh (id INTEGER primary key NOT NULL, jmeno VARCHAR2(20), info NCLOB)");

        initScripts.add("ALTER TABLE zvire " +
                "ADD CONSTRAINT fk_zvireDruh " +
                "  FOREIGN KEY (idDruhu) " +
                "  REFERENCES druhZvirete(id) " +
                "  ON DELETE SET NULL");

        initScripts.add("INSERT INTO USER_SDO_GEOM_METADATA VALUES ( " +
                " 'zoo', 'prostorovaData', " +
                " SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', 0, 150, 0.1), SDO_DIM_ELEMENT('Y', 0, 150, 0.1)), " +
                " NULL " +
                ")");

        initScripts.add("CREATE INDEX indexZoo ON zoo (prostorovaData) indextype is MDSYS.SPATIAL_INDEX");

        for (String query : initScripts) {
            stmt.execute(query);
        }

        initScripts.clear();
        dbConnection.commit();

        initScripts.add("CREATE OR REPLACE PROCEDURE deleteUmisteni(pId IN INT, pFrom IN TIMESTAMP, pTo IN TIMESTAMP, pNewId IN INT) IS" +
                "  animalId INT;" +
                "  dateFrom TIMESTAMP;" +
                "  dateTo TIMESTAMP;" +
                "  isUpdated BOOLEAN;" +
                "  newId INT;" +
                "BEGIN" +
                "  animalId := pId;" +
                "  dateFrom := pFrom;" +
                "  dateTo := pTo;" +
                "  newId := pNewId;" +
                "" +
                "  FOR i IN (SELECT um.id, um.idvybehu, um.umisteniOd, um.umisteniDo FROM umisteni um" +
                "              WHERE um.idZvirete = animalId AND (um.umisteniOd <= dateTo AND NVL(um.umisteniDo, SYSDATE) >= dateFrom))" +
                "  LOOP" +
                "    isUpdated := FALSE;" +
                "    IF (i.umisteniOd < dateFrom) THEN" +
                "      UPDATE umisteni" +
                "      SET umisteniDo = dateFrom" +
                "      WHERE id = i.id;" +
                "" +
                "      i.umisteniOd := dateFrom;" +
                "      isUpdated := TRUE;" +
                "    END IF;" +
                "" +
                "    IF (i.umisteniOd >= dateFrom AND NVL(i.umisteniDo, SYSDATE) <= dateTo) THEN" +
                "      IF (isUpdated = FALSE) THEN" +
                "        DELETE umisteni" +
                "        WHERE id = i.id;" +
                "      END IF;" +
                "    ELSE" +
                "      i.umisteniOd := dateTo;" +
                "      IF (isUpdated = TRUE) THEN" +
                "        INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo)" +
                "        VALUES (newId, i.idvybehu, animalId, i.umisteniOd, i.umisteniDo);" +
                "        newId := newId +1;" +
                "      ELSE" +
                "        UPDATE umisteni" +
                "        SET umisteniDo = i.umisteniDo, umisteniOd = i.umisteniOd" +
                "        WHERE id = i.id;" +
                "     END IF;" +
                "    END IF;" +
                "  END LOOP;" +
                "  COMMIT;" +
                "END;");

        initScripts.add("CREATE OR REPLACE PROCEDURE updateUmisteni(pAnimalId IN INT, pFrom IN TIMESTAMP, pTo IN TIMESTAMP, pNewUmisteni IN INT, pNewId IN INT) IS " +
                " " +
                "  animalId INT; " +
                "  dateFrom TIMESTAMP; " +
                "  dateTo TIMESTAMP; " +
                "  newUmisteni INT; " +
                "  isUpdated BOOLEAN; " +
                "  oldFrom TIMESTAMP; " +
                "  oldTo TIMESTAMP; " +
                "  oldLoc INT; " +
                "  newId INT; " +
                " " +
                "BEGIN " +
                " " +
                "  animalId  := pAnimalId; " +
                "  dateFrom := pFrom; " +
                "  dateTo := pTo; " +
                "  newUmisteni := pNewUmisteni; " +
                "  newId := pNewId; " +
                " " +
                " " +
                "  FOR i IN (SELECT * FROM umisteni um " +
                "              WHERE um.idZvirete = animalId AND (um.umisteniOd <= dateTo AND NVL(um.umisteniDo, SYSDATE) >= dateFrom) " +
                "              ORDER BY um.umisteniOd) " +
                "  LOOP " +
                " " +
                "    isUpdated := FALSE; " +
                "    IF (i.umisteniOd  < dateFrom) THEN " +
                "      IF (i.idVybehu = newUmisteni) THEN " +
                " " +
                "        dateFrom := i.umisteniOd; " +
                "      ELSE " +
                " " +
                "        oldFrom := i.umisteniOd; " +
                "        oldTo:= dateFrom; " +
                "        oldLoc := i.idVybehu; " +
                " " +
                "        UPDATE umisteni " +
                "        SET umisteniDo = oldTo, umisteniOd = oldFrom, idVybehu = oldLoc " +
                "        WHERE id = i.id; " +
                " " +
                "        i.umisteniOd := dateFrom; " +
                "        isUpdated := TRUE; " +
                "      END IF; " +
                "    END IF; " +
                "        IF (NVL(i.umisteniDo, SYSDATE) <= dateTo) THEN " +
                "          IF (isUpdated = FALSE) THEN " +
                "        DELETE umisteni " +
                "        WHERE id = i.id; " +
                "      END IF; " +
                "        ELSE IF (i.idVybehu = newUmisteni) THEN " +
                "                dateTo := i.umisteniDo; " +
                " " +
                "        DELETE umisteni " +
                "        WHERE id = i.id; " +
                "        ELSE IF (isUpdated = FALSE) THEN " +
                " " +
                "        DELETE umisteni " +
                "        WHERE id = i.id; " +
                "    END IF; " +
                " " +
                "      i.umisteniOd := dateTo; " +
                " " +
                "        INSERT INTO umisteni (id, idVybehu, idZvirete, umisteniOd, umisteniDo) " +
                "    VALUES (newId, i.idvybehu, animalId, i.umisteniOd, i.umisteniDo); " +
                "    newId := newId + 1; " +
                "    END IF; END IF; " +
                "  END LOOP; " +
                "          IF (dateTo > TO_TIMESTAMP('9998/01/01', 'yyyy/mm/dd hh24:mi:ss')) THEN " +
                "    dateTo := NULL; " +
                "    END IF; " +
                "  INSERT INTO umisteni (id, idVybehu, idZvirete, umisteniOd, umisteniDo) VALUES (newId, newUmisteni, animalId, dateFrom, dateTo); " +
                " " +
                "  COMMIT; " +
                "END;");

        for (String query : initScripts) {
            stmt.execute(query);
        }

        initScripts.clear();
        dbConnection.commit();

        initScripts.add("INSERT INTO Typ (id, jmeno, barva) VALUES (1, 'Stromy', '1A5422')");
        initScripts.add("INSERT INTO Typ (id, jmeno, barva) VALUES (2, 'Trávník', '3DB443')");
        initScripts.add("INSERT INTO Typ (id, jmeno, barva) VALUES (3, 'Cesta', '566063')");
        initScripts.add("INSERT INTO Typ (id, jmeno, barva) VALUES (4, 'Voda', '4494F2')");
        initScripts.add("INSERT INTO Typ (id, jmeno, barva) VALUES (5, 'Kámen', '98B4BD')");
        initScripts.add("INSERT INTO Typ (id, jmeno, barva) VALUES (6, 'Budova', 'C96148')");

        initScripts.add("INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('1', 'Gorila', 'Hominidae', 'Afrika', 'Gorila (Gorilla) je rod největších žijících primátů. Žijí na zemi a jsou převážně býložravé. Obývají lesy střední Afriky. Gorily se dělí na dva druhy a čtyři poddruhy (podle některých biologů pět poddruhů). DNA gorily je z 98 až 99 % shodná s DNA člověka [1] a po šimpanzích jsou gorily nejbližší příbuzní člověka. Gorily obývají tropické a subtropické lesy. Ačkoliv se jejich výskyt omezuje jen na několik procent rozlohy Afriky, jejich biotopy pokrývají velký rozsah nadmořských výšek. Gorila východní (horská) žije v pohoří Virunga ve výškách 2225 až 4267 m nad mořem. Gorila západní (nížinná) žije v hustých lesích a mokřinách i v úrovni mořské hladiny.')");
        initScripts.add("INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('2', 'Tygr', 'Felidae', 'Asie', 'Tygři jsou obvykle samotáři. Jejich hlavní kořistí jsou velcí sudokopytníci. Obývají různé biotopy, od tropických lesů, přes stepi a mokřady až k severským tundrám. Původně areál rozšíření sahal od Kavkazu a východního Turecka do středoasijských stepí, od indického subkontinentu přes Zadní Indii a východní Čínu na ruský dálný východ. Areál také zahrnoval indonéské ostrovy Sumatra, Jáva a Bali.')");
        initScripts.add("INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('3', 'Lev', 'Felidae', 'Afrika', 'Lev pustinný (Panthera leo) je savec čeledi kočkovitých a jeden ze čtyř druhů velkých koček rodu Panthera. Lev je po tygrovi druhá největší kočkovitá šelma. Samci váží 150–250 kg a samice 90–165 kg. V divočině se lvi dožívají 10–14 let, kdežto v zajetí se mohou dožít i věku 20 let. Dříve se lvi vyskytovali v celé Africe, Asii, Evropě a dokonce i v Americe, dnes se vyskytují pouze v Africe a na několika místech v Indii. Jsou to společenská zvířata a loví ve smečkách. U lvů se projevuje výrazný pohlavní dimorfismus, hlavním a určujícím rysem lvích samců je jejich hříva.')");
        initScripts.add("INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('4', 'Slon Africký', 'Elephantidae', 'Afrika', 'Slon je souhrnný český název pro dva rody chobotnatců - Loxodonta a Elephas. Slon je největší žijící suchozemský savec. Při narození váží okolo 100 kg. Samice slona je březí 20 až 22 měsíců, což je nejdelší doba březosti u suchozemského zvířete. Slon se dožívá 60 až 70 let. Největšího slona zastřelili v Angole v roce 1974, vážil 12 000 kilogramů.')");
        initScripts.add("INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('5', 'Lama', 'Camelidae)', 'Jižní Amerika', 'Lamy, známé též jako velbloudi Jižní Ameriky, jsou zvířata z čeledi velbloudovití, která se přizpůsobila těžkému životu v Andách. V posledních 40 letech se šíří do celého světa jako chovný dobytek či zvěř chovaná v zoologických zahradách.')");

        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('1', 'Maruška', '2', 'samice', TO_DATE('1993/12/22', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/01', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('2', 'Lucka', '2', 'samice', TO_DATE('2000/09/15', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/30', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('3', 'Radek', '2', 'samec', TO_DATE('1997/04/20', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/02/01', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('4', 'Iva', '1', 'samice', TO_DATE('1966/05/14', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/03', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('5', 'Maruška', '1', 'samice', TO_DATE('1967/05/14', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/03', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('6', 'Petr', '1', 'samec', TO_DATE('1966/07/18', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/10', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('7', 'Tomáš', '1', 'samec', TO_DATE('1967/03/18', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/10', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('8', 'Andrea', '3', 'samice', TO_DATE('1986/06/15', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/11', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('9', 'Daniela', '3', 'samice', TO_DATE('1977/06/12', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/02/11', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('10', 'Ladík', '3', 'samec', TO_DATE('1977/09/20', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/02/11', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('11', 'Berta', '4', 'samice', TO_DATE('1955/06/22', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/13', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('12', 'Blažena', '5', 'samice', TO_DATE('1970/06/16', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/29', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('13', 'Tereza', '5', 'samice', TO_DATE('1987/04/16', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/28', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('14', 'Igor', '5', 'samec', TO_DATE('1990/09/15', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/23', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('15', 'Lukáš', '5', 'samec', TO_DATE('1999/07/18', 'yyyy/mm/dd'), TO_DATE('2005/06/08', 'yyyy/mm/dd'),  null, ORDSYS.ORDIMAGE.init())");
        initScripts.add("INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('16', 'Zdena', '1', 'samice', TO_DATE('1920/10/22', 'yyyy/mm/dd'), TO_DATE('2000/03/10', 'yyyy/mm/dd'),  null, ORDSYS.ORDIMAGE.init())");

        initScripts.add("INSERT INTO vybeh (id, jmeno, info) VALUES('1', 'Pavilon goril', 'Výstavba pavilonu začala v roce 2005 a byla dokončena v roce 2007....')");
        initScripts.add("INSERT INTO vybeh (id, jmeno, info) VALUES('2', 'Pavilon koček', 'Výstavba pavilonu začala v roce 2000 a byla dokončena v roce 2002....')");
        initScripts.add("INSERT INTO vybeh (id, jmeno, info) VALUES('3', 'Pavilon slonů', 'Výstavba pavilonu začala v roce 1990 a byla dokončena v roce 1991....')");
        initScripts.add("INSERT INTO vybeh (id, jmeno, info) VALUES('4', 'Výběh lam', 'Výstavba nejnovějíšího přírůstki naší zoo začala v roce 2015 a skočila v roce 2016')");
        initScripts.add("INSERT INTO vybeh (id, jmeno, info) VALUES('5', 'Karanténa', 'Všechny zvířata co jsou přemístěna do Zoo, nebo se v ní narodí musejí projít vstupní prohlídkou v karanténě')");
        initScripts.add("INSERT INTO vybeh (id, jmeno, info) VALUES('6', 'Ošetřovna', 'Pokuď je zvíře zraněné nebo nemocné, tak je dočasně umístěno na ošetřovně')");

        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('1', '1', '4', TO_TIMESTAMP('1966/05/15 10:00:11', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('2', '1', '5',TO_TIMESTAMP('1967/05/20 09:00:00', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('3', '1', '6',TO_TIMESTAMP('1966/07/18 12:00:30', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2016/09/01 12:00:00', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('7', '1', '7',TO_TIMESTAMP('1967/03/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('4', '2', '1',TO_TIMESTAMP('1993/12/23 10:00:00', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('5', '2', '2',TO_TIMESTAMP('2000/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('6', '2', '3',TO_TIMESTAMP('1997/04/21 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('8', '2', '8',TO_TIMESTAMP('1986/06/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('9', '2', '9',TO_TIMESTAMP('1977/06/13 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('10', '2', '10',TO_TIMESTAMP('1967/03/20 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('11', '3', '11',TO_TIMESTAMP('1955/06/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('12', '4', '12',TO_TIMESTAMP('1970/06/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('13', '4', '13',TO_TIMESTAMP('1987/04/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('14', '4', '14',TO_TIMESTAMP('1990/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('15', '4', '15',TO_TIMESTAMP('1999/07/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null)");

        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('16', '5', '4', TO_TIMESTAMP('1966/05/14 01:00:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1966/05/15 10:00:11', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('17', '5', '5', TO_TIMESTAMP('1967/05/19 10:01:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1967/05/20 09:00:00', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('18', '5', '6', TO_TIMESTAMP('1966/07/17 15:00:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1966/07/18 12:00:30', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('19', '5', '1',TO_TIMESTAMP('1993/12/22 02:10:00', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1993/12/23 10:00:00', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('20', '5', '2',TO_TIMESTAMP('2000/09/15 05:15:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2000/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('21', '5', '3',TO_TIMESTAMP('1997/04/20 13:12:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1997/04/21 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('22', '5', '7',TO_TIMESTAMP('1967/03/18 20:12:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1967/03/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('23', '5', '8',TO_TIMESTAMP('1986/06/15 18:18:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1986/06/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('24', '5', '9',TO_TIMESTAMP('1977/06/12 19:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1977/06/13 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('25', '5', '10',TO_TIMESTAMP('1967/03/19 15:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1967/03/20 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('26', '5', '11',TO_TIMESTAMP('1955/06/22 20:51:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1955/06/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('27', '5', '12',TO_TIMESTAMP('1970/06/16 18:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1970/06/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('28', '5', '13',TO_TIMESTAMP('1987/04/16 01:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1987/04/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('29', '5', '14',TO_TIMESTAMP('1990/09/15 14:15:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1990/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('30', '5', '15',TO_TIMESTAMP('1999/07/18 19:01:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1999/07/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('31', '5', '16',TO_TIMESTAMP('1920/10/22 10:01:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1920/10/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('32', '6', '6',TO_TIMESTAMP('2016/09/01 12:00:00', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2016/09/05 12:00:00', 'yyyy/mm/dd hh24:mi:ss'))");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('33', '1', '6',TO_TIMESTAMP('2016/09/05 12:00:00', 'yyyy/mm/dd hh24:mi:ss'), null)");
        initScripts.add("INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('34', '1', '16',TO_TIMESTAMP('1920/10/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2000/03/10 23:36:00', 'yyyy/mm/dd hh24:mi:ss'))");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (2,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "0.0,0.0, " +
                "25.0,0.0, " +
                "25.0,45.0, " +
                "0.0,45.0, " +
                "0.0,0.0)),3,2)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (3,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "35.0,10.0, " +
                "55.0,30.0, " +
                "55.0,45.0, " +
                "35.0,45.0, " +
                "35.0,10.0)),2,2)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (4,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "50.0,0.0, " +
                "100.0,0.0, " +
                "100.0,20.0, " +
                "70.0,20.0, " +
                "50.0,0.0)),1,2)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (5,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "70.0,30.0, " +
                "100.0,30.0, " +
                "100.0,70.0, " +
                "70.0,70.0, " +
                "70.0,30.0)),4,2)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (6,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "60.0,80.0, " +
                "80.0,80.0, " +
                "80.0,100.0, " +
                "60.0,100.0, " +
                "60.0,80.0)),5,6)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (7,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "80.0,80.0, " +
                "100.0,80.0, " +
                "100.0,100.0, " +
                "80.0,100.0, " +
                "80.0,80.0)),6,6)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (8,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY( " +
                "0.0,80.0, " +
                "50.0,80.0, " +
                "50.0,100.0, " +
                "0.0,100.0, " +
                "0.0,80.0)),null,4)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (9,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(5,52.5,7.5,55.0,5,57.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (10,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(15,52.5,17.5,55.0,15,57.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (11,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(25,52.5,27.5,55.0,25,57.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (12,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(35,52.5,37.5,55.0,35,57.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (13,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(45,52.5,47.5,55.0,45,57.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (14,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(55,52.5,57.5,55.0,55,57.5)),null,1)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (15,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(10,27.5,12.5,30.0,10,32.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (16,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(20,37.5,22.5,40.0,20,42.5)),null,1)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (17,SDO_GEOMETRY(2003, NULL, NULL, SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(20,7.5,22.5,10.0,20,12.5)),null,1)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (18,SDO_GEOMETRY(2002,null,null, " +
                "SDO_ELEM_INFO_ARRAY(1,2,1), " +
                "SDO_ORDINATE_ARRAY(0.0,50.0,30.0,50.0,30.0,5.0,40.0,5.0,60.0,25.0,100.0,25.0)),null,3)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (19,SDO_GEOMETRY(2002,null,null, " +
                "SDO_ELEM_INFO_ARRAY(1,2,1), " +
                "SDO_ORDINATE_ARRAY(0.0,75.0,60.0,75.0,60.0,25.0,100.0,25.0)),null,3)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (20,SDO_GEOMETRY(2002,null,null, " +
                "SDO_ELEM_INFO_ARRAY(1,2,1), " +
                "SDO_ORDINATE_ARRAY(0.0,75.0,70.0,75.0,70.0,80.0)),null,3)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (21,SDO_GEOMETRY(2002,null,null, " +
                "SDO_ELEM_INFO_ARRAY(1,2,1), " +
                "SDO_ORDINATE_ARRAY(0.0,75.0,90.0,75.0,90.0,80.0)),null,3)");

        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (22,SDO_GEOMETRY(2005, NULL, NULL," + 
        		"SDO_ELEM_INFO_ARRAY(1,1,2)," + 
        		"SDO_ORDINATE_ARRAY(" + 
        		"10.0,70.0," + 
        		"20.0,70.0" + 
        		")),null,5)");
        initScripts.add("INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (23,SDO_GEOMETRY(2005, NULL, NULL," + 
        		"SDO_ELEM_INFO_ARRAY(1,1,2)," + 
        		"SDO_ORDINATE_ARRAY(" + 
        		"40.0,70.0," + 
        		"50.0,70.0" + 
        		")),null,5)");

        initScripts.add("INSERT INTO zamestnanec (id, jmeno, prijmeni, login, heslo) values ('1', 'Daniel', 'Vosáhlo', 'xvosah00', 'gopbkiw5wfygk82w')");
        initScripts.add("INSERT INTO zamestnanec (id, jmeno, prijmeni, login, heslo) values ('2', 'Ondřej', 'Zemánek', 'xzeman53', 'tokvkiw58fygk8kw')");
        initScripts.add("INSERT INTO zamestnanec (id, jmeno, prijmeni, login, heslo) values ('3', 'Jan', 'Pavlica', 'xpavli78', 't2mvkizmqf8gk2kp')");

        for (String query : initScripts) {
            stmt.execute(query);
        }

        initScripts.clear();

        stmt.close();

        dbConnection.commit();

        //Upload images to animals
        ZvireModel zv = new ZvireModel(dbConnection);

        zv.UploadPhoto(1,"images/tiger_f2.jpg");
        zv.UploadPhoto(2,"images/tiger_f1.jpg");
        zv.UploadPhoto(3,"images/tiger_m1.jpg");

        zv.UploadPhoto(4,"images/gorila2.jpg");
        zv.UploadPhoto(5,"images/gorila3.jpg");
        zv.UploadPhoto(6,"images/gorila1.jpg");
        zv.UploadPhoto(7,"images/gorila4.jpg");

        zv.UploadPhoto(8,"images/lion_f2.jpg");
        zv.UploadPhoto(9,"images/lion_f1.jpg");
        zv.UploadPhoto(10,"images/lion_m1.jpg");

        zv.UploadPhoto(11,"images/elephant1.jpg");

        zv.UploadPhoto(12,"images/lama3.jpg");
        zv.UploadPhoto(13,"images/lama1.jpg");
        zv.UploadPhoto(14,"images/lama4.jpg");

        dbConnection.setAutoCommit(true);
    }
}
