-- ============================================================
--  seed.sql  –  5 librarii cu carti, ratinguri si clienti
--  Rulare:  sqlite3 librarie.db < seed.sql
-- ============================================================

PRAGMA foreign_keys = OFF;

-- ============================================================
--  CREARE TABELE (daca nu exista deja)
-- ============================================================

CREATE TABLE IF NOT EXISTS librarii (
    id       INTEGER PRIMARY KEY AUTOINCREMENT,
    denumire TEXT,
    adresa   TEXT
);

CREATE TABLE IF NOT EXISTS carti (
    id             INTEGER PRIMARY KEY AUTOINCREMENT,
    idLibrarie     INTEGER,
    titlu          TEXT,
    anPublicare    INTEGER,
    nrPag          INTEGER,
    categorie      INTEGER,
    pret           REAL,
    autorNume      TEXT,
    autorAn        INTEGER,
    autorNrPremii  INTEGER,
    autorNrScrieri INTEGER,
    FOREIGN KEY(idLibrarie) REFERENCES librarii(id)
);

CREATE TABLE IF NOT EXISTS ratinguri (
    idCarte INTEGER,
    valoare REAL,
    FOREIGN KEY(idCarte) REFERENCES carti(id)
);

CREATE TABLE IF NOT EXISTS clienti (
    id                INTEGER PRIMARY KEY AUTOINCREMENT,
    idLibrarie        INTEGER,
    nume              TEXT,
    buget             REAL,
    gen               TEXT,
    areCardFidelitate INTEGER,
    FOREIGN KEY(idLibrarie) REFERENCES librarii(id)
);

CREATE TABLE IF NOT EXISTS carti_cumparate (
    idClient INTEGER,
    idCarte  INTEGER,
    FOREIGN KEY(idClient) REFERENCES clienti(id),
    FOREIGN KEY(idCarte)  REFERENCES carti(id)
);

-- ============================================================
--  CURATARE DATE VECHI
-- ============================================================

DELETE FROM carti_cumparate;
DELETE FROM ratinguri;
DELETE FROM clienti;
DELETE FROM carti;
DELETE FROM librarii;
DELETE FROM sqlite_sequence WHERE name IN ('librarii','carti','clienti');

PRAGMA foreign_keys = ON;

-- ============================================================
--  LIBRARII  (id-urile vor fi 1-5)
-- ============================================================

INSERT INTO librarii (denumire, adresa) VALUES
    ('Libraria Eminescu',    'Bulevardul Unirii nr. 14, Bucuresti'),
    ('Cartea de Nisip',      'Strada Lipscani nr. 7, Bucuresti'),
    ('Humanitas Cluj',       'Calea Dorobantilor nr. 3, Cluj-Napoca'),
    ('Libraria Universalis', 'Strada Mihai Eminescu nr. 22, Iasi'),
    ('Pagina de Hartie',     'Bulevardul Victoriei nr. 55, Timisoara');

-- ============================================================
--  CARTI  (id=1-6 Lib1, 7-11 Lib2, 12-16 Lib3, 17-21 Lib4, 22-26 Lib5)
-- ============================================================

INSERT INTO carti (idLibrarie, titlu, anPublicare, nrPag, categorie, pret, autorNume, autorAn, autorNrPremii, autorNrScrieri) VALUES
    (1, 'Orbitor',                          1996, 524, 1,  52.99, 'Mircea Cartarescu',        1956, 3, 12),
    (1, 'Nostalgia',                        1993, 320, 1,  44.50, 'Mircea Cartarescu',        1956, 3, 12),
    (1, '1984',                             1949, 328, 3,  35.00, 'George Orwell',            1903, 2,  9),
    (1, 'Ferma animalelor',                 1945, 112, 3,  29.99, 'George Orwell',            1903, 2,  9),
    (1, 'Norwegian Wood',                   1987, 296, 2,  39.99, 'Haruki Murakami',          1949, 5, 15),
    (1, 'Harry Potter si Piatra Filozofala',1997, 368, 5,  55.00, 'J.K. Rowling',             1965, 4,  7),
    (2, 'Un veac de singuratate',           1967, 432, 1,  48.99, 'Gabriel Garcia Marquez',   1927, 6, 11),
    (2, 'Crima si pedeapsa',                1866, 551, 1,  42.00, 'Fiodor Dostoievski',       1821, 3, 10),
    (2, 'Maestrul si Margareta',            1967, 480, 5,  46.50, 'Mihail Bulgakov',          1891, 2,  8),
    (2, 'Procesul',                         1925, 240, 1,  33.00, 'Franz Kafka',              1883, 1,  7),
    (2, 'Mandrie si prejudecata',           1813, 432, 2,  37.50, 'Jane Austen',              1775, 2,  6),
    (3, 'Kafka pe malul marii',             2002, 448, 1,  47.00, 'Haruki Murakami',          1949, 5, 15),
    (3, 'Numele trandafirului',             1980, 502, 3,  54.00, 'Umberto Eco',              1932, 4,  9),
    (3, 'Don Quijote',                      1605, 863, 7,  65.00, 'Miguel de Cervantes',      1547, 1,  5),
    (3, 'Micul Print',                      1943,  96, 1,  22.00, 'Antoine de Saint-Exupery', 1900, 3,  6),
    (3, 'Stapanul inelelor',                1954, 576, 5,  59.99, 'J.R.R. Tolkien',           1892, 5,  8),
    (4, 'Anna Karenina',                    1877, 864, 2,  62.00, 'Lev Tolstoi',              1828, 4, 14),
    (4, 'Fratii Karamazov',                 1880, 796, 1,  58.00, 'Fiodor Dostoievski',       1821, 3, 10),
    (4, 'Doamna Bovary',                    1857, 392, 2,  38.00, 'Gustave Flaubert',         1821, 2,  6),
    (4, 'Metamorfoza',                      1915, 128, 1,  25.00, 'Franz Kafka',              1883, 1,  7),
    (4, 'O suta de ani de singuratate',     1967, 432, 1,  48.99, 'Gabriel Garcia Marquez',   1927, 6, 11),
    (5, 'Dune',                             1965, 688, 4,  57.00, 'Frank Herbert',            1920, 3,  7),
    (5, 'Neuromantul',                      1984, 288, 4,  43.00, 'William Gibson',           1948, 5, 12),
    (5, 'Fundatia',                         1951, 255, 4,  39.00, 'Isaac Asimov',             1920, 6, 40),
    (5, 'Razboiul lumilor',                 1898, 192, 4,  31.00, 'H.G. Wells',               1866, 2, 18),
    (5, 'Fahrenheit 451',                   1953, 256, 4,  36.50, 'Ray Bradbury',             1920, 4, 27);

