column titulo format a40

--Creacion de la tabla intermedia secuelas
CREATE VIEW secuelas
AS SELECT TITULO_LINK, ESTRENO_LINK, TITULO, ESTRENO
FROM Link
WHERE LINK = 'follows' AND ESTRENO > ESTRENO_LINK;

--Creacion de la tabla intermedia numSecuelas
CREATE VIEW numSecuelas
AS SELECT TITULO_LINK, ESTRENO_LINK, COUNT(*) AS NUM_SECUELAS
FROM secuelas
GROUP BY TITULO_LINK, ESTRENO_LINK;

--Creacion de la tabla intermedia maxSecuelas
CREATE VIEW maxSecuelas
AS SELECT TITULO_LINK AS TITULO, n.ESTRENO_LINK AS ESTRENO, n.NUM_SECUELAS AS NUMERO_DE_SECUELAS
FROM numSecuelas n
WHERE n.NUM_SECUELAS IN (SELECT MAX(NUM_SECUELAS) FROM numSecuelas s);

--Seleccian de la obra con mas secuelas, mostrando el numero de ellas
SELECT * FROM maxSecuelas;

--Seleccion de todas las peliculas que son secuela de la pelicula de la seleccion anterior
SELECT DISTINCT s.TITULO, s.ESTRENO
FROM maxSecuelas m, secuelas s
WHERE s.TITULO_LINK = m.TITULO
ORDER BY s.ESTRENO ASC;

EXPLAIN PLAN FOR SELECT DISTINCT s.TITULO, s.ESTRENO
FROM maxSecuelas m, secuelas s
WHERE s.TITULO_LINK = m.TITULO
ORDER BY s.ESTRENO ASC;

SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());

DROP VIEW secuelas;
DROP VIEW numSecuelas;
DROP VIEW maxSecuelas;


