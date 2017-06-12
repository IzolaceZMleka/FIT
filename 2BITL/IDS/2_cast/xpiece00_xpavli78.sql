-- Authors: Jan Pavlica - loxin:xpavli78, Adam Piecek - login: xpiece00
DROP TABLE Seznam_rekvizit_pro_hru;
DROP TABLE Seznam_roli;
DROP TABLE Prestavky;
DROP TABLE Obsazeni_inscenace;
DROP TABLE Seznam_rekvizit_pro_roli;
DROP TABLE Naucene_role;
DROP TABLE Hry;
DROP TABLE Jednotlive_role;
DROP TABLE Inscenace;
DROP TABLE Herci;
DROP TABLE Kulisy;
DROP TABLE Kostymy;
DROP TABLE Doplnky;
DROP TABLE Rekvizity;
DROP TABLE Autori;
DROP TABLE Reziseri;
DROP TABLE Poradatele;

DROP SEQUENCE seq_id_prestavky;
DROP SEQUENCE seq_id_hry;
DROP SEQUENCE seq_id_role;
DROP SEQUENCE seq_id_inscenace;
DROP SEQUENCE seq_id_herce;
DROP SEQUENCE seq_id_kulisy;
DROP SEQUENCE seq_id_kostymu;
DROP SEQUENCE seq_id_doplnku;
DROP SEQUENCE seq_id_rekvizity;
DROP SEQUENCE seq_id_autora;
DROP SEQUENCE seq_id_rezisera;
DROP SEQUENCE seq_ic_poradatele;

CREATE TABLE Autori(
IdAutora INTEGER NOT NULL,
Jmeno VARCHAR(10) NOT NULL,
Prijmeni VARCHAR(15),
Datum_narozeni DATE,
Datum_umrti DATE,
Zeme_puvodu VARCHAR(25),

CONSTRAINT pk_autora PRIMARY KEY (IdAutora)
);

CREATE TABLE Reziseri(
IdRezisera INTEGER NOT NULL,
Jmeno VARCHAR(10) NOT NULL,
Prijmeni VARCHAR(15) NOT NULL,
Kontakt CHAR(20) NOT NULL,

CONSTRAINT pk_rezisera PRIMARY KEY (IdRezisera)
);

CREATE TABLE Hry(
IdHry INTEGER,
IdAutora INTEGER,
IdRezisera INTEGER,
Jmeno VARCHAR(20),
Zanr VARCHAR(10),
Smer VARCHAR(120),

CONSTRAINT pk_hry PRIMARY KEY (IdHry),
CONSTRAINT fk_autora FOREIGN KEY (IdAutora) REFERENCES Autori(IdAutora),
CONSTRAINT fk_rezisera FOREIGN KEY (IdRezisera) REFERENCES Reziseri(IdRezisera)
);

CREATE TABLE Poradatele(
IcPoradatele INTEGER NOT NULL,
Jmeno VARCHAR(50) NOT NULL,
Ulice VARCHAR(30),
Mesto VARCHAR(15),
Psc INTEGER,
Kontakt CHAR(20) NOT NULL,
Poznamky VARCHAR(140),

CONSTRAINT pk_poradatele PRIMARY KEY (IcPoradatele)
);

CREATE TABLE Jednotlive_role(
IdRole INTEGER,
Jmeno VARCHAR(20),
Obtiznost VARCHAR(20),
Pohlavi VARCHAR(10),
Casova_narocnost INTERVAL DAY TO SECOND,
Popis_charakteru VARCHAR(255),

CONSTRAINT pk_role PRIMARY KEY (IdRole)
);

CREATE TABLE Inscenace(
IdInscenace INTEGER NOT NULL,
IcPoradatele INTEGER NOT NULL,
Datum_a_cas TIMESTAMP,
Cena_za_predstaveni INTEGER,

CONSTRAINT pk_inscenace PRIMARY KEY (IdInscenace),
CONSTRAINT fk_poradatele FOREIGN KEY (IcPoradatele) REFERENCES Poradatele(IcPoradatele)
);

