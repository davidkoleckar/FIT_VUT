--PDB Autobazar 
--David Kolečkář xkolec07
--Hynek Bočán xbocan00
--Petr Navrátil xnavra53
--------------------------

DROP TABLE uzivatele;
DROP TABLE vozidla;
DROP TABLE kategorie;
DROP TABLE areal;
DROP TABLE obrazky;
DROP SEQUENCE  vehicle_seq;
DROP PROCEDURE rotate;
DROP INDEX mapa_index;

-- SEQUENCE
CREATE SEQUENCE  vehicle_seq minvalue 1 start with 1 increment by 1 nocache;

COMMIT;

CREATE TABLE uzivatele(
  id NUMBER GENERATED ALWAYS as IDENTITY(START with 1 INCREMENT by 1),
  email VARCHAR(32) NOT NULL,
  jmeno VARCHAR(32) NOT NULL,
  adresa VARCHAR(256) NOT NULL,
  heslo VARCHAR(32) NOT NULL,
  CONSTRAINT pk_uzivatele PRIMARY KEY (id)
);

CREATE TABLE kategorie(
  id NUMBER GENERATED ALWAYS as IDENTITY(START with 1 INCREMENT by 1),
  nazev VARCHAR(32) NOT NULL,
  CONSTRAINT pk_kategorie PRIMARY KEY (id)
);

CREATE TABLE areal(
  id NUMBER GENERATED ALWAYS as IDENTITY(START with 1 INCREMENT by 1),
  nazev VARCHAR(32),
  typ VARCHAR(22) CHECK( typ IN ('areal','budova','parkoviste','cesta', 'sekce')),
  typ_entity VARCHAR(22) CHECK( typ_entity IN ('kruh', 'ctverec', 'trojuhelnik', 'elko', 'oval')),
  tvar SDO_GEOMETRY,
  CONSTRAINT pk_areal PRIMARY KEY (id)
);

CREATE TABLE obrazky(
  id NUMBER GENERATED ALWAYS as IDENTITY(START with 1 INCREMENT by 1),
  photo ORDSYS.ORDIMAGE,
  photo_si ORDSYS.SI_STILLIMAGE,
  photo_ac ORDSYS.SI_AVERAGECOLOR,
  photo_ch ORDSYS.SI_COLORHISTOGRAM,
  photo_pc ORDSYS.SI_POSITIONALCOLOR,
  photo_tx ORDSYS.SI_TEXTURE,
  CONSTRAINT pk_obrazky PRIMARY KEY (id)
);

CREATE TABLE vozidla(
  id NUMBER,
  nakup DATE,
  prodej DATE,
  znacka VARCHAR(32) NOT NULL,
  model VARCHAR(32),
  motor VARCHAR(32) NOT NULL,
  rok NUMBER NOT NULL,
  stk DATE,
  kupni_cena NUMBER NOT NULL,
  cena NUMBER NOT NULL,
  kilometry NUMBER NOT NULL,
  stav VARCHAR(255),
  kategorie NUMBER NOT NULL,
  umisteni NUMBER,
  valid_from DATE,
  valid_to DATE,
  user_id NUMBER,
  obrazek NUMBER,
  CONSTRAINT pk_vozidla PRIMARY KEY (id, valid_from),
  CONSTRAINT fk_kategorie FOREIGN KEY (kategorie) REFERENCES kategorie(id) ON DELETE CASCADE,
  CONSTRAINT fk_areal FOREIGN KEY (umisteni) REFERENCES areal(id) ON DELETE SET NULL,
  CONSTRAINT fk_obrazek FOREIGN KEY (obrazek) REFERENCES obrazky(id) ON DELETE CASCADE
);

CREATE INDEX mapa_index on areal(tvar) INDEXTYPE IS MDSYS.SPATIAL_INDEX;

COMMIT;

--INSERT DATA--------------

--UZIVATELE
INSERT INTO uzivatele (email, jmeno, adresa, heslo) VALUES ('xkolec07@fit.vutbr.cz', 'Kolec', 'Skoupilova 37', 'oracle');
INSERT INTO uzivatele (email, jmeno, adresa, heslo) VALUES ('xbocan00@fit.vutbr.cz', 'Bocan', 'Jarose 88', 'oracle');
INSERT INTO uzivatele (email, jmeno, adresa, heslo) VALUES ('xnavra53@fit.vutbr.cz', 'Navratil', 'Brno 10', 'oracle');


