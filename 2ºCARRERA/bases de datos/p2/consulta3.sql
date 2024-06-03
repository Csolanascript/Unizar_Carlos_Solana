column NOM_ACTOR format a25
column NOM_ACTRIZ format a25
column ID_ACTOR format a6
column ID_ACTRIZ format a6
COLUMN NACION_ACTOR format a15
COLUMN NACION_ACTRIZ format a15

--Creacion de la tabla intermedia actores
CREATE VIEW actores
AS SELECT p.NOMBRE AS NOM_ACTOR, t.ID as ID_actor, t.TITULO, t.ESTRENO, p.LUGAR_NACIMIENTO
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
WHERE t.PAPEL = 'actor' AND t.TIPO = 'movie';

--Creacion de la tabla intermedia actoresNacionalidad
CREATE VIEW actoresNacionalidad
AS SELECT t.NOM_ACTOR, t.ID_actor, t.TITULO, t.ESTRENO, p.NACIONALIDAD AS NACION_ACTOR
FROM actores t
JOIN LugarNacimiento p ON t.LUGAR_NACIMIENTO = p.LUGAR_NACIMIENTO
WHERE ESTRENO >= 1980 AND ESTRENO <=2010;

--Creacion de la tabla intermedia actrices
CREATE VIEW actrices
AS SELECT p.NOMBRE AS NOM_ACTRIZ, t.ID AS ID_ACTRIZ, t.TITULO, t.ESTRENO, p.LUGAR_NACIMIENTO
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
WHERE t.PAPEL = 'actress' AND t.TIPO = 'movie';

--Creacion de la tabla intermedia actricesNacionalidad
CREATE VIEW actricesNacionalidad
AS SELECT t.NOM_ACTRIZ, t.ID_ACTRIZ, t.TITULO, t.ESTRENO, p.NACIONALIDAD AS NACION_ACTRIZ
FROM actrices t
JOIN LugarNacimiento p ON t.LUGAR_NACIMIENTO = p.LUGAR_NACIMIENTO
WHERE ESTRENO >= 1980 AND ESTRENO <=2010;

--Creacion de la tabla intermedia contadorActores
CREATE VIEW contadorActores
AS SELECT NOM_ACTOR, ID_actor, NACION_ACTOR, COUNT(*) AS NUM_OBRAS
FROM actoresNacionalidad
GROUP BY NOM_ACTOR, ID_actor, NACION_ACTOR;

--Creacion de la tabla intermedia contadorActrices
CREATE VIEW contadorActrices
AS SELECT NOM_ACTRIZ, ID_actriz, NACION_ACTRIZ, COUNT(*) AS NUM_OBRAS
FROM actricesNacionalidad
GROUP BY NOM_ACTRIZ, ID_actriz, NACION_ACTRIZ;

--Creacion de la tabla intermedia mismoNumObras
CREATE VIEW mismoNumObras
AS SELECT a.NOM_ACTOR, a.ID_actor, a.NACION_ACTOR, b.NOM_ACTRIZ,  b.ID_actriz, b.NACION_ACTRIZ, a.NUM_OBRAS
FROM contadorActores a, contadorActrices b
WHERE a.NUM_OBRAS = b.NUM_OBRAS;

--Creacion de la tabla intermedia coincidenciasActores
CREATE VIEW coincidenciasActores
AS SELECT a.NOM_ACTOR, a.ID_actor, a.NACION_ACTOR , b.NOM_ACTRIZ, b.ID_actriz, b.NACION_ACTRIZ, a.TITULO, a.ESTRENO
FROM actoresNacionalidad a, actricesNacionalidad b
WHERE b.TITULO = a.TITULO;

--Creacion de la tabla intermedia numCoincidenciasActores
CREATE VIEW numCoincidenciasActores
AS SELECT NOM_ACTOR, ID_ACTOR, NACION_ACTOR, NOM_ACTRIZ, ID_ACTRIZ, NACION_ACTRIZ, COUNT(*) AS NUM_COINCIDENCIAS
FROM coincidenciasActores
GROUP BY NOM_ACTOR, ID_ACTOR, NACION_ACTOR,NOM_ACTRIZ, ID_ACTRIZ, NACION_ACTRIZ;

--Creacion de la tabla intermedia parejasActores
CREATE VIEW parejasActores
AS SELECT DISTINCT n.NOM_ACTOR, n.ID_ACTOR, n.NACION_ACTOR, n.NOM_ACTRIZ, n.ID_ACTRIZ, n.NACION_ACTRIZ, m.NUM_OBRAS
FROM mismoNumObras m, numCoincidenciasActores n
WHERE m.NUM_OBRAS = n.NUM_COINCIDENCIAS AND n.ID_ACTOR = m.ID_ACTOR AND n.ID_ACTRIZ = m.ID_ACTRIZ AND (n.NACION_ACTOR != n.NACION_ACTRIZ OR n.NACION_ACTOR IS NULL OR n.NACION_ACTRIZ IS NULL)
ORDER BY m.NUM_OBRAS desc;

--Seleccion de todas las parejas que cumplan todas las condiciones de las vistas
SELECT * FROM parejasActores;

EXPLAIN PLAN FOR SELECT count(*) from parejasActores;

SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());

DROP VIEW actores;
DROP VIEW actoresNacionalidad;
DROP VIEW actrices;
DROP VIEW actricesNacionalidad;
DROP VIEW contadorActores;
DROP VIEW contadorActrices;
DROP VIEW mismoNumObras;
DROP VIEW coincidenciasActores;
DROP VIEW numCoincidenciasActores;
DROP VIEW parejasActores;