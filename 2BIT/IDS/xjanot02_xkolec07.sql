
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


CREATE TABLE technik(
	ID_technik char(10) NOT NULL,
	jmeno varchar(20) NOT NULL,
	prijmeni varchar(20) NOT NULL,
	rodne_cislo char(10) NOT NULL,

	PRIMARY KEY(ID_technik),

	CHECK (MOD(rodne_cislo, 11) = 0)
);

CREATE TABLE typ_letadla
(
	ID_typ char(10) NOT NULL,
	vyrobce varchar(20) NOT NULL,
	typ varchar(20) NOT NULL,

	PRIMARY KEY(ID_typ)
);

CREATE TABLE letadlo
(
	ID_letadlo char(10) NOT NULL,
	datum_vyroby DATE NOT NULL,
	datum_posledni_revize DATE,
	ID_technik char(10) NOT NULL,
	ID_typ char(10) NOT NULL,

	PRIMARY KEY(ID_letadlo),
	CONSTRAINT FK_LO_typ FOREIGN KEY(ID_typ) REFERENCES typ_letadla ON DELETE CASCADE,
	CONSTRAINT FK_LO_technik FOREIGN KEY(ID_technik) REFERENCES technik ON DELETE CASCADE,

	
	CHECK (datum_vyroby < datum_posledni_revize)	
);

CREATE TABLE terminal
(
	ID_terminal char(10) NOT NULL,
	nazev varchar(20) NOT NULL,

	PRIMARY KEY(ID_terminal)
);

CREATE TABLE gate
(
	ID_gate char(10) NOT NULL,
	cislo INTEGER NOT NULL,
	ID_terminal char(10) NOT NULL,

	PRIMARY KEY(ID_gate),
	CONSTRAINT FK_G_terminal FOREIGN KEY(ID_terminal) REFERENCES terminal ON DELETE CASCADE,

	CHECK (cislo >= 0)
);

CREATE TABLE dovoluje
(
	ID_gate char(10) NOT NULL,
	ID_typ char(10) NOT NULL,

	PRIMARY KEY(ID_gate, ID_typ),
	CONSTRAINT FK_D_gate FOREIGN KEY(ID_gate) REFERENCES gate ON DELETE CASCADE,
	CONSTRAINT FK_D_typ FOREIGN KEY(ID_typ) REFERENCES typ_letadla ON DELETE CASCADE
);

CREATE TABLE let
(
	ID_let char(10) NOT NULL,
	cas_odletu TIMESTAMP NOT NULL,
	cas_priletu TIMESTAMP NOT NULL,
	ID_letadlo char(10) NOT NULL,
	ID_terminal char(10) NOT NULL,
	ID_gate char(10) NOT NULL,

	PRIMARY KEY(ID_let),
	CONSTRAINT FK_LT_letadlo FOREIGN KEY(ID_letadlo) REFERENCES letadlo ON DELETE CASCADE,
	CONSTRAINT FK_LT_terminal FOREIGN KEY(ID_terminal) REFERENCES terminal ON DELETE CASCADE,
	CONSTRAINT FK_LT_gate FOREIGN KEY(ID_gate) REFERENCES gate ON DELETE CASCADE,

	CHECK (cas_odletu < cas_priletu)
);

CREATE TABLE trida
(
	ID_trida char(10) NOT NULL,
	nazev varchar(20) NOT NULL,

	PRIMARY KEY(ID_trida),

	CHECK (nazev IN ('business', 'první', 'turisticka'))
);

CREATE TABLE letenka
(
	ID_letenka char(10) NOT NULL,
	cena INTEGER NOT NULL,
    ID_let char(10) NOT NULL,
    ID_trida char(10) NOT NULL,

	PRIMARY KEY(ID_letenka),
	CONSTRAINT FK_LA_let FOREIGN KEY(ID_let) REFERENCES let ON DELETE CASCADE,
	CONSTRAINT FK_LA_trida FOREIGN KEY(ID_trida) REFERENCES trida ON DELETE CASCADE,

	CHECK (cena >= 0)
);

CREATE TABLE misto
(
	ID_misto char(10) NOT NULL,
	cislo INTEGER NOT NULL,
	umisteni varchar(10) NOT NULL,
	ID_trida char(10) NOT NULL,
	ID_letadlo char(10) NOT NULL,

	PRIMARY KEY(ID_misto),
	CONSTRAINT FK_MO_trida FOREIGN KEY(ID_trida) REFERENCES trida ON DELETE CASCADE,
	CONSTRAINT FK_MO_letadlo FOREIGN KEY(ID_letadlo) REFERENCES letadlo ON DELETE CASCADE,

	CHECK (cislo >= 0),
	CHECK (umisteni IN ('okýnko', 'uprostøed', 'ulièka'))
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

	PRIMARY KEY(ID_clen),

	CHECK (MOD(rodne_cislo, 11) = 0),
	CHECK (pohlavi IN ('mu¾', '¾ena')),
	CHECK (pocet_hodin >= 0)
);