CREATE TABLE Herci(
IdHerce INTEGER NOT NULL,
Jmeno VARCHAR(10),
Prijmeni VARCHAR(15)NOT NULL,
Vek INTEGER,
Kontakt CHAR(20) NOT NULL,
Poznamky VARCHAR(160),

CONSTRAINT pk_herce PRIMARY KEY (IdHerce)
);

CREATE TABLE Rekvizity(
IdRekvizity INTEGER NOT NULL,
Jmeno VARCHAR(20) NOT NULL,
Popis VARCHAR(160),
Pocet_skladem INTEGER,

CONSTRAINT pk_rekvizity PRIMARY KEY (IdRekvizity)
);

CREATE TABLE Kostymy(
IdKostymu INTEGER NOT NULL,
Velikost VARCHAR(10),
Zasazeni VARCHAR(50),

CONSTRAINT fk_kostymu PRIMARY KEY (IdKostymu),
CONSTRAINT pk_kostymu FOREIGN KEY (IdKostymu) REFERENCES Rekvizity(IdRekvizity)
);

CREATE TABLE Kulisy(
IdKulisy INTEGER NOT NULL,
Velikost VARCHAR(50),
Umisteni CHAR(8),

CONSTRAINT check_umisteni CHECK (Umisteni='Interier' OR Umisteni='Exterier'),
CONSTRAINT fk_kulisy PRIMARY KEY (IdKulisy),
CONSTRAINT pk_kulisy FOREIGN KEY (IdKulisy) REFERENCES Rekvizity(IdRekvizity)
);

CREATE TABLE Doplnky(
IdDoplnku INTEGER NOT NULL,
Velikost VARCHAR(20),
Podrobnejsi_popis VARCHAR(160),

CONSTRAINT fk_doplnku PRIMARY KEY (IdDoplnku),
CONSTRAINT pk_doplnku FOREIGN KEY (IdDoplnku) REFERENCES Rekvizity(IdRekvizity)
);

CREATE TABLE Seznam_rekvizit_pro_hru(
IdRekvizity INTEGER,
IdHry INTEGER,

CONSTRAINT fk_rekvizity_hra FOREIGN KEY (IdRekvizity) REFERENCES Rekvizity(IdRekvizity),
CONSTRAINT fk_hry_rekvizita FOREIGN KEY (IdHry) REFERENCES Hry(IdHry),
CONSTRAINT pk_rekvizity_hry PRIMARY KEY (IdRekvizity, IdHry)
);

CREATE TABLE Seznam_roli(
IdRole INTEGER,
IdHry INTEGER,

CONSTRAINT fk_role_hra FOREIGN KEY (IdRole) REFERENCES Jednotlive_role(IdRole),
CONSTRAINT fk_hry_role FOREIGN KEY (IdHry) REFERENCES Hry(IdHry),
CONSTRAINT pk_role_hry PRIMARY KEY (IdRole, IdHry)
);

CREATE TABLE Obsazeni_inscenace(
IdInscenace INTEGER,
IdHerce INTEGER,

CONSTRAINT fk_incenace_herec FOREIGN KEY (IdInscenace) REFERENCES Inscenace(IdInscenace),
CONSTRAINT fk_herce_inscenace FOREIGN KEY (IdHerce) REFERENCES Herci(IdHerce),
CONSTRAINT pk_inscenace_herce PRIMARY KEY (IdInscenace, IdHerce)
);

CREATE TABLE Seznam_rekvizit_pro_roli(
IdRole INTEGER,
IdRekvizity INTEGER,

CONSTRAINT fk_rekvizity_role FOREIGN KEY (IdRekvizity) REFERENCES Rekvizity(IdRekvizity),
CONSTRAINT fk_role_rekvizita FOREIGN KEY (IdRole) REFERENCES Jednotlive_role(IdRole),
CONSTRAINT pk_rekvizity_role PRIMARY KEY (IdRekvizity, IdRole)
);

