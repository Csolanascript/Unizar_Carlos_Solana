    -- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Sim�n Gay�n   --- 870984


INSERT INTO Produccion SELECT DISTINCT title, production_year, kind
	FROM datosdb.datospeliculas
	WHERE title IS NOT NULL AND kind IS NOT NULL AND production_year IS NOT NULL;
 
INSERT INTO Genero SELECT DISTINCT title, production_year, keyword, kind
	FROM datosdb.datospeliculas
	WHERE title IS NOT NULL AND production_year IS NOT NULL AND keyword IS NOT NULL AND kind IS NOT NULL;
 
INSERT INTO Serie SELECT DISTINCT title, production_year, series_years, kind
    FROM datosdb.datospeliculas
	WHERE title IS NOT NULL AND production_year IS NOT NULL AND kind IS NOT NULL AND kind = 'tv series';
 
INSERT INTO Capitulo SELECT DISTINCT serie_title,season_nr,episode_nr,title,production_year, kind, serie_prod_year, 'tv series'
	FROM datosdb.datospeliculas
	WHERE title IS NOT NULL AND serie_title IS NOT NULL AND production_year IS NOT NULL AND kind IS NOT NULL AND serie_prod_year IS NOT NULL AND kind = 'episode';
 
INSERT INTO Pelicula SELECT DISTINCT TITULO, ESTRENO, TIPO
	FROM Produccion
	WHERE TITULO IS NOT NULL AND ESTRENO IS NOT NULL AND TIPO IS NOT NULL AND TIPO = 'movie';
 
INSERT INTO Link SELECT DISTINCT title, production_year, titlelink, productionyearlink, link, kind
	FROM datosdb.datospeliculas dp
	WHERE dp.title IS NOT NULL
	AND dp.production_year IS NOT NULL 
	AND dp.kind = 'movie' 
	AND dp.productionyearlink IS NOT NULL
	AND dp.titlelink IS NOT NULL
	AND dp.link IS NOT NULL
	AND (dp.titlelink,dp.productionyearlink, 'movie') IN (SELECT TITULO,ESTRENO,TIPO FROM Pelicula);

INSERT INTO LugarNacimiento(LUGAR_NACIMIENTO) SELECT DISTINCT person_info
	FROM datosDB.datospeliculas
	WHERE info_context = 'birth notes';

CREATE SEQUENCE sec START WITH 1 INCREMENT BY 1;

CREATE VIEW PeliculaInfo AS
SELECT DISTINCT
    name,gender,role,title,production_year,kind
FROM datosDB.datospeliculas
WHERE name IS NOT NULL;


CREATE VIEW PersonaInfo AS
SELECT DISTINCT
    name,gender,person_info,role,title,production_year,kind,info_context
FROM datosDB.datospeliculas
WHERE info_context = 'birth notes';

INSERT INTO Persona
SELECT
    gender, person_info, name, sec.NEXTVAL
FROM (
    SELECT DISTINCT
        p.name, p.gender, pn.person_info
    FROM PeliculaInfo p
    LEFT JOIN PersonaInfo pn ON (p.name = pn.name AND p.gender = pn.gender AND p.role = pn.role AND p.title = pn.title AND p.production_year=pn.production_year AND p.kind=pn.kind)
);



DROP SEQUENCE sec;
DROP VIEW PeliculaInfo;
DROP VIEW PersonaInfo;

INSERT INTO Trabaja
SELECT
    role,
    ID,
    production_year,
    title,
    kind
FROM (
    SELECT DISTINCT
    dp.role,
    p.ID,
    dp.production_year,
    dp.title,
    dp.kind
    FROM datosDB.datospeliculas dp
    LEFT JOIN  PERSONA p ON ((p.NOMBRE = dp.name) AND (p.SEXO = dp.gender) AND ((p.LUGAR_NACIMIENTO = dp.person_info )OR (p.LUGAR_NACIMIENTO IS NULL AND dp.person_info IS NULL)))
    WHERE p.NOMBRE IS NOT NULL
);

INSERT INTO InfoPersonaje
SELECT
    role_name,
    nr_order,
    role,
    ID,
    production_year,
    title,
    kind
FROM (
    SELECT DISTINCT
    dp.role_name,
    dp.nr_order,
    dp.role,
    p.ID,
    dp.production_year,
    dp.title,
    dp.kind
    FROM datosDB.datospeliculas dp
    LEFT JOIN  PERSONA p ON ((p.NOMBRE = dp.name) AND (p.SEXO = dp.gender) AND ((p.LUGAR_NACIMIENTO = dp.person_info ) OR (p.LUGAR_NACIMIENTO IS NULL AND dp.person_info IS NULL)))
	WHERE p.NOMBRE IS NOT NULL AND (dp.role='actor' or dp.role='actress') AND dp.role_name IS NOT NULL
);















































--CREATE VIEW PersonaNull AS
--SELECT DISTINCT gender, person_info, info_context, name
--      FROM datosdb.datospeliculas
--      WHERE person_info IS NULL 
--      AND info_context IS NULL
--      AND name IS NOT NULL;

