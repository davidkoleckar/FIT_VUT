	 DROP TABLE umi;
	 DROP TABLE ma;
	 DROP TABLE dovoluje;
	 DROP TABLE ovlada;
	 DROP TABLE clen;
	 DROP TABLE jazyk;
	 DROP TABLE letenka;
	 DROP TABLE misto;
	 DROP TABLE let;
	 DROP TABLE gate;
	 DROP TABLE letadlo;
	 DROP TABLE terminal;
	 DROP TABLE trida;
	 DROP TABLE technik;
	 DROP TABLE typ_letadla;

	DROP SEQUENCE letadlo_id;
  
  DROP PROCEDURE vypis_letenky;
  
  DROP PROCEDURE oprava;

	CREATE TABLE technik(
		ID_technik char(10) NOT NULL,
		jmeno varchar(20) NOT NULL,
		prijmeni varchar(20) NOT NULL,
		rodne_cislo char(10) NOT NULL,

		CONSTRAINT PK_technik PRIMARY KEY(ID_technik),

		CHECK (MOD(rodne_cislo, 11) = 0)
	);

	CREATE TABLE typ_letadla
	(
		ID_typ char(10) NOT NULL,
		vyrobce varchar(20) NOT NULL,
		typ varchar(20) NOT NULL,

		CONSTRAINT PK_typ_letadla PRIMARY KEY(ID_typ)
	);

	CREATE TABLE letadlo
	(
		ID_letadlo varchar(10) NOT NULL,
		datum_vyroby DATE NOT NULL,
		datum_posledni_revize DATE,
		ID_technik char(10) NOT NULL,
		ID_typ char(10) NOT NULL,

		CONSTRAINT PK_letadlo PRIMARY KEY(ID_letadlo),
		CONSTRAINT FK_LO_typ FOREIGN KEY(ID_typ) REFERENCES typ_letadla ON DELETE CASCADE,
		CONSTRAINT FK_LO_technik FOREIGN KEY(ID_technik) REFERENCES technik ON DELETE CASCADE,

		
		CHECK (datum_vyroby < datum_posledni_revize)	
	);

	CREATE TABLE terminal
	(
		ID_terminal char(10) NOT NULL,
		nazev varchar(20) NOT NULL,

		CONSTRAINT PK_terminal PRIMARY KEY(ID_terminal)
	);

	CREATE TABLE gate
	(
		ID_gate char(10) NOT NULL,
		cislo INTEGER NOT NULL,
		ID_terminal char(10) NOT NULL,

		CONSTRAINT PK_gate PRIMARY KEY(ID_gate),
		CONSTRAINT FK_G_terminal FOREIGN KEY(ID_terminal) REFERENCES terminal ON DELETE CASCADE,

		CHECK (cislo >= 0)
	);

	CREATE TABLE dovoluje
	(
		ID_gate char(10) NOT NULL,
		ID_typ char(10) NOT NULL,

		CONSTRAINT PK_dovoluje PRIMARY KEY(ID_gate, ID_typ),
		CONSTRAINT FK_D_gate FOREIGN KEY(ID_gate) REFERENCES gate ON DELETE CASCADE,
		CONSTRAINT FK_D_typ FOREIGN KEY(ID_typ) REFERENCES typ_letadla ON DELETE CASCADE
	);

	CREATE TABLE let
	(
		ID_let char(10) NOT NULL,
		cas_odletu TIMESTAMP NOT NULL,
		cas_priletu TIMESTAMP NOT NULL,
    max_cestujicich INTEGER NOT NULL,
		ID_letadlo varchar(10) NOT NULL,
		ID_terminal char(10) NOT NULL,
		ID_gate char(10) NOT NULL,
    

		CONSTRAINT PK_let PRIMARY KEY(ID_let),
		CONSTRAINT FK_LT_letadlo FOREIGN KEY(ID_letadlo) REFERENCES letadlo ON DELETE CASCADE,
		CONSTRAINT FK_LT_terminal FOREIGN KEY(ID_terminal) REFERENCES terminal ON DELETE CASCADE,
		CONSTRAINT FK_LT_gate FOREIGN KEY(ID_gate) REFERENCES gate ON DELETE CASCADE,

		CHECK (cas_odletu < cas_priletu)
	);

	CREATE TABLE trida
	(
		ID_trida char(10) NOT NULL,
		nazev varchar(20) NOT NULL,

		CONSTRAINT PK_trida PRIMARY KEY(ID_trida),

		CHECK (nazev IN ('business', 'první', 'turistická'))
	);

	CREATE TABLE misto
	(
		ID_misto char(10) NOT NULL,
		cislo INTEGER NOT NULL,
		umisteni varchar(10) NOT NULL,
		ID_trida char(10) NOT NULL,
		ID_letadlo varchar(10) NOT NULL,

		CONSTRAINT PK_misto PRIMARY KEY(ID_misto),
		CONSTRAINT FK_MO_trida FOREIGN KEY(ID_trida) REFERENCES trida ON DELETE CASCADE,
		CONSTRAINT FK_MO_letadlo FOREIGN KEY(ID_letadlo) REFERENCES letadlo ON DELETE CASCADE,

		CHECK (cislo >= 0),
		CHECK (umisteni IN ('okýnko', 'uprostøed', 'ulièka'))
	);

	CREATE TABLE letenka
	(
		ID_letenka char(10) NOT NULL,
		cena INTEGER NOT NULL,
	    ID_let char(10) NOT NULL,
	    ID_trida char(10) NOT NULL,
	    ID_misto char(10) NOT NULL,

		CONSTRAINT PK_letenka PRIMARY KEY(ID_letenka),
		CONSTRAINT FK_LA_let FOREIGN KEY(ID_let) REFERENCES let ON DELETE CASCADE,
		CONSTRAINT FK_LA_trida FOREIGN KEY(ID_trida) REFERENCES trida ON DELETE CASCADE,
		CONSTRAINT FK_LA_misto FOREIGN KEY(ID_misto) REFERENCES misto ON DELETE CASCADE,

		CHECK (cena >= 0)
	);

	CREATE TABLE clen
	(
		ID_clen char(10) NOT NULL,
		jmeno varchar(20) NOT NULL,
		prijmeni varchar(20) NOT NULL,
		rodne_cislo char(10) NOT NULL,
		pohlavi varchar(5) NOT NULL,
		pocet_hodin INTEGER,
		datum_prezkouseni DATE,
		funkce varchar(20),

		CONSTRAINT PK_clen PRIMARY KEY(ID_clen),

		CHECK (MOD(rodne_cislo, 11) = 0),
		CHECK (pohlavi IN ('mu¾', '¾ena')),
		CHECK (pocet_hodin >= 0)
	);

	CREATE TABLE ma
	(
		ID_let char(10) NOT NULL,
		ID_clen char(10) NOT NULL,

		CONSTRAINT PK_ma PRIMARY KEY(ID_let, ID_clen),
		CONSTRAINT FK_MA_let FOREIGN KEY(ID_let) REFERENCES let ON DELETE CASCADE,
		CONSTRAINT FK_MA_clen FOREIGN KEY(ID_clen) REFERENCES clen ON DELETE CASCADE
	);

	CREATE TABLE jazyk
	(
		ID_jazyk char(2) NOT NULL,

		CONSTRAINT PK_jazyk PRIMARY KEY(ID_jazyk)
	);

	CREATE TABLE ovlada
	(
		ID_clen char(10) NOT NULL,
		ID_typ char(10) NOT NULL,

		CONSTRAINT PK_ovlada PRIMARY KEY(ID_clen, ID_typ),
		CONSTRAINT FK_OA_clen FOREIGN KEY(ID_clen) REFERENCES clen ON DELETE CASCADE,
		CONSTRAINT FK_OA_typ FOREIGN KEY(ID_typ) REFERENCES typ_letadla ON DELETE CASCADE
	);

	CREATE TABLE umi
	(
		ID_clen char(10) NOT NULL,
		ID_jazyk char(2) NOT NULL,

		CONSTRAINT PK_umi PRIMARY KEY(ID_clen, ID_jazyk),
		CONSTRAINT FK_UI_clen FOREIGN KEY(ID_clen) REFERENCES clen ON DELETE CASCADE,
		CONSTRAINT FK_UI_jazyk FOREIGN KEY(ID_jazyk) REFERENCES jazyk ON DELETE CASCADE
	);

	CREATE SEQUENCE letadlo_id
		increment by 1
		start with 1
		maxvalue 1000
		nocache
		nocycle;
  
	CREATE OR REPLACE TRIGGER check_not_null
	before INSERT OR UPDATE 
	ON letadlo
	for EACH ROW
	begin
		if :new.ID_letadlo IS NULL THEN
			SELECT letadlo_id.nextval INTO :new.ID_letadlo from dual;
    end if;
	end;
  /
  CREATE OR REPLACE TRIGGER check_max_mess
  before INSERT
  ON letenka
  for EACH ROW
  declare
    lid letenka.ID_LET%TYPE;
    maximum integer;
    pocet integer;
  begin
    SELECT :new.ID_let INTO lid FROM dual;
    SELECT let.MAX_CESTUJICICH INTO maximum  FROM let WHERE let.ID_LET = lid;
    SELECT (SELECT COUNT(*) from letenka WHERE lid = ID_let GROUP BY ID_let) INTO pocet FROM dual;
    if (pocet+1 > maximum) THEN
      Raise_Application_Error(-20005, 'presahnut limit');
    end if;
  end;
  /
  
  CREATE OR REPLACE TRIGGER check_rodne_cislo
  before INSERT OR UPDATE
  ON technik
  for EACH ROW
  declare
    rc integer;
    rok integer;
    mesic integer;
    den integer;
    datum DATE;
  begin
    SELECT :new.rodne_cislo INTO rc FROM dual;
    rok := rc / 100000000;
    mesic := MOD(rc / 1000000, 100);
    den := MOD(rc / 10000, 100);
    
    if (MOD(rc, 11) != 0) THEN
      Raise_Application_Error(-20001, 'nevalidni rodne cislo');
    end if;
    
    if (mesic > 70) THEN
      mesic := mesic - 70;
    end if;
    
    if (mesic > 50) THEN
      mesic := mesic - 50;
    end if;
    
    begin
    datum := den||'.'||mesic||'.'||rok;
    exception
      when others then
        Raise_Application_Error(-20004, 'neplatne datum');
    end;
  end;
