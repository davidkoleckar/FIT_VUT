CREATE TABLE users(
	id int NOT NULL AUTO_INCREMENT,
	username varchar(20) NOT NULL,
	password char(60) NOT NULL,
	role varchar(20) NOT NULL,
	rodne_cislo char(10),
	pohlavi varchar(4),
	email varchar(30),
	activ int(11),

	PRIMARY KEY(id)

)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO users VALUES('1', 'admin', '$2y$10$QRwjujj48hf9Lu4GpFSVWucEmd/jRWRmRLaiumGnCLE6FkolM7pKS','admin', '6905026678', 'muž','admin@seznam.cz','1');
INSERT INTO users VALUES('2', 'technik', '$2y$10$EsyUIQu0BXNEMlbWSnzwh.I6CJqIrH7Im0lIBBWLWzRe0h6eDeaoS','technik', '8803241117', 'muž','technik@seznam.cz','1');
INSERT INTO users VALUES('3', 'spravce', '$2y$10$XVG4UZJ.Y20DKgFByrnNPOC90/yXxy5kJ3QJpRcyBZVjJUkWbIuhG','spravce', '', 'muž','spravce@seznam.cz','1');


CREATE TABLE technik(
	ID_technik int NOT NULL,
	jmeno varchar(20) NOT NULL,
	rodne_cislo char(10),

	PRIMARY KEY(ID_technik)

)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO technik VALUES('2', 'Petr', '8803241117');

CREATE TABLE typ_letadla
(
	ID_typ int NOT NULL AUTO_INCREMENT,
	vyrobce varchar(20) NOT NULL,
	typ varchar(20) NOT NULL,

	PRIMARY KEY(ID_typ)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO typ_letadla VALUES('1', 'USA Air', 'BOEING 747');
INSERT INTO typ_letadla VALUES('2', 'Beijing Corporation', 'BOEING 482');

CREATE TABLE letadlo
(
	ID_letadlo int NOT NULL AUTO_INCREMENT,
	datum_vyroby DATE NOT NULL,
	datum_posledni_revize DATE,
  ID_technik int NOT NULL REFERENCES technik (ID_technik),
	ID_typ int NOT NULL REFERENCES typ_letadla (ID_typ),

	PRIMARY KEY(ID_letadlo)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO letadlo VALUES('1', '2008-03-21', '', '2', '1');
INSERT INTO letadlo VALUES('2', '2004-02-11', '', '2', '2');


CREATE TABLE terminal
(
	ID_terminal int NOT NULL AUTO_INCREMENT,
	nazev varchar(20) NOT NULL,

	PRIMARY KEY(ID_terminal)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO terminal VALUES('1', 'Sever 1');
INSERT INTO terminal VALUES('2', 'Jihozápad 1');

CREATE TABLE gate
(
	ID_gate int NOT NULL AUTO_INCREMENT,
	cislo int NOT NULL,
	ID_terminal int NOT NULL REFERENCES terminal (ID_terminal),

	PRIMARY KEY(ID_gate)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO gate VALUES('1', '11','1');
INSERT INTO gate VALUES('2', '22','2');
INSERT INTO gate VALUES('3', '33','2');

CREATE TABLE dovoluje
(
	ID_dovoluje int NOT NULL AUTO_INCREMENT,
	ID_gate int NOT NULL REFERENCES gate (ID_gate),
	ID_typ int NOT NULL REFERENCES typ_letadla (ID_typ),

	PRIMARY KEY(ID_dovoluje)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO dovoluje VALUES('1', '1', '1');
INSERT INTO dovoluje VALUES('2', '2', '1');
INSERT INTO dovoluje VALUES('3', '3', '2');

CREATE TABLE let
(
	ID_let int NOT NULL AUTO_INCREMENT,
	cas_odletu DATETIME NOT NULL,
	cas_priletu DATETIME NOT NULL,
	max_cestujicich int NOT NULL,
	kam varchar(30) NOT NULL,

	ID_letadlo int NOT NULL REFERENCES letadlo (ID_letadlo),
	ID_terminal int NOT NULL REFERENCES terminal (ID_terminal),
	ID_gate int NOT NULL REFERENCES gate (ID_gate),

	PRIMARY KEY(ID_let)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO let VALUES('1', '2008-03-21 17:30:00', '2008-03-21 20:30:00', '10','Londýn','1', '2', '2');


CREATE TABLE clen
(
	ID_clen int NOT NULL AUTO_INCREMENT,
	jmeno varchar(20) NOT NULL,
	rodne_cislo char(10) ,
	pohlavi varchar(5) ,
	funkce varchar(20) NOT NULL,

	PRIMARY KEY(ID_clen)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO clen VALUES('1', 'Adam Novák', '7111112932', 'muž','pilot');
INSERT INTO clen VALUES('2', 'Eva Nováková', '7504037838', 'žena','steward');
INSERT INTO clen VALUES('3', 'Kateřina Svatojánská', '8108163481', 'žena','steward');

CREATE TABLE ovlada
(
	ID_ovlada int NOT NULL AUTO_INCREMENT,
	ID_clen int NOT NULL REFERENCES clen (ID_clen),
	ID_typ int NOT NULL REFERENCES typ_letadla (ID_typ),

	PRIMARY KEY(ID_ovlada)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO ovlada VALUES('1', '1', '1');

CREATE TABLE jazyk
(
	ID_jazyk char(2) NOT NULL,

	PRIMARY KEY(ID_jazyk)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;


INSERT INTO jazyk VALUES('cs'); -- cestina
INSERT INTO jazyk VALUES('de'); -- nemcina
INSERT INTO jazyk VALUES('es'); -- spanelstina
INSERT INTO jazyk VALUES('bg'); -- bulharstina
INSERT INTO jazyk VALUES('ru'); -- rustina
INSERT INTO jazyk VALUES('sl'); -- slovinstina
INSERT INTO jazyk VALUES('sk'); -- slovenstina


CREATE TABLE umi
(
	ID_umi int NOT NULL AUTO_INCREMENT,
	ID_clen int NOT NULL REFERENCES clen (ID_clen),
	ID_jazyk char(2) NOT NULL REFERENCES jazyk (ID_jazyk),

	PRIMARY KEY(ID_umi)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO umi VALUES('1', '2', 'cs');
INSERT INTO umi VALUES('2', '3', 'sk');

CREATE TABLE ma
(
	ID_ma int NOT NULL AUTO_INCREMENT,
	ID_let int NOT NULL REFERENCES let (ID_let),
	ID_clen int NOT NULL REFERENCES clen (ID_clen),

	PRIMARY KEY(ID_ma)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO ma VALUES('1', '1', '1');
INSERT INTO ma VALUES('2', '1', '2');

CREATE TABLE letenka
(
	ID_letenka int NOT NULL AUTO_INCREMENT,
	cena int NOT NULL, 
	trida varchar(20),
	umisteni varchar(20),
	ID_let int NOT NULL REFERENCES let (ID_let),

	PRIMARY KEY(ID_letenka)
)ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_czech_cs;

INSERT INTO letenka VALUES('1', '2500', 'první', 'uprostřed', '1');
INSERT INTO letenka VALUES('2', '1500', 'turistická', 'ulička', '1');