--CREATE VIEW PersonaParte AS
--SELECT DISTINCT gender, person_info, info_context, name
--      FROM datosdb.datospeliculas
--      WHERE info_context <> 'birth notes'
--      AND person_info IS NOT NULL
--      AND name IS NOT NULL;

--CREATE VIEW PersonaTodo AS
--SELECT DISTINCT gender, person_info, info_context, name
--    FROM datosdb.datospeliculas
--    WHERE info_context = 'birth notes' 
--    AND name IS NOT NULL;

--CREATE VIEW PersonaTotal AS
--SELECT PP.gender, PP.person_info, PP.info_context, PP.name
--FROM PersonaTodo PP
--UNION
--SELECT PN.gender, PN.person_info, PN.info_context, PN.name
--FROM PersonaParte PN
--LEFT JOIN PersonaTodo PP ON (PN.name = PP.name AND ((PN.gender = PP.gender)))
--WHERE PP.name IS NULL;


--CREATE VIEW PersonaFull AS
--SELECT PP.gender, PP.person_info, PP.info_context, PP.name
--FROM PersonaTotal PP
--UNION
--SELECT PN.gender, PN.person_info, PN.info_context, PN.name
--FROM PersonaNull PN
--LEFT JOIN PersonaTotal PP ON (PN.name = PP.name AND ((PN.gender = PP.gender)))
--WHERE PP.name IS NULL;



--INSERT INTO Persona 
--SELECT gender, person_info, info_context, name, sec.NEXTVAL
--FROM PersonaFull;

--CREATE VIEW Trabaja1 AS
--SELECT DISTINCT title, production_year, kind, name, gender, person_info, info_context, role
--      FROM datosdb.datospeliculas
--      WHERE info_context <> 'birth notes'
--      AND person_info IS NOT NULL
--      AND name IS NOT NULL;


--CREATE VIEW Trabaja1 AS
--SELECT  DISTINCT j.ID, p.title, p.production_year, p.kind, p.name, p.gender, p.person_info, p.info_context, p.role
--FROM datosdb.datospeliculas p
--JOIN persona j ON (p.name = j.NOMBRE AND p.gender = j.NOMBRE  AND p.person_info = j.NOMBRE  AND p.person_info = j.NOMBRE;
--WHERE PP.name IS NULL;


--CREATE TABLE Persona (
--   SEXO VARCHAR(2),
--    LUGAR_NACIMIENTO VARCHAR(150),
--    CONTEXTO_INFO_PERSONA VARCHAR(32),
--    NOMBRE VARCHAR(100),
--    ID CHAR(32),
--    PRIMARY KEY (ID) 
--);

--CREATE TABLE Trabaja (
--   PAPEL CHAR(32),
--    ID CHAR(32),
--    ESTRENO NUMBER,
--    TITULO VARCHAR(32), 
--    TIPO CHAR(32),
--    PRIMARY KEY (PAPEL,ID,ESTRENO,TITULO,TIPO), 
--    FOREIGN KEY (TITULO,ESTRENO,TIPO) REFERENCES Produccion(TITULO,ESTRENO,TIPO),
--    FOREIGN KEY (ID) REFERENCES Persona(ID)
--);












--CREATE VIEW PersonaTodo AS
--SELECT DISTINCT gender, person_info, info_context, name
 --     FROM datosdb.datospeliculas
 --     WHERE info_context = 'birth notes'
  --    AND name IS NOT NULL;


--CREATE VIEW PersonaFull AS
--SELECT PP.gender, PP.person_info, PP.info_context, PP.name
--FROM TotalCompleto PP
--UNION
--SELECT PN.gender, PN.person_info, PN.info_context, PN.name
--FROM PersonaNull PN
---LEFT JOIN TotalCompleto PP ON PN.name = PP.name
--WHERE PP.name IS NULL;





--SELECT *
--FROM PersonaParte
--LEFT JOIN PersonaNull ON PersonaParte.gender = PersonaNull.gender
--                        AND PersonaParte.person_info = PersonaNull.person_info
--                        AND PersonaParte.info_context = PersonaNull.info_context
--                        AND PersonaParte.name = PersonaNull.name;
--INSERT INTO Persona 
--SELECT gender, person_info, info_context, name, sec.NEXTVAL
--FROM PersonaParte
--INSERT INTO Persona
--SELECT gender, person_info, info_context, name, sec.NEXTVAL
--FROM datosdb.datospeliculas bd
--WHERE person_info IS NULL AND info_context IS NULL AND gender IS NULL;

--DELETE FROM Persona
--WHERE ID NOT IN (
 --   SELECT MIN(ID)
  --  FROM Persona
   -- GROUP BY SEXO, LUGAR_NACIMIENTO, CONTEXTO_INFO_PERSONA, NOMBRE
--);


--DELETE FROM Persona
--WHERE ID NOT IN (
 --   SELECT MIN(ID)
  --  FROM Persona
   -- GROUP BY SEXO, LUGAR_NACIMIENTO, CONTEXTO_INFO_PERSONA, NOMBRE
--);
