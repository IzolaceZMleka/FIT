DROP TABLE zamestnanec;

DROP TABLE typ;

DROP TABLE umisteni;
DROP TABLE vybeh;
DROP TABLE zvire;
DROP TABLE druhZvirete;
DROP TABLE zoo;

DROP INDEX indexZoo;

-- nazvy tabulky a sloupce musi byt velkymi pismeny
DELETE FROM USER_SDO_GEOM_METADATA WHERE
	TABLE_NAME = 'ZOO' AND COLUMN_NAME = 'PROSTOROVADATA';

CREATE TABLE zamestnanec (id INTEGER primary key NOT NULL, jmeno VARCHAR2(40) NOT NULL, prijmeni VARCHAR2(40) NOT NULL, login VARCHAR2(40) NOT NULL UNIQUE, heslo CHAR(16) NOT NULL);

CREATE TABLE zvire (id INTEGER primary key NOT NULL, jmeno VARCHAR2(40) NOT NULL, idDruhu INTEGER NULL, pohlavi VARCHAR2(6) NOT NULL, datumNarozeni DATE NOT NULL, datumUmrti DATE NULL, datumPorizeni DATE NULL, fotka ORDSYS.ORDImage NOT NULL, fotkaSi ORDSYS.SI_StillImage ,fotkaAc ORDSYS.SI_AverageColor
, fotkaCh ORDSYS.SI_ColorHistogram , fotkaPc ORDSYS.SI_PositionalColor , fotkaTx ORDSYS.SI_Texture);

CREATE TABLE druhZvirete (id INTEGER primary key NOT NULL, jmenoDruhu VARCHAR2(50) NOT NULL, celed VARCHAR2(50) NOT NULL, oblast VARCHAR2(50) NOT NULL, info NCLOB);

CREATE TABLE zoo (id INTEGER primary key NOT NULL, prostorovaData SDO_GEOMETRY NULL, idVybehu INTEGER NULL, idTyp INTEGER NOT NULL);

CREATE TABLE typ (id INTEGER primary key NOT NULL, jmeno VARCHAR2(15), barva VARCHAR(6));

CREATE TABLE umisteni (id INTEGER primary key NOT NULL, idVybehu INTEGER NOT NULL, idZvirete INTEGER NOT NULL, umisteniOd TIMESTAMP NOT NULL, umisteniDo TIMESTAMP);

CREATE TABLE vybeh (id INTEGER primary key NOT NULL, jmeno VARCHAR2(20), info NCLOB);

ALTER TABLE zvire
ADD CONSTRAINT fk_zvireDruh
  FOREIGN KEY (idDruhu)
  REFERENCES druhZvirete(id)
  ON DELETE SET NULL;
  

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'zoo', 'prostorovaData',
	-- souradnice X,Y s hodnotami 0-150 a presnosti 0.1 bod (velikost mrizky a hustota budu v planu z prikladu, napr. kulate rohy komunikace s presnosti 1 bod a stromy v zeleni s presnosti 0.1 bod)
	SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', 0, 150, 0.1), SDO_DIM_ELEMENT('Y', 0, 150, 0.1)),
	-- lokalni (negeograficky) souradnicovy system (v analytickych fcich neuvadet jednotky)
	NULL
);


CREATE INDEX indexZoo ON zoo (prostorovaData) indextype is MDSYS.SPATIAL_INDEX ;

/
--@author Daniel Vosáhlo
CREATE OR REPLACE PROCEDURE deleteUmisteni(pId IN INT, pFrom IN TIMESTAMP, pTo IN TIMESTAMP, pNewId IN INT) IS
  animalId INT;
  dateFrom TIMESTAMP;
  dateTo TIMESTAMP;
  isUpdated BOOLEAN;
  newId INT;