CREATE TABLE ma
(
	ID_let char(10) NOT NULL,
	ID_clen char(10) NOT NULL,

	PRIMARY KEY(ID_let, ID_clen),
	CONSTRAINT FK_MA_let FOREIGN KEY(ID_let) REFERENCES let ON DELETE CASCADE,
	CONSTRAINT FK_MA_clen FOREIGN KEY(ID_clen) REFERENCES clen ON DELETE CASCADE
);

CREATE TABLE jazyk
(
	ID_jazyk char(2) NOT NULL,

	PRIMARY KEY(ID_jazyk)
);

CREATE TABLE ovlada
(
	ID_clen char(10) NOT NULL,
	ID_typ char(10) NOT NULL,

	PRIMARY KEY(ID_clen, ID_typ),
	CONSTRAINT FK_OA_clen FOREIGN KEY(ID_clen) REFERENCES clen ON DELETE CASCADE,
	CONSTRAINT FK_OA_typ FOREIGN KEY(ID_typ) REFERENCES typ_letadla ON DELETE CASCADE
);

CREATE TABLE umi
(
	ID_clen char(10) NOT NULL,
	ID_jazyk char(2) NOT NULL,

	PRIMARY KEY(ID_clen, ID_jazyk),
	CONSTRAINT FK_UI_clen FOREIGN KEY(ID_clen) REFERENCES clen ON DELETE CASCADE,
	CONSTRAINT FK_UI_jazyk FOREIGN KEY(ID_jazyk) REFERENCES jazyk ON DELETE CASCADE
);

-- 							ID_technik
INSERT INTO technik VALUES('0000-00001', 'Honza', 'Ka¹par', '6905026678');
INSERT INTO technik VALUES('0000-00002', 'Petr', '©kvorecký', '8803241117');

--								ID_typ
INSERT INTO typ_letadla VALUES('2-7-4-7-01', 'USA Air', 'BOEING 747');
INSERT INTO typ_letadla VALUES('2-4-8-2-01', 'Beijing Corporation', 'BOEING 482');

--							ID_letadlo 																				ID_technik	  ID_typ
INSERT INTO letadlo VALUES('0001-00001', TO_DATE('21-03-2008', 'dd-mm-yyyy'), TO_DATE('15-09-2015', 'dd-mm-yyyy'), '0000-00002', '2-4-8-2-01');
INSERT INTO letadlo VALUES('0004-00001', TO_DATE('02-11-2004', 'dd-mm-yyyy'), TO_DATE('12-03-2016', 'dd-mm-yyyy'), '0000-00001', '2-7-4-7-01');

--							ID_terminal
INSERT INTO terminal VALUES('01-NX-0001', 'Sever 1');
INSERT INTO terminal VALUES('01-WS-0001', 'Jihozápad 1');

--						  ID_gate		  ID_terminal
INSERT INTO gate VALUES('01-NX-0001', 1, '01-NX-0001');
INSERT INTO gate VALUES('01-WS-0009', 9, '01-WS-0001');

--							 ID_gate 		ID_typ
INSERT INTO dovoluje VALUES('01-NX-0001', '2-7-4-7-01');
INSERT INTO dovoluje VALUES('01-WS-0009', '2-4-8-2-01');

--						ID_let 																															ID_letadlo 	  ID_terminal	ID_gate
INSERT INTO let VALUES('NYork00001', TO_DATE('16-03-2016 17:30', 'dd-mm-yyyy hh24:mi'), TO_DATE('17-03-2016 03:45', 'dd-mm-yyyy hh24:mi'), '0001-00001', '01-WS-0001', '01-WS-0009');
INSERT INTO let VALUES('Sydney0001', TO_DATE('17-03-2016 02:15', 'dd-mm-yyyy hh24:mi'), TO_DATE('17-03-2016 11:00', 'dd-mm-yyyy hh24:mi'), '0004-00001', '01-NX-0001', '01-NX-0001');

--							ID_trida
INSERT INTO trida VALUES('Main-00001', 'první');
INSERT INTO trida VALUES('Buss-00001', 'business');

--							ID_letenka 	 cena 		ID_let 		ID_trida
INSERT INTO letenka VALUES('ASDFGHJKL0', 28648, 'NYork00001', 'Buss-00001');
INSERT INTO letenka VALUES('ASDFGHJKL1', 14964, 'Sydney0001', 'Main-00001');

--							ID_misto  cislo  umisteni  ID_trida 	ID_letadlo
INSERT INTO misto VALUES('Main-175-3', 175, 'ulièka', 'Main-00001', '0004-00001');
INSERT INTO misto VALUES('Buss-342-1', 342, 'okýnko', 'Buss-00001', '0001-00001');

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