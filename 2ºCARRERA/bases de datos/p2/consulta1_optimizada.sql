column ID format a6
column NOMBRE format a30

-- Crear una vista materializada para la última obra de cada director
CREATE VIEW directoresUltimaObra AS
SELECT NOMBRE, ID, TITULO, ESTRENO
FROM directorM d
WHERE ESTRENO IN (SELECT MAX(ESTRENO) FROM directorM t WHERE t.ID = d.ID);

CREATE VIEW actores
AS SELECT p.NOMBRE, t.ID, t.TITULO, t.ESTRENO
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
WHERE (t.PAPEL = 'actor' OR t.PAPEL = 'actress') AND t.TIPO = 'movie';

-- Crear una vista para la última obra de cada actor y actriz
CREATE VIEW actoresUltimaObra AS
SELECT a.NOMBRE, a.ID, a.TITULO, a.ESTRENO
FROM actores a
WHERE ESTRENO IN (SELECT MAX(ESTRENO) FROM actores t WHERE t.ID = a.ID);

SELECT DISTINCT a.ID, a.NOMBRE
FROM actoresUltimaObra a, directoresUltimaObra d
WHERE a.ID = d.ID AND a.ESTRENO>=d.ESTRENO;


EXPLAIN PLAN FOR SELECT DISTINCT a.ID, a.NOMBRE
FROM actoresUltimaObra a, directoresUltimaObra d
WHERE a.ID = d.ID AND a.ESTRENO>=d.ESTRENO;


SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());

DROP VIEW directoresUltimaObra;
DROP VIEW actores;
DROP VIEW actoresUltimaObra;
