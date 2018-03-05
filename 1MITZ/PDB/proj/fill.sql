-- prostorove typy
INSERT INTO Typ (id, jmeno, barva) VALUES (1, 'Stromy', '1A5422');
INSERT INTO Typ (id, jmeno, barva) VALUES (2, 'Trávník', '3DB443');
INSERT INTO Typ (id, jmeno, barva) VALUES (3, 'Cesta', '566063');
INSERT INTO Typ (id, jmeno, barva) VALUES (4, 'Voda', '4494F2');
INSERT INTO Typ (id, jmeno, barva) VALUES (5, 'Kámen', '98B4BD');
INSERT INTO Typ (id, jmeno, barva) VALUES (6, 'Budova', 'C96148');

--Script for filling the DB
--Druh zirete
INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('1', 'Gorila', 'Hominidae', 'Afrika', 'Gorila (Gorilla) je rod největších žijících primátů. Žijí na zemi a jsou převážně býložravé. Obývají lesy střední Afriky. Gorily se dělí na dva druhy a čtyři poddruhy (podle některých biologů pět poddruhů). DNA gorily je z 98 až 99 % shodná s DNA člověka [1] a po šimpanzích jsou gorily nejbližší příbuzní člověka.

Gorily obývají tropické a subtropické lesy. Ačkoliv se jejich výskyt omezuje jen na několik procent rozlohy Afriky, jejich biotopy pokrývají velký rozsah nadmořských výšek. Gorila východní (horská) žije v pohoří Virunga ve výškách 2225 až 4267 m nad mořem. Gorila západní (nížinná) žije v hustých lesích a mokřinách i v úrovni mořské hladiny.');

INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('2', 'Tygr', 'Felidae', 'Asie', 'Tygři jsou obvykle samotáři. Jejich hlavní kořistí jsou velcí sudokopytníci. Obývají různé biotopy, od tropických lesů, přes stepi a mokřady až k severským tundrám. Původně areál rozšíření sahal od Kavkazu a východního Turecka do středoasijských stepí, od indického subkontinentu přes Zadní Indii a východní Čínu na ruský dálný východ. Areál také zahrnoval indonéské ostrovy Sumatra, Jáva a Bali.');

INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('3', 'Lev', 'Felidae', 'Afrika', 'Lev pustinný (Panthera leo) je savec čeledi kočkovitých a jeden ze čtyř druhů velkých koček rodu Panthera. Lev je po tygrovi druhá největší kočkovitá šelma. Samci váží 150–250 kg a samice 90–165 kg. V divočině se lvi dožívají 10–14 let, kdežto v zajetí se mohou dožít i věku 20 let. Dříve se lvi vyskytovali v celé Africe, Asii, Evropě a dokonce i v Americe, dnes se vyskytují pouze v Africe a na několika místech v Indii. Jsou to společenská zvířata a loví ve smečkách. U lvů se projevuje výrazný pohlavní dimorfismus, hlavním a určujícím rysem lvích samců je jejich hříva.');

INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('4', 'Slon Africký', 'Elephantidae', 'Afrika', 'Slon je souhrnný český název pro dva rody chobotnatců - Loxodonta a Elephas. Slon je největší žijící suchozemský savec. Při narození váží okolo 100 kg. Samice slona je březí 20 až 22 měsíců, což je nejdelší doba březosti u suchozemského zvířete. Slon se dožívá 60 až 70 let. Největšího slona zastřelili v Angole v roce 1974, vážil 12 000 kilogramů.');

INSERT INTO druhZvirete (id, jmenoDruhu, celed, oblast, info ) VALUES('5', 'Lama', 'Camelidae)', 'Jižní Amerika', 'Lamy, známé též jako velbloudi Jižní Ameriky, jsou zvířata z čeledi velbloudovití, která se přizpůsobila těžkému životu v Andách. V posledních 40 letech se šíří do celého světa jako chovný dobytek či zvěř chovaná v zoologických zahradách.');

--Zvire
INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('1', 'Maruška', '2', 'samice', TO_DATE('1993/12/22', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/01', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('2', 'Lucka', '2', 'samice', TO_DATE('2000/09/15', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/30', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('3', 'Radek', '2', 'samec', TO_DATE('1997/04/20', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/02/01', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('4', 'Iva', '1', 'samice', TO_DATE('1966/05/14', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/03', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('5', 'Maruška', '1', 'samice', TO_DATE('1967/05/14', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/03', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('6', 'Petr', '1', 'samec', TO_DATE('1966/07/18', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/10', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('7', 'Tomáš', '1', 'samec', TO_DATE('1967/03/18', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/10', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('8', 'Andrea', '3', 'samice', TO_DATE('1986/06/15', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/11', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('9', 'Daniela', '3', 'samice', TO_DATE('1977/06/12', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/02/11', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('10', 'Ladík', '3', 'samec', TO_DATE('1977/09/20', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/02/11', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('11', 'Berta', '4', 'samice', TO_DATE('1955/06/22', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/13', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('12', 'Blažena', '5', 'samice', TO_DATE('1970/06/16', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/29', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('13', 'Tereza', '5', 'samice', TO_DATE('1987/04/16', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/28', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('14', 'Igor', '5', 'samec', TO_DATE('1990/09/15', 'yyyy/mm/dd'), NULL,  TO_DATE('2017/01/23', 'yyyy/mm/dd'), ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('15', 'Lukáš', '5', 'samec', TO_DATE('1999/07/18', 'yyyy/mm/dd'), TO_DATE('2005/06/08', 'yyyy/mm/dd'),  null, ORDSYS.ORDIMAGE.init());

INSERT INTO zvire (id , jmeno, idDruhu, pohlavi, datumNarozeni, datumUmrti, datumPorizeni, fotka) VALUES('16', 'Zdena', '1', 'samice', TO_DATE('1920/10/22', 'yyyy/mm/dd'), TO_DATE('2000/03/10', 'yyyy/mm/dd'),  null, ORDSYS.ORDIMAGE.init());

--Vybeh
INSERT INTO vybeh (id, jmeno, info) VALUES('1', 'Pavilon goril', 'Výstavba pavilonu začala v roce 2005 a byla dokončena v roce 2007....');

INSERT INTO vybeh (id, jmeno, info) VALUES('2', 'Pavilon koček', 'Výstavba pavilonu začala v roce 2000 a byla dokončena v roce 2002....');

INSERT INTO vybeh (id, jmeno, info) VALUES('3', 'Pavilon slonů', 'Výstavba pavilonu začala v roce 1990 a byla dokončena v roce 1991....');

INSERT INTO vybeh (id, jmeno, info) VALUES('4', 'Výběh lam', 'Výstavba nejnovějíšího přírůstki naší zoo začala v roce 2015 a skočila v roce 2016');

INSERT INTO vybeh (id, jmeno, info) VALUES('5', 'Karanténa', 'Všechny zvířata co jsou přemístěna do Zoo, nebo se v ní narodí musejí projít vstupní prohlídkou v karanténě');

INSERT INTO vybeh (id, jmeno, info) VALUES('6', 'Ošetřovna', 'Pokuď je zvíře zraněné nebo nemocné, tak je dočasně umístěno na ošetřovně');

--Umisteni
--1. pavilon
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('1', '1', '4', TO_TIMESTAMP('1966/05/15 10:00:11', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('2', '1', '5',TO_TIMESTAMP('1967/05/20 09:00:00', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('3', '1', '6',TO_TIMESTAMP('1966/07/18 12:00:30', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2016/09/01 12:00:00', 'yyyy/mm/dd hh24:mi:ss'));

--2.pavilon
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('4', '2', '1',TO_TIMESTAMP('1993/12/23 10:00:00', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('5', '2', '2',TO_TIMESTAMP('2000/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('6', '2', '3',TO_TIMESTAMP('1997/04/21 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

--3.pavilon
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('7', '3', '7',TO_TIMESTAMP('1967/03/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('8', '3', '8',TO_TIMESTAMP('1986/06/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('9', '3', '9',TO_TIMESTAMP('1977/06/13 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('10', '4', '10',TO_TIMESTAMP('1967/03/20 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

--4.pavilon
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('11', '4', '11',TO_TIMESTAMP('1955/06/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('12', '4', '12',TO_TIMESTAMP('1970/06/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('13', '4', '13',TO_TIMESTAMP('1987/04/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('14', '4', '14',TO_TIMESTAMP('1990/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('15', '4', '15',TO_TIMESTAMP('1999/07/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), null);

--Umístění v karanténě5.pavilon
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('16', '5', '4', TO_TIMESTAMP('1966/05/14 01:00:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1966/05/15 10:00:11', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('17', '5', '5', TO_TIMESTAMP('1967/05/19 10:01:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1967/05/20 09:00:00', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('18', '5', '6', TO_TIMESTAMP('1966/07/17 15:00:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1966/07/18 12:00:30', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('19', '5', '1',TO_TIMESTAMP('1993/12/22 02:10:00', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1993/12/23 10:00:00', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('20', '5', '2',TO_TIMESTAMP('2000/09/15 05:15:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2000/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('21', '5', '3',TO_TIMESTAMP('1997/04/20 13:12:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1997/04/21 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('22', '5', '7',TO_TIMESTAMP('1967/03/18 20:12:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1967/03/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('23', '5', '8',TO_TIMESTAMP('1986/06/15 18:18:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1986/06/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('24', '5', '9',TO_TIMESTAMP('1977/06/12 19:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1977/06/13 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('25', '5', '10',TO_TIMESTAMP('1967/03/19 15:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1967/03/20 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('26', '5', '11',TO_TIMESTAMP('1955/06/22 20:51:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1955/06/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('27', '5', '12',TO_TIMESTAMP('1970/06/16 18:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1970/06/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('28', '5', '13',TO_TIMESTAMP('1987/04/16 01:10:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1987/04/17 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('29', '5', '14',TO_TIMESTAMP('1990/09/15 14:15:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1990/09/16 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('30', '5', '15',TO_TIMESTAMP('1999/07/18 19:01:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1999/07/19 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('31', '5', '16',TO_TIMESTAMP('1920/10/22 10:01:40', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('1920/10/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'));

--Ošetřovna (je nutné ukončit původní interval) 6. pavilon
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('32', '6', '6',TO_TIMESTAMP('2016/09/01 12:00:00', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2016/09/05 12:00:00', 'yyyy/mm/dd hh24:mi:ss'));

INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('33', '1', '6',TO_TIMESTAMP('2016/09/05 12:00:00', 'yyyy/mm/dd hh24:mi:ss'), null);

--Dead gorilla
INSERT INTO umisteni (id, idvybehu, idZvirete, umisteniOd, umisteniDo) VALUES('34', '1', '16',TO_TIMESTAMP('1920/10/23 12:00:20', 'yyyy/mm/dd hh24:mi:ss'), TO_TIMESTAMP('2000/03/10 23:36:00', 'yyyy/mm/dd hh24:mi:ss'));

-- * v�bhy *

-- sloni
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (2,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
0.0,0.0,
25.0,0.0,
25.0,45.0,
0.0,45.0,
0.0,0.0)),3,2);

-- ko�ky
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (3,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
35.0,10.0,
55.0,30.0,
55.0,45.0,
35.0,45.0,
35.0,10.0)),2,2);

-- gorily
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (4,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
50.0,0.0,
100.0,0.0,
100.0,20.0,
70.0,20.0,
50.0,0.0)),1,2);

-- lamy
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (5,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
70.0,30.0,
100.0,30.0,
100.0,70.0,
70.0,70.0,
70.0,30.0)),4,2);


-- * budovy *

-- oetovna
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (6,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
60.0,80.0,
80.0,80.0,
80.0,100.0,
60.0,100.0,
60.0,80.0)),5,6);

-- karantna
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (7,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
80.0,80.0,
100.0,80.0,
100.0,100.0,
80.0,100.0,
80.0,80.0)),6,6);

-- * ostatn� *
-- voda
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (8,SDO_GEOMETRY(2003,null,null,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(
0.0,80.0,
50.0,80.0,
50.0,100.0,
0.0,100.0,
0.0,80.0)),null,4);

-- stromy u cesty
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (9,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(5,52.5,7.5,55.0,5,57.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (10,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(15,52.5,17.5,55.0,15,57.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (11,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(25,52.5,27.5,55.0,25,57.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (12,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(35,52.5,37.5,55.0,35,57.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (13,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(45,52.5,47.5,55.0,45,57.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (14,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(55,52.5,57.5,55.0,55,57.5)),null,1);

-- stromy u slonu
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (15,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(10,27.5,12.5,30.0,10,32.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (16,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(20,37.5,22.5,40.0,20,42.5)),null,1);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (17,SDO_GEOMETRY(2003, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1003,4),SDO_ORDINATE_ARRAY(20,7.5,22.5,10.0,20,12.5)),null,1);
      
-- * cesty *
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (18,SDO_GEOMETRY(2002,null,null,
SDO_ELEM_INFO_ARRAY(1,2,1),
SDO_ORDINATE_ARRAY(
0.0,50.0,30.0,50.0,
30.0,5.0,40.0,5.0,
60.0,25.0,100.0,
25.0
)),null,3);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (19,SDO_GEOMETRY(2002,null,null,
SDO_ELEM_INFO_ARRAY(1,2,1),
SDO_ORDINATE_ARRAY(
0.0,75.0,60.0,75.0,
60.0,25.0,100.0,25.0
)),null,3);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (20,SDO_GEOMETRY(2002,null,null,
SDO_ELEM_INFO_ARRAY(1,2,1),
SDO_ORDINATE_ARRAY(
0.0,75.0,70.0,75.0,
70.0,80.0
)),null,3);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (21,SDO_GEOMETRY(2002,null,null,
SDO_ELEM_INFO_ARRAY(1,2,1),
SDO_ORDINATE_ARRAY(
0.0,75.0,90.0,75.0,
90.0,80.0
)),null,3);

-- kameny
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (22,SDO_GEOMETRY(2005, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1,2),
SDO_ORDINATE_ARRAY(
10.0,70.0,
20.0,70.0
)),null,5);
INSERT INTO Zoo (id, prostorovaData, idVybehu, idTyp) VALUES (23,SDO_GEOMETRY(2005, NULL, NULL,
SDO_ELEM_INFO_ARRAY(1,1,2),
SDO_ORDINATE_ARRAY(
40.0,70.0,
50.0,70.0
)),null,5);

COMMIT