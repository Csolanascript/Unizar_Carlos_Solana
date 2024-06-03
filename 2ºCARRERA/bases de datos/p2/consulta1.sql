column ID format a6
column NOMBRE format a30

--Creación de la tabla intermedia directores
CREATE VIEW directores
AS SELECT p.NOMBRE, t.ID, t.TITULO, t.ESTRENO
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
WHERE t.PAPEL = 'director' AND t.TIPO = 'movie';

--Creación de la tabla intermedia directoresUltimaObra
CREATE VIEW directoresUltimaObra
AS SELECT NOMBRE, ID, TITULO, ESTRENO
FROM directores d
WHERE ESTRENO IN (SELECT MAX(ESTRENO) FROM directores t WHERE t.ID = d.ID);

--Creación de la tabla intermedia actores
CREATE VIEW actores
AS SELECT p.NOMBRE, t.ID, t.TITULO, t.ESTRENO
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
WHERE (t.PAPEL = 'actor' OR t.PAPEL = 'actress') AND t.TIPO = 'movie';

--Creación de la tabla intermedia actoresUltimaObra
CREATE VIEW actoresUltimaObra
AS SELECT a.NOMBRE, a.ID, a.TITULO, a.ESTRENO
FROM actores a
WHERE ESTRENO IN (SELECT MAX(ESTRENO) FROM actores t WHERE t.ID = a.ID);

--Selección de directores cuya última obra ha sido como actor
SELECT DISTINCT a.ID, a.NOMBRE
FROM actoresUltimaObra a, directoresUltimaObra d
WHERE a.ID = d.ID AND a.ESTRENO>=d.ESTRENO;

EXPLAIN PLAN FOR SELECT DISTINCT a.ID, a.NOMBRE
FROM actoresUltimaObra a, directoresUltimaObra d
WHERE a.ID = d.ID AND a.ESTRENO>=d.ESTRENO;

SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());

DROP VIEW directores;
DROP VIEW directoresUltimaObra;
DROP VIEW actores;
DROP VIEW actoresUltimaObra;