/

  SET SERVEROUTPUT ON;
  CREATE OR REPLACE PROCEDURE vypis_letenky(kod_letenky letenka.ID_letenka%TYPE)
  IS
    CURSOR c_letenka IS SELECT * FROM letenka NATURAL JOIN let;
    polozka c_letenka%ROWTYPE;
    pom_polozka c_letenka%ROWTYPE;
    nazev_tridy letenka.ID_trida%TYPE;
    cislo_mista misto.cislo%TYPE;
    umisteni_mista misto.umisteni%TYPE;
  begin
    SELECT * INTO pom_polozka FROM letenka NATURAL JOIN let WHERE kod_letenky = ID_letenka;
    
    -- open c_letenka;
    
    FOR ROW IN c_letenka
    LOOP
      if (row.ID_letenka = pom_polozka.ID_letenka) THEN
        dbms_output.put_line('Va¹e místo je umístìno:');
        SELECT (SELECT nazev FROM letenka NATURAL JOIN trida WHERE row.ID_trida = ID_trida) INTO nazev_tridy FROM dual;
        dbms_output.put_line('Tøída: ' || nazev_tridy);
        SELECT (SELECT cislo FROM letenka NATURAL JOIN misto WHERE row.ID_misto = ID_misto) INTO cislo_mista FROM dual;
        dbms_output.put_line('Místo: ' || cislo_mista);
        SELECT (SELECT umisteni FROM letenka NATURAL JOIN misto WHERE row.ID_misto = ID_misto) INTO umisteni_mista FROM dual;
        dbms_output.put_line('Umístìní: ' || umisteni_mista);
        dbms_output.put_line('Cena letenky: ' || row.cena || ',-');
      end if;
    END LOOP;
    EXCEPTION
      WHEN NO_DATA_FOUND THEN
        dbms_output.put_line('Letenka s kódem ' || kod_letenky || ' neexistuje.');
      WHEN OTHERS THEN
        Raise_Application_Error(-20006, 'Procedura vypis_letenky selhala');
  end;
  /
    
  ALTER session set nls_date_format='dd-mm-yyyy';
  CREATE OR REPLACE PROCEDURE oprava(technik_id technik.ID_technik%TYPE)
  IS
    lo_id letadlo.ID_letadlo%TYPE;
    datum letadlo.datum_posledni_revize%TYPE;
    typ_id letadlo.ID_typ%TYPE;
    jm technik.jmeno%TYPE;
    pr technik.prijmeni%TYPE;
    rc technik.rodne_cislo%TYPE;
  begin
    SELECT (SELECT ID_letadlo FROM letadlo WHERE ID_technik = technik_id) INTO lo_id FROM dual;
    SELECT (SELECT datum_posledni_revize FROM letadlo WHERE ID_technik = technik_id) INTO datum FROM dual;
    SELECT (SELECT ID_typ FROM letadlo WHERE ID_technik = technik_id) INTO typ_id FROM dual;

    dbms_output.put_line('Letadlo ' || lo_id || ' typu ' || typ_id || ' bylo naposledy revizováno dne ' || datum || '.');
    
    SELECT (SELECT jmeno FROM technik WHERE ID_technik = technik_id) INTO jm FROM dual;
    SELECT (SELECT prijmeni FROM technik WHERE ID_technik = technik_id) INTO pr FROM dual;
    SELECT (SELECT rodne_cislo FROM technik WHERE ID_technik = technik_id) INTO rc FROM dual;
    
    dbms_output.put_line('Technik, který revizi provádìl:');
    dbms_output.put_line('Jméno: ' || jm);
    dbms_output.put_line('Pøíjmení: ' || pr);
    dbms_output.put_line('Rodné èíslo: ' || rc);

    UPDATE letadlo SET datum_posledni_revize = TO_DATE(SYSDATE,'dd-mm-yyyy') WHERE ID_technik = technik_id;
    
    dbms_output.put_line('Datum revize aktualizováno na ' || TO_DATE(SYSDATE,'dd-mm-yyyy') || '.');
  end;
  /
  
	-- 							ID_technik
	INSERT INTO technik VALUES('0000-00001', 'Honza', 'Ka¹par', '6905026678');
	INSERT INTO technik VALUES('0000-00002', 'Petr', '©kvorecký', '8803241117');

	--								ID_typ
	INSERT INTO typ_letadla VALUES('2-7-4-7-01', 'USA Air', 'BOEING 747');
	INSERT INTO typ_letadla VALUES('2-4-8-2-01', 'Beijing Corporation', 'BOEING 482');

	--							ID_letadlo 																				ID_technik	  ID_typ
	INSERT INTO letadlo VALUES(NULL, TO_DATE('21-03-2008', 'dd-mm-yyyy'), TO_DATE('15-09-2015', 'dd-mm-yyyy'), '0000-00002', '2-4-8-2-01');
	INSERT INTO letadlo VALUES(NULL, TO_DATE('02-11-2004', 'dd-mm-yyyy'), TO_DATE('12-03-2016', 'dd-mm-yyyy'), '0000-00001', '2-7-4-7-01');

	--							ID_terminal
	INSERT INTO terminal VALUES('01-NX-0001', 'Sever 1');
	INSERT INTO terminal VALUES('01-WS-0001', 'Jihozápad 1');

	--						  ID_gate		  ID_terminal
	INSERT INTO gate VALUES('01-NX-0001', 1, '01-NX-0001');
	INSERT INTO gate VALUES('01-WS-0009', 9, '01-WS-0001');

	--							 ID_gate 		ID_typ
	INSERT INTO dovoluje VALUES('01-NX-0001', '2-7-4-7-01');
	INSERT INTO dovoluje VALUES('01-WS-0009', '2-4-8-2-01');

	--						ID_let 																												ID_letadlo 	  ID_terminal	ID_gate
	INSERT INTO let VALUES('NYork00001', TO_DATE('16-03-2016 17:30', 'dd-mm-yyyy hh24:mi'), TO_DATE('17-03-2016 03:45', 'dd-mm-yyyy hh24:mi'), 747, 1, '01-WS-0001', '01-WS-0009');
	INSERT INTO let VALUES('Sydney0001', TO_DATE('17-03-2016 02:15', 'dd-mm-yyyy hh24:mi'), TO_DATE('17-03-2016 11:00', 'dd-mm-yyyy hh24:mi'), 482, 2, '01-NX-0001', '01-NX-0001');

	--							ID_trida
	INSERT INTO trida VALUES('Main-00001', 'první');
	INSERT INTO trida VALUES('Buss-00001', 'business');
  INSERT INTO trida VALUES('Tour-00001', 'turistická');

	--							ID_misto  cislo  umisteni  ID_trida 	ID_letadlo
	INSERT INTO misto VALUES('Main-175-3', 175, 'ulièka', 'Main-00001', 2);
	INSERT INTO misto VALUES('Buss-343-2', 343, 'uprostøed', 'Buss-00001', 1);
	INSERT INTO misto VALUES('Tour-253-1', 253, 'okýnko', 'Tour-00001', 1);

	--							ID_letenka 	 cena 		ID_let 		ID_trida
	INSERT INTO letenka VALUES('ASDFGHJKL0', 28648, 'NYork00001', 'Tour-00001', 'Tour-253-1');
	INSERT INTO letenka VALUES('ASDFGHJKL2', 13526, 'NYork00001', 'Buss-00001', 'Buss-343-2');
	INSERT INTO letenka VALUES('ASDFGHJKL1', 14964, 'Sydney0001', 'Main-00001', 'Main-175-3');

	--						ID_clen
	INSERT INTO clen VALUES('Cap-123456', 'Zdenìk', 'Vavøina', '7805017748', 'mu¾', 1204, TO_DATE('28-09-2015', 'dd-mm-yyyy'), NULL);
	INSERT INTO clen VALUES('Cap-123654', 'Ale¹', 'Pokorný', '8206241252', 'mu¾', 684, TO_DATE('14-12-2015', 'dd-mm-yyyy'), NULL);
	INSERT INTO clen VALUES('Stw-123456', 'Lucie', 'Zapletalová', '8762053751', '¾ena', NULL, NULL, 'servis');
	INSERT INTO clen VALUES('Stw-123654', 'Aneta', 'Vykoukalová', '8356144940', '¾ena', NULL, NULL, 'servis');
	INSERT INTO clen VALUES('Stw-123123', 'Linda', 'Nìmcová', '8557062745', '¾ena', NULL, NULL, 'prùvodèí');

	--						ID_clen
	INSERT INTO clen VALUES('Cap-654321', 'Tomá¹', 'Motl', '7111112932', 'mu¾', 1352, TO_DATE('11-06-2015', 'dd-mm-yyyy'), NULL);
	INSERT INTO clen VALUES('Cap-654123', 'Ladislav', 'Mamut', '7504037838', 'mu¾', 586, TO_DATE('14-02-2016', 'dd-mm-yyyy'), NULL);
	INSERT INTO clen VALUES('Stw-654321', 'Patrik', 'Fatka', '8108163481', 'mu¾', NULL, NULL, 'servis');
	INSERT INTO clen VALUES('Stw-654123', 'Luká¹', 'Vácha', '7806234711', 'mu¾', NULL, NULL, 'servis');
	INSERT INTO clen VALUES('Stw-654654', 'Tereza', 'Du¹ková', '7257306881', '¾ena', NULL, NULL, 'prùvodèí');

	--						ID_let 		 	ID_clen
	INSERT INTO ma VALUES('NYork00001', 'Cap-123456');
	INSERT INTO ma VALUES('NYork00001', 'Cap-123654');
	INSERT INTO ma VALUES('NYork00001', 'Stw-123456');
	INSERT INTO ma VALUES('NYork00001', 'Stw-123654');
	INSERT INTO ma VALUES('NYork00001', 'Stw-123123');

	INSERT INTO ma VALUES('Sydney0001', 'Cap-654321');	
	INSERT INTO ma VALUES('Sydney0001', 'Cap-654123');
	INSERT INTO ma VALUES('Sydney0001', 'Stw-654321');
	INSERT INTO ma VALUES('Sydney0001', 'Stw-654123');
	INSERT INTO ma VALUES('Sydney0001', 'Stw-654654');

	INSERT INTO jazyk VALUES('cs'); -- cestina
	INSERT INTO jazyk VALUES('de'); -- nemcina
	INSERT INTO jazyk VALUES('es'); -- spanelstina
	INSERT INTO jazyk VALUES('bg'); --bulharstina
	INSERT INTO jazyk VALUES('da'); --danstina
	INSERT INTO jazyk VALUES('fi'); --finstina
	INSERT INTO jazyk VALUES('ga'); --irstina
	INSERT INTO jazyk VALUES('he'); --hebrejstina
	INSERT INTO jazyk VALUES('hu'); --madarstina
	INSERT INTO jazyk VALUES('hi'); --hindstina
	INSERT INTO jazyk VALUES('in'); --indonestina
	INSERT INTO jazyk VALUES('it'); --italstina
	INSERT INTO jazyk VALUES('ja'); --japonstina
	INSERT INTO jazyk VALUES('ko'); --korejstina
	INSERT INTO jazyk VALUES('lt'); --litevstina
	INSERT INTO jazyk VALUES('lv'); --lotystina
	INSERT INTO jazyk VALUES('nl'); --nizozemstina
	INSERT INTO jazyk VALUES('no'); --norstina
	INSERT INTO jazyk VALUES('pl'); --polstina
	INSERT INTO jazyk VALUES('pt'); --portugalstina
	INSERT INTO jazyk VALUES('ro'); --romunstina
	INSERT INTO jazyk VALUES('ru'); --rustina
	INSERT INTO jazyk VALUES('sl'); --slovinstina
	INSERT INTO jazyk VALUES('sk'); --slovenstina
	INSERT INTO jazyk VALUES('sv'); --svedstina
	INSERT INTO jazyk VALUES('th'); --thaistina
	INSERT INTO jazyk VALUES('tr'); --turectina
	INSERT INTO jazyk VALUES('vi'); --vietnamstina
	INSERT INTO jazyk VALUES('zh'); --cinstina

	--							ID_clen		ID_typ
	INSERT INTO ovlada VALUES('Cap-123456', '2-7-4-7-01');
	INSERT INTO ovlada VALUES('Cap-123456', '2-4-8-2-01');

	INSERT INTO ovlada VALUES('Cap-123654', '2-4-8-2-01');

	INSERT INTO ovlada VALUES('Cap-654321', '2-7-4-7-01');
	INSERT INTO ovlada VALUES('Cap-654321', '2-4-8-2-01');

	INSERT INTO ovlada VALUES('Cap-654123', '2-7-4-7-01');

	--						ID_clen		
	INSERT INTO umi VALUES('Stw-123456', 'cs');
	INSERT INTO umi VALUES('Stw-123456', 'sk');
	INSERT INTO umi VALUES('Stw-123456', 'ru');

	INSERT INTO umi VALUES('Stw-123123', 'cs');
	INSERT INTO umi VALUES('Stw-123123', 'zh');
	INSERT INTO umi VALUES('Stw-123123', 'no');

	INSERT INTO umi VALUES('Stw-123654', 'cs');
	INSERT INTO umi VALUES('Stw-123654', 'pl');

	INSERT INTO umi VALUES('Stw-654123', 'cs');
	INSERT INTO umi VALUES('Stw-654123', 'pl');
	INSERT INTO umi VALUES('Stw-654123', 'de');


	INSERT INTO umi VALUES('Stw-654321', 'cs');
	INSERT INTO umi VALUES('Stw-654321', 'ru');
	INSERT INTO umi VALUES('Stw-654321', 'nl');

	INSERT INTO umi VALUES('Stw-654654', 'cs');
	INSERT INTO umi VALUES('Stw-654654', 'ko');
	INSERT INTO umi VALUES('Stw-654654', 'ja');
  
  
  exec vypis_letenky('ASDFGHJKL1');
  
 -- ALTER session set nls_date_format='dd-mm-yyyy';
  exec oprava('0000-00001');
  exec oprava('0000-00002');
  
  -- DROP INDEX my_index;
  
  EXPLAIN PLAN FOR
  SELECT prijmeni, COUNT(*) AS pocet_jazyku
  FROM clen NATURAL JOIN umi
  GROUP BY prijmeni;
  
  SELECT * FROM TABLE(DBMS_XPLAN.display);  
  
  CREATE INDEX my_index ON clen (prijmeni);

  EXPLAIN PLAN FOR
  SELECT /*+ INDEX(clen, my_index) pouziti indexu podle oracle help*/ prijmeni, COUNT(*) AS pocet_jazyku
  FROM clen, umi
  WHERE clen.ID_clen = umi.ID_clen
  GROUP BY prijmeni;
  
  SELECT * FROM TABLE(DBMS_XPLAN.display);
 
 GRANT ALL ON jazyk TO xkolec07;
 GRANT ALL ON clen TO xkolec07;
 GRANT ALL ON ma TO xkolec07;
 GRANT ALL ON letadlo TO xkolec07;
 GRANT ALL ON letenka TO xkolec07;
 GRANT ALL ON umi TO xkolec07;
 GRANT ALL ON technik TO xkolec07;
 GRANT ALL ON trida TO xkolec07;
 GRANT ALL ON gate TO xkolec07;
 GRANT ALL ON misto TO xkolec07;
 GRANT ALL ON ovlada TO xkolec07;
 GRANT ALL ON typ_letadla TO xkolec07;
 GRANT ALL ON let TO xkolec07;
 GRANT ALL ON dovoluje TO xkolec07;
 GRANT ALL ON terminal TO xkolec07;
 
  
 -- DROP MATERIALIZED VIEW pocet_jazyku;
  
 -- CREATE MATERIALIZED VIEW ceny_trida
 -- REFRESH ON COMMIT
 -- AS
    -- Zobrazi tridy a cenu vsech letenek v jednotlivych tridach.
    -- prikaz s GROUP BY a agrefacni funkci SUM()
 --   SELECT ID_let as Let, ID_trida as Trida, Sum(cena) as Cena_letenek
 --   FROM xjanot02.letenka
 --   GROUP BY ID_Let, ID_trida;
  
  SELECT * FROM xkolec07.ceny_trida;
  
  INSERT INTO misto VALUES('Tour-255-1', 255, 'okýnko', 'Tour-00001', 1);
  INSERT INTO letenka VALUES('ASDFGHJKL4', 30000, 'NYork00001', 'Tour-00001', 'Tour-255-1');
  
  commit;
  
  SELECT * FROM xkolec07.ceny_trida;
  