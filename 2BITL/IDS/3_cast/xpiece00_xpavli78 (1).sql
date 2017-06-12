-- Authors: Jan Pavlica - loxin:xpavli78, Adam Piecek - login: xpiece00
SET LINESIZE 250

DROP TABLE Seznam_rekvizit_pro_hru;
DROP TABLE Seznam_roli;
DROP TABLE Prestavky;
DROP TABLE Obsazeni_inscenace;
DROP TABLE Seznam_rekvizit_pro_roli;
DROP TABLE Naucene_role;
DROP TABLE Inscenace;
DROP TABLE Hry;
DROP TABLE Jednotlive_role;
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
Jmeno VARCHAR(50),
Zanr VARCHAR(10),
Popis VARCHAR(350),

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
IdHry INTEGER NOT NULL,
Datum_a_cas TIMESTAMP,
Cena_za_predstaveni INTEGER,

CONSTRAINT pk_inscenace PRIMARY KEY (IdInscenace),
CONSTRAINT fk_poradatele FOREIGN KEY (IcPoradatele) REFERENCES Poradatele(IcPoradatele),
CONSTRAINT fk_hry FOREIGN KEY (IdHry) REFERENCES Hry(IdHry)
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
INSERT INTO Autori VALUES (seq_id_autora.nextval, 'Pablo', 'Maraka', TO_DATE('03.12.1906', 'dd.mm.yyyy'), TO_DATE('16.01.1948', 'dd.mm.yyyy'), '�pan�lsko');
INSERT INTO Autori VALUES (seq_id_autora.nextval, 'Jamal', 'Edwards', TO_DATE('06.08.1984', 'dd.mm.yyyy'), NULL, 'Spojen� St�ty Americk�');
INSERT INTO Autori VALUES (seq_id_autora.nextval, 'Rudolf', 'Ka�ka', TO_DATE('17.11.1973', 'dd.mm.yyyy'), NULL, '�esk� Republika');
INSERT INTO Autori VALUES (seq_id_autora.nextval, 'Tony', 'Duong', TO_DATE('04.06.1993', 'dd.mm.yyyy'), NULL, 'Vietnam');
INSERT INTO Autori VALUES (seq_id_autora.nextval, 'Jan', 'Kamz�k', TO_DATE('08.06.1894', 'dd.mm.yyyy'), TO_DATE('18.07.1923', 'dd.mm.yyyy'), '�esk� Republika');

INSERT INTO Reziseri VALUES (seq_id_rezisera.nextval, 'Ivan', 'Rajmont', '+420 736 652 313');
INSERT INTO Reziseri VALUES (seq_id_rezisera.nextval, 'Michal', 'Kr�je�', '+420 731 431 882');
INSERT INTO Reziseri VALUES (seq_id_rezisera.nextval, 'Pavel', 'Gold', '+420 605 645 852');
INSERT INTO Reziseri VALUES (seq_id_rezisera.nextval, 'Dardan', 'Klos', '+420 773 211 706');

INSERT INTO Poradatele VALUES (seq_ic_poradatele.nextval, 'Divadeln� spole�nost Josefa Dvo��ka', '�itom��sk� 46', 'Praha 10', 10100, '+420 602 236 892', 'Dobr� zku�enosti'); 
INSERT INTO Poradatele VALUES (seq_ic_poradatele.nextval, 'Karel a jeho kumpani', 'U Potoka 8', '�idenice', 60423, '+420 777 456 357', 'Hor�� komunikace');
INSERT INTO Poradatele VALUES (seq_ic_poradatele.nextval, 'Voj��ci z molitanu', 'Eli�ky Kr�snohorsk�', 'Praha 8', 10101, '+420 605 432 432', 'Dobr� zku�enosti');

INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Godolni'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Rajmont'), 'Sluha dvou p�n�', 'Komedie', 'italsk� komedie v obdob� klasicismu (Commedia dell�arte)');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Godolni'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Gold'), 'Kolos', 'Satira', 'satira, kv�li n� byl vylou�en z koleje v Pavii');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Maraka'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Gold'), 'Kalu� pln� �ab', 'Komedie', 'surrealistick� d�lo ze �ivota �ab');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Maraka'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Kr�je�'), 'Had� louka', 'Drama', 'drama, odehr�vaj�c� se na louce v klubku pln�m had�');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Maraka'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Kr�je�'), 'Datel, kter� kloval do vos�ho hn�zda', 'Drama', 'drama z lesa');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Edwards'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Gold'), 'Modern� popeln�k', 'Komedie', 'komedie z ku��ck� rodiny');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Edwards'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Klos'), 'Katapultem k moj� d�vce', 'Drama', 'drama o osudov� l�sce, kde mlad�k tvrd� narazil');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Edwards'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Klos'), 'Jak m� �kr�bali ze zdi', 'Drama', 'surov� drama o t�k�m �ivot� a jeho konci');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Ka�ka'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Gold'), 'P�se� od M�i', 'Komedie', 'komedie zalo�ena na z�m�n� postav');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Ka�ka'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Rajmont'), 'Tenhle hn�j teda f�j', 'Komedie', 'komedie z �esk�ho venkova');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Ka�ka'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Klos'), 'Halal aneb zabija�ka', 'Drama', 'drama popisuj�c� odli�nosti kultur');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Duong'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Rajmont'), 'Samuraj, kter� byl t�et� od konce', 'Komedie', 'p��b�h o skoro posledn�m samurajovi');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Duong'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Kr�je�'), 'Me� od krve', 'Drama', 'st�edov�k� drama odehr�vaj�c� se ve vietnamsk� vesnici');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Kamz�k'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Kr�je�'), 'Mimochodem', 'Satira', 'satirick� d�lo, kritizuj�c� sou�asnou spole�nost');
INSERT INTO Hry VALUES (seq_id_hry.nextval, (SELECT IdAutora from Autori WHERE Prijmeni='Kamz�k'), (SELECT IdRezisera from Reziseri WHERE Prijmeni='Gold'), 'Snapchat a jin� vymo�enosti', 'Satira', 'kritika mlad�ch �en d�vaj�c�h se na odiv');

INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='Divadeln� spole�nost Josefa Dvo��ka'), (SELECT idHry from Hry WHERE Jmeno='Sluha dvou p�n�'), TO_DATE('22.5.2016 20:00', 'dd.mm.yyyy HH24:MI'), 300);
INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='Divadeln� spole�nost Josefa Dvo��ka'), (SELECT idHry from Hry WHERE Jmeno='Had� louka'), TO_DATE('22.5.2016 20:00', 'dd.mm.yyyy HH24:MI'), 300);
INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='Karel a jeho kumpani'), (SELECT idHry from Hry WHERE Jmeno='Sluha dvou p�n�'), TO_DATE('7.5.2016 20:00', 'dd.mm.yyyy HH24:MI'), 350);
INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='Karel a jeho kumpani'), (SELECT idHry from Hry WHERE Jmeno='Kolos'), TO_DATE('8.5.2016 20:00', 'dd.mm.yyyy HH24:MI'), 350);
INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='Voj��ci z molitanu'), (SELECT idHry from Hry WHERE Jmeno='Modern� popeln�k'), TO_DATE('4.1.2015 19:00', 'dd.mm.yyyy HH24:MI'), 249);
INSERT INTO Inscenace VALUES (seq_id_inscenace.nextval, (SELECT icPoradatele from Poradatele WHERE Jmeno='Voj��ci z molitanu'), (SELECT idHry from Hry WHERE Jmeno='Jak m� �kr�bali ze zdi'), TO_DATE('5.1.2015 18:00', 'dd.mm.yyyy HH24:MI'), 249);

INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 1, TO_DATE('21:00', 'HH24:MI'), TO_DATE('21:10', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 1, TO_DATE('22:00', 'HH24:MI'), TO_DATE('22:10', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 2, TO_DATE('21:20', 'HH24:MI'), TO_DATE('21:30', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 2, TO_DATE('22:20', 'HH24:MI'), TO_DATE('22:30', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 3, TO_DATE('21:20', 'HH24:MI'), TO_DATE('21:30', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 3, TO_DATE('22:20', 'HH24:MI'), TO_DATE('22:30', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 4, TO_DATE('20:45', 'HH24:MI'), TO_DATE('21:00', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 4, TO_DATE('21:45', 'HH24:MI'), TO_DATE('22:00', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 5, TO_DATE('20:00', 'HH24:MI'), TO_DATE('20:15', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 5, TO_DATE('21:10', 'HH24:MI'), TO_DATE('21:20', 'HH24:MI'));
INSERT INTO Prestavky VALUES (seq_id_prestavky.nextval, 6, TO_DATE('19:20', 'HH24:MI'), TO_DATE('19:35', 'HH24:MI'));

INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'd�m', 'hn�d�, vypad� sta�e', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'auto', 'modr�, hlin�kov� konstrukce', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'zadn� st�na', '�lut� nabarven�, vvhcodov� dve�e vlevo', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'ko�ile', 'pruhovan�', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'kalhoty', '�ern�', 3);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'kab�t', 'hn�d�, za�l�', 2);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, '��tek', 'zmuchlan�, modr� barvy', 1);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'prsten', 'zlat�', 12);
INSERT INTO Rekvizity VALUES (seq_id_rekvizity.nextval, 'ocas', '�ern�, tlust�, �ertovsk�', 3);

INSERT INTO Kulisy VALUES (1, '10x4 metru, 4x4 po slozeni', 'Interier');
INSERT INTO Kulisy VALUES (2, '3,5 x 1,8 x 1,8', 'Exterier');
INSERT INTO Kulisy VALUES (3, '8x2 metru', 'Interier');

INSERT INTO Kostymy VALUES (4, 'L', '18 stoleti Francie');
INSERT INTO Kostymy VALUES (5, '42', 'univerz�ln�');
INSERT INTO Kostymy VALUES (6, 'L-XL', 'pro o�u�tel� vzhled (�ebr�k)');

INSERT INTO Doplnky VALUES (7, '3x4 decimetru', 'je na n�m d�ra');
INSERT INTO Doplnky VALUES (8, '32-33', 'd�msk�');
INSERT INTO Doplnky VALUES (9, '30cm-60cm', 'do pekla');


INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Truffaldino', 'velmi vysok�', 'muz', INTERVAL '3 12:00:00' DAY TO SECOND, ' vychytral� sluha, sv�m zp�sobem pros���ek, kter� vypad�, �e neum� do p�ti napo��tat, docela obratn� v�ak vybrusl� z ka�d� situace, kterou mu jeho dvoj� slu�ba p�inese.');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Hugo', 'vysok�', 'mu�', INTERVAL '2 6:00:00' DAY TO SECOND, 'mal�, podl�, sk��tek, kter� je velice vypo��tav�');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Beatrice', 'vysok�', '�ena', INTERVAL '1 18:00:00' DAY TO SECOND, 'p�evle�en� za mu�e vystupuje pod jm�nem Frederigy Rasponi, je mazan�, moudr�, prolhan�, spravedliv�, ohledupln�');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Hubert', 'vysok�', 'mu�', INTERVAL '2 13:00:00' DAY TO SECOND, 'cestuje s�m po sv�t�, neboj�cn�, p�idrzl�, my�len�m pomalej��, kokt�');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Martha', 'st�edn�', '�ena', INTERVAL '2 16:00:00' DAY TO SECOND, 'dominantn� �ena, je� zcela kontroluje sv�ho man�ela, manipulativn�, koketa');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Alex', 'velmi vysok�', 'mu�', INTERVAL '5 18:00:00' DAY TO SECOND, 'schyzofrenn� mu�, kter� bojuje s vnit�n�mi d�mony, nest�l�');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Snake', 'n�zk�', 'mu�', INTERVAL '1 00:00:00' DAY TO SECOND, 'slizsk� pomocn�k, Huga');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Frank', 'st�edn�', 'mu�', INTERVAL '2 17:00:00' DAY TO SECOND, 'm�stn� o�rala v hospod�, v��n� opil�');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Piqik', 'velmi vysok�', '�ena', INTERVAL '7 15:00:00' DAY TO SECOND, 'ne�sp�n� zp�va�ak, kter� se sna�� prosadit v zahrani��');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Tiny', 'n�zk�', 'mu�', INTERVAL '1 15:00:00' DAY TO SECOND, 'mu� cel� z kamene, kter� postupem �asu roste z kam�nku ve sk�lu, moudr�, osamocen�');
INSERT INTO Jednotlive_role VALUES (seq_id_role.nextval, 'Sir Delas Parnak', 'vysok�', 'mu�', INTERVAL '3 15:00:00' DAY TO SECOND, 'state�n� ryt��, kter� jedn� �ist� sobecky, n�ladov�');

INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Miroslav', 'Donutil', 65, '+420 543 211 593', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Jaroslav', 'Kom�r', 43, '+420 763 211 453', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Luk�', 'Dominik', 27, '+420 769 706 211', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Marek', 'Rus�k', 36, '+420 789 566 503', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Marie', 'Hor�kov�', 34, '+420 737 651 503', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Jana', 'Kov��ov�', 24, '+420 605 603 601', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Karel', 'Navr�til', 22, '+420 739 979 663', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Petra', 'Karasov�', 34, '+420 615 604 223', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Jan', 'N�plava', 38, '+420 798 655 592', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Marek', 'Mlad�', 45, '+420 777 561 693', '');
INSERT INTO Herci VALUES (seq_id_herce.nextval, 'Karol�na', '�ako', 49, '+420 763 211 593', '');

INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='d�m'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p�n�'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='zadn� st�na'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p�n�'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='ko�ile'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p�n�'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='prsten'), (SELECT IdHry from Hry WHERE Jmeno='Sluha dvou p�n�'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='��tek'), (SELECT IdHry from Hry WHERE Jmeno='Samuraj, kter� byl t�et� od konce'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='kab�t'), (SELECT IdHry from Hry WHERE Jmeno='Samuraj, kter� byl t�et� od konce'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='d�m'), (SELECT IdHry from Hry WHERE Jmeno='Modern� popeln�k'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='prsten'), (SELECT IdHry from Hry WHERE Jmeno='Samuraj, kter� byl t�et� od konce'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='kalhoty'), (SELECT IdHry from Hry WHERE Jmeno='Samuraj, kter� byl t�et� od konce'));
INSERT INTO Seznam_rekvizit_pro_hru VALUES ((SELECT IdRekvizity from Rekvizity WHERE Jmeno='auto'), (SELECT IdHry from Hry WHERE Jmeno='Modern� popeln�k'));

INSERT INTO Seznam_roli VALUES (1, 1);
INSERT INTO Seznam_roli VALUES (2, 12);
INSERT INTO Seznam_roli VALUES (3, 1);
INSERT INTO Seznam_roli VALUES (4, 12);
INSERT INTO Seznam_roli VALUES (5, 12);
INSERT INTO Seznam_roli VALUES (6, 12);
INSERT INTO Seznam_roli VALUES (7, 6);
INSERT INTO Seznam_roli VALUES (8, 6);
INSERT INTO Seznam_roli VALUES (9, 6);
INSERT INTO Seznam_roli VALUES (10, 12);
INSERT INTO Seznam_roli VALUES (11, 12);


INSERT INTO Obsazeni_inscenace VALUES (1, 1);
INSERT INTO Obsazeni_inscenace VALUES (1, 2);
INSERT INTO Obsazeni_inscenace VALUES (2, 3);
INSERT INTO Obsazeni_inscenace VALUES (2, 4);
INSERT INTO Obsazeni_inscenace VALUES (3, 5);
INSERT INTO Obsazeni_inscenace VALUES (3, 6);
INSERT INTO Obsazeni_inscenace VALUES (3, 7);
INSERT INTO Obsazeni_inscenace VALUES (4, 8);

INSERT INTO Seznam_rekvizit_pro_roli VALUES (1, 2);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (1, 6);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (2, 4);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (4, 7);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (6, 7);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (2, 6);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (1, 8);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (3, 7);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (6, 5);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (7, 4);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (4, 5);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (5, 8);
INSERT INTO Seznam_rekvizit_pro_roli VALUES (6, 9);