--KATEGORIE
INSERT INTO kategorie (nazev) VALUES ('Osobní');
INSERT INTO kategorie (nazev) VALUES ('Motocykly');
INSERT INTO kategorie (nazev) VALUES ('Nákladní');

INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Sekce 1', 'sekce', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(54,51,269,58,259,226,85,227,54,51)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Sekce 2', 'sekce', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(387,68,557,68,555,301,416,285,387,68)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Budova 1', 'budova', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(39,297,116,253,231,253,309,253,385,297,352,343,284,376,187,444,74,399,32,335,39,297)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Parkoviste 1', 'parkoviste', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(81,74,131,74,131,124,81,124,81,74)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Parkoviste 2', 'parkoviste', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(173,75,223,75,223,125,173,125,173,75)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Parkoviste 3', 'parkoviste', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(407,81,457,81,457,131,407,131,407,81)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Parkoviste 4', 'parkoviste', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(418,157,468,157,468,267,418,207,418,157)),'ctverec');
INSERT INTO areal (nazev, typ, tvar, typ_entity) VALUES('Parkoviste 5', 'parkoviste', SDO_GEOMETRY(2003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1,1003,1),SDO_ORDINATE_ARRAY(481,82,531,82,531,132,481,132,481,82)),'ctverec');
--VOZIDLA
INSERT INTO vozidla (id, nakup, znacka, model, motor, rok, stk, cena, kupni_cena, kilometry, stav, kategorie,user_id,valid_from, umisteni)
VALUES (vehicle_seq.nextval, TO_DATE ('2014-06-21', 'yyyy-mm-dd'), 'Audi', 'A5', 'TDI 2.0', 2008, TO_DATE('2017-12-31', 'yyyy-mm-dd'), 250000, 200000, 110000, 'Odřený lak', 1, 1, TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss'), 4);
INSERT INTO vozidla (id, nakup, znacka, model, motor, rok, stk, cena, kupni_cena, kilometry, stav, kategorie,user_id,valid_from, umisteni)
VALUES (vehicle_seq.nextval, TO_DATE('2016-12-31', 'yyyy-mm-dd'), 'BMW', 'M3', '320d', 2009, TO_DATE('2018-06-30', 'yyyy-mm-dd'), 200000, 90000, 190000, 'Nefunkční rádio', 1, 2, TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss'),5);
INSERT INTO vozidla (id, nakup, znacka, model, motor, rok, stk, cena, kupni_cena, kilometry, stav, kategorie,user_id,valid_from, umisteni)
VALUES (vehicle_seq.nextval, TO_DATE('2015-02-03', 'yyyy-mm-dd'), 'Yamaha', 'FJR13', 'Y125', 2014, TO_DATE('2018-03-31', 'yyyy-mm-dd'), 100000, 80000, 25000, '', 2, 1, TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss'),6);
INSERT INTO vozidla (id, nakup, znacka, model, motor, rok, stk, cena, kupni_cena, kilometry, stav, kategorie,user_id,valid_from, umisteni)
VALUES (vehicle_seq.nextval, TO_DATE('2015-12-31', 'yyyy-mm-dd'), 'Scania', 'R450', 'DC13 450', 2010, TO_DATE('2017-11-30', 'yyyy-mm-dd'), 550000, 450000, 420000, 'Nízký vzorek pneumatik', 3, 1, TO_DATE(TO_CHAR(CURRENT_DATE, 'yyyy-mm-dd HH24:MI:ss'), 'yyyy-mm-dd HH24:MI:ss'),7);

--AREAL

--Smazani metadat o velikosti arealu
DELETE FROM USER_SDO_GEOM_METADATA WHERE
	TABLE_NAME = 'AREAL' AND COLUMN_NAME = 'TVAR';

--Inicializace arealu na nejakou velikost s urcitou presnosti

INSERT INTO USER_SDO_GEOM_METADATA VALUES (
	'areal', 'tvar',
	SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', 0, 700, 0.1), SDO_DIM_ELEMENT('Y', 0, 500, 0.1)),
	NULL
);

COMMIT;