BEGIN
  animalId := pId;
  dateFrom := pFrom;
  dateTo := pTo;
  newId := pNewId;

  FOR i IN (SELECT um.id, um.idvybehu, um.umisteniOd, um.umisteniDo FROM umisteni um
              WHERE um.idZvirete = animalId AND (um.umisteniOd <= dateTo AND NVL(um.umisteniDo, SYSDATE) >= dateFrom))
  LOOP
    isUpdated := FALSE;
    IF (i.umisteniOd < dateFrom) THEN
      UPDATE umisteni
      SET umisteniDo = dateFrom
      WHERE id = i.id;

      i.umisteniOd := dateFrom;
      isUpdated := TRUE;
    END IF;

    IF (i.umisteniOd >= dateFrom AND NVL(i.umisteniDo, SYSDATE) <= dateTo) THEN
      IF (isUpdated = FALSE) THEN
        DELETE umisteni
        WHERE id = i.id;
      END IF;
    ELSE
      i.umisteniOd := dateTo;
      IF (isUpdated = TRUE) THEN
        INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo)
        VALUES (newId, i.idvybehu, animalId, i.umisteniOd, i.umisteniDo);
        newId := newId +1;
      ELSE
        UPDATE umisteni
        SET umisteniDo = i.umisteniDo, umisteniOd = i.umisteniOd
        WHERE id = i.id;
     END IF;
    END IF;
  END LOOP;
  COMMIT;
END;
/

--@author Daniel Vosáhlo
CREATE OR REPLACE PROCEDURE updateUmisteni(pAnimalId IN INT, pFrom IN TIMESTAMP, pTo IN TIMESTAMP, pNewUmisteni IN INT, pNewId IN INT) IS
-- Inicialize vars
  animalId INT;
  dateFrom TIMESTAMP;
  dateTo TIMESTAMP;
  newUmisteni INT;
  isUpdated BOOLEAN;
  oldFrom TIMESTAMP;
  oldTo TIMESTAMP;
  oldLoc INT;
  newId INT;

BEGIN
-- Filling vars
  animalId  := pAnimalId;
  dateFrom := pFrom;
  dateTo := pTo;
  newUmisteni := pNewUmisteni;
  newId := pNewId;

  -- For all results which are lower or equal that he input interval AND their and is higher that the end of the interval
  FOR i IN (SELECT * FROM umisteni um
              WHERE um.idZvirete = animalId AND (um.umisteniOd <= dateTo AND NVL(um.umisteniDo, SYSDATE) >= dateFrom)
              ORDER BY um.umisteniOd)
  LOOP

    isUpdated := FALSE;
    IF (i.umisteniOd  < dateFrom) THEN
      IF (i.idVybehu = newUmisteni) THEN
      --We set the start of the new interval to old interval start
        dateFrom := i.umisteniOd;
      ELSE
      --We temporary store the values
        oldFrom := i.umisteniOd;
        oldTo:= dateFrom;
        oldLoc := i.idVybehu;

        UPDATE umisteni
        SET umisteniDo = oldTo, umisteniOd = oldFrom, idVybehu = oldLoc
        WHERE id = i.id;
        --We update the i.from do new interval from
        i.umisteniOd := dateFrom;
        isUpdated := TRUE;
      END IF;
    END IF;
    --If the i.to is <= new interval to
    IF (NVL(i.umisteniDo, SYSDATE) <= dateTo) THEN
    --We didnt updated
      IF (isUpdated = FALSE) THEN
        DELETE umisteni
        WHERE id = i.id;
      END IF;
    --Or we updated and idvybehu are equal
    ELSE IF (i.idVybehu = newUmisteni) THEN
        --We set new interval to to current interval to
        dateTo := i.umisteniDo;

        DELETE umisteni
        WHERE id = i.id;
    --Or we didnt updated
    ELSE IF (isUpdated = FALSE) THEN

        DELETE umisteni
        WHERE id = i.id;
    END IF;

      i.umisteniOd := dateTo;

    --We insert new changed interval
    INSERT INTO umisteni (id, idVybehu, idZvirete, umisteniOd, umisteniDo)
    VALUES (newId, i.idvybehu, animalId, i.umisteniOd, i.umisteniDo);
    newId := newId + 1;
    END IF; END IF;
  END LOOP;
      --We must check for 9998/01/01 (means null)
    IF (dateTo > TO_TIMESTAMP('9998/01/01', 'yyyy/mm/dd hh24:mi:ss')) THEN
    dateTo := NULL;
    END IF;
  INSERT INTO umisteni (id, idVybehu, idZvirete, umisteniOd, umisteniDo) VALUES (newId, newUmisteni, animalId, dateFrom, dateTo);

  COMMIT;
END;
/

COMMIT;