CREATE TABLE Naucene_role(
IdRole INTEGER,
IdHerce INTEGER,

CONSTRAINT fk_role_herec FOREIGN KEY (IdRole) REFERENCES Jednotlive_role(IdRole),
CONSTRAINT fk_herce_role FOREIGN KEY (IdHerce) REFERENCES Herci(IdHerce),
CONSTRAINT pk_herce_role PRIMARY KEY (IdHerce, IdRole)
);

CREATE TABLE Prestavky(
IdPrestavky INTEGER,
IdInscenace INTEGER,
Od TIMESTAMP,
Do TIMESTAMP,

CONSTRAINT fk_inscenace FOREIGN KEY (IdInscenace) REFERENCES Inscenace(IdInscenace) ON DELETE CASCADE,
CONSTRAINT pk_prestavky PRIMARY KEY(IdInscenace, IdPrestavky)
);

CREATE SEQUENCE seq_id_prestavky
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_role
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_hry
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_inscenace
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_herce
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_kulisy
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_kostymu
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_doplnku
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_rekvizity
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_autora
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_id_rezisera
START WITH 1
INCREMENT BY 1;

CREATE SEQUENCE seq_ic_poradatele
START WITH 1
INCREMENT BY 1;

INSERT INTO Autori VALUES (seq_id_autora.nextval, 'Carlo', 'Godolni', TO_DATE('25.02.1707', 'dd.mm.yyyy'), TO_DATE('06.02.1793', 'dd.mm.yyyy'), 'Francie');
INSERT INTO Reziseri VALUES (seq_id_rezisera.nextval, 'Ivan', 'Rajmont', '+420 555 444 333');
INSERT INTO Poradatele VALUES (seq_ic_poradatele.nextval, 'DivadelnÌ spoleËnost Josefa Dvo¯·ka', 'éitomÌ¯sk· 46', 'Praha 10', 10100, '+420 602 236 892', 'DobrÈ zkuöenosti'); 

INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='DivadelnÌ spoleËnost Josefa Dvo¯·ka'), TO_DATE('22.5.2016 20:00', 'dd.mm.yyyy HH24:MI'), 300);
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 1, TO_DATE('21:20', 'HH24:MI'), TO_DATE('21:30', 'HH24:MI'));

INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Godolni'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Rajmont'), 'Sluha dvou p·n˘', 'Komedie', 'italsk· komedie v obdobÌ klasicismu (Commedia dell¥arte)');

INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'd˘m', 'hnÏd˝, vypad· sta¯e', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'koöile', 'pruhovan·', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'ö·tek', 'zmuchlan˝, modrÈ barvy', 1);
INSERT INTO Kulisy VALUES (1, '10x4 metru, 4x4 po slozeni', 'Interier');
INSERT INTO Kostymy VALUES (2, 'L', '18 stoleti Francie');
INSERT INTO Doplnky VALUES (3, '3x4 decimetru', 'je na nÏm dÌra');

INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Truffaldino', 'velmi vysok·', 'muz', INTERVAL '60' DAY, ' vychytral˝ sluha, sv˝m zp˘sobem prosù·Ëek, kter˝ vypad·, ûe neumÌ do pÏti napoËÌtat, docela obratnÏ vöak vybruslÌ z kaûdÈ situace, kterou mu jeho dvojÌ sluûba p¯inese.');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Beatrice', 'vysok·', 'ûena', INTERVAL '40' DAY, 'p¯evleËen· za muûe vystupuje pod jmÈnem Frederigy Rasponi, je mazan·, moudr·, prolhan·, spravedliv·, ohledupln·');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Miroslav', 'Donutil', 65, '+420 543 211 593', ''); 

INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='d˘m'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p·n˘'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='koöile'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p·n˘'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='ö·tek'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p·n˘'));
INSERT INTO Seznam_roli VALUES (1, 1);
INSERT INTO Seznam_roli VALUES (2, 1);
INSERT INTO Obsazeni_inscenace VALUES (1, 1);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (1, 2);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (1, 3);
INSERT INTO Naucene_role VALUES (1, 1);