-- ============================================================
--  RATINGURI
-- ============================================================

INSERT INTO ratinguri (idCarte, valoare) VALUES
    (1,4.8),(1,4.5),(1,5.0),(2,4.2),(2,4.0),(3,5.0),(3,4.9),(3,5.0),
    (4,4.3),(4,4.1),(5,4.9),(5,4.7),(6,4.7),(6,4.8),(6,4.9),
    (7,5.0),(7,4.8),(8,4.6),(8,4.4),(9,4.9),(9,5.0),(10,4.2),(10,4.0),(11,4.5),(11,4.3),
    (12,4.6),(12,4.4),(13,4.8),(13,4.7),(14,4.5),(14,4.3),(15,4.9),(15,5.0),(16,5.0),(16,4.9),(16,5.0),
    (17,4.9),(17,4.8),(18,4.7),(18,4.6),(19,4.3),(19,4.2),(20,4.5),(20,4.4),(21,5.0),(21,4.8),
    (22,4.8),(22,4.7),(22,4.9),(23,4.5),(23,4.3),(24,4.7),(24,4.6),(25,4.2),(25,4.0),(26,4.6),(26,4.5);

-- ============================================================
--  CLIENTI  (id=1-4 Lib1, 5-8 Lib2, 9-12 Lib3, 13-16 Lib4, 17-20 Lib5)
-- ============================================================

INSERT INTO clienti (idLibrarie, nume, buget, gen, areCardFidelitate) VALUES
    (1,'Andrei Popescu',   150.00,'M',1),(1,'Maria Ionescu',    200.00,'F',1),
    (1,'Alexandru Duma',    80.00,'M',0),(1,'Elena Constantin', 320.00,'F',1),
    (2,'Radu Georgescu',    90.00,'M',0),(2,'Ioana Stanescu',   175.00,'F',1),
    (2,'Mihai Dumitrescu', 120.00,'M',1),(2,'Ana Popa',          60.00,'F',0),
    (3,'Bogdan Marin',     250.00,'M',1),(3,'Cristina Lungu',   130.00,'F',1),
    (3,'Dan Enache',        45.00,'M',0),(3,'Raluca Nitu',      180.00,'F',1),
    (4,'Florin Badea',     100.00,'M',0),(4,'Gabriela Stoica',  210.00,'F',1),
    (4,'Horia Vasile',      70.00,'M',0),(4,'Irina Moraru',     295.00,'F',1),
    (5,'Julien Barbu',     160.00,'M',1),(5,'Karina Nica',      220.00,'F',1),
    (5,'Lucian Oprea',      55.00,'M',0),(5,'Monica Serban',    340.00,'F',1);

-- ============================================================
--  CARTI CUMPARATE
-- ============================================================

INSERT INTO carti_cumparate (idClient, idCarte) VALUES
    (1,3),(1,4),(2,5),(2,6),(4,1),(4,2),
    (5,7),(6,8),(6,9),(7,11),
    (9,16),(10,13),(10,15),(12,12),
    (13,20),(14,17),(14,18),(16,19),
    (17,22),(18,24),(18,26),(20,23),(20,25);