INSERT INTO Naucene_role VALUES (1, 1);
INSERT INTO Naucene_role VALUES (1, 2);
INSERT INTO Naucene_role VALUES (2, 3);
INSERT INTO Naucene_role VALUES (2, 6);
INSERT INTO Naucene_role VALUES (3, 6);
INSERT INTO Naucene_role VALUES (3, 7);
INSERT INTO Naucene_role VALUES (4, 10);
INSERT INTO Naucene_role VALUES (4, 2);
INSERT INTO Naucene_role VALUES (4, 11);
INSERT INTO Naucene_role VALUES (5, 2);
INSERT INTO Naucene_role VALUES (7, 3);
INSERT INTO Naucene_role VALUES (8, 5);
INSERT INTO Naucene_role VALUES (9, 6);
INSERT INTO Naucene_role VALUES (10, 7);
INSERT INTO Naucene_role VALUES (10, 8);

-- Spojeni dvou tabulek: Vypis vsech autor a jejich her
SELECT A.Jmeno, A.Prijmeni, H.JMENO AS Hra
FROM Autori A JOIN Hry H ON A.IDAUTORA=H.IDAUTORA;

-- Spojeni dvou tabulek: Vypis pro hru Sluha dvou panu jmeno role, obtiznost, casovou narocnost v hodinach
SELECT R.Jmeno AS JMENO_ROLE, R.Obtiznost AS Obt�nost, extract(day from 24*R.Casova_narocnost) AS CASOVA_NAROCNOST_H
FROM Hry JOIN SEZNAM_ROLI ON Hry.IDHRY=SEZNAM_ROLI.IDHRY 
         JOIN JEDNOTLIVE_ROLE R ON SEZNAM_ROLI.IDROLE=R.IDROLE;

-- Spojeni tri tabulek: Pro kazdou inscenaci vypis jmena hry a Poradatele
SELECT P.Jmeno AS JMENO_PORADATELE, TO_CHAR(I.DATUM_A_CAS, 'dd.mm.yyyy HH24:MI') AS DATUM_A_CAS, H.Jmeno AS JMENO_HRY, TO_CHAR(I.CENA_ZA_PREDSTAVENI, 'FM999,999,999,990U') AS CENA
FROM Poradatele P JOIN Inscenace I ON P.IcPoradatele=I.IcPoradatele JOIN Hry H ON I.IdHry=H.IdHry;

-- Dotaz s Group by: U kazde Hry vypis pocet rekvizit
SELECT H.Jmeno AS JMENO_ROLE, COUNT(SH.IDREKVIZITY) AS Po�et_rekvizit
FROM Hry H, SEZNAM_REKVIZIT_PRO_HRU SH
WHERE H.IDHRY=SH.IDHRY
Group by H.Jmeno;

-- Dotaz s Group by: Pr�m�rn� po�et nau�en�ch rol� na herce
SELECT ROUND(AVG(COUNT(NAUCENE_ROLE.IDHERCE)),3) AS PRUMERNY_POCET_NAUCENYCH_ROLI
FROM HERCI
  LEFT JOIN NAUCENE_ROLE ON HERCI.IDHERCE = NAUCENE_ROLE.IDROLE
GROUP BY HERCI.IDHERCE;
         
         
-- Pouzizi IN: Vypis vsech hercu, kteri maji naucene POUZE role s 'nizkou' obtiznosti                
SELECT H.JMENO, H.PRIJMENI, H.VEK, H.KONTAKT
FROM Herci H JOIN NAUCENE_ROLE NR ON H.IDHERCE=NR.IDHERCE JOIN JEDNOTLIVE_ROLE JR ON NR.IDROLE=JR.IDROLE
  WHERE H.IDHERCE NOT IN(
  SELECT H.IDHERCE
  FROM Herci H JOIN NAUCENE_ROLE NR ON H.IDHERCE=NR.IDHERCE JOIN JEDNOTLIVE_ROLE JR ON NR.IDROLE=JR.IDROLE
  WHERE JR.IDROLE=NR.IDROLE and JR.OBTIZNOST <> 'n�zk�'
);

-- Pouzizi EXISTS: Vypis hercu, kteri nemaji naucenou zadnou roli
SELECT JMENO, PRIJMENI, VEK, KONTAKT
FROM HERCI
WHERE NOT EXISTS (SELECT *
              FROM NAUCENE_ROLE
              WHERE HERCI.IDHERCE = NAUCENE_ROLE.IDHERCE);