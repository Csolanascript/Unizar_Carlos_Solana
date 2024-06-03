-- Crear una vista consolidada para actores con nacionalidad y rango de años específico


CREATE VIEW actoresNacionalidad AS
SELECT 
    p.NOMBRE AS NOM_ACTOR, 
    t.ID as ID_ACTOR, 
    t.TITULO, 
    t.ESTRENO, 
    ln.NACIONALIDAD AS NACION_ACTOR
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
JOIN LugarNacimiento ln ON p.LUGAR_NACIMIENTO = ln.LUGAR_NACIMIENTO
WHERE t.PAPEL = 'actor' AND t.TIPO = 'movie' AND t.ESTRENO BETWEEN 1980 AND 2010;

-- Crear una vista consolidada para actrices con nacionalidad y rango de años específico
CREATE VIEW actricesNacionalidad AS
SELECT 
    p.NOMBRE AS NOM_ACTRIZ, 
    t.ID AS ID_ACTRIZ, 
    t.TITULO, 
    t.ESTRENO, 
    ln.NACIONALIDAD AS NACION_ACTRIZ
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
JOIN LugarNacimiento ln ON p.LUGAR_NACIMIENTO = ln.LUGAR_NACIMIENTO
WHERE t.PAPEL = 'actress' AND t.TIPO = 'movie' AND t.ESTRENO BETWEEN 1980 AND 2010;

CREATE VIEW parejasActores AS
SELECT DISTINCT 
    a.NOM_ACTOR, a.ID_ACTOR, a.NACION_ACTOR,
    b.NOM_ACTRIZ, b.ID_ACTRIZ, b.NACION_ACTRIZ,
    COUNT(*) AS NUM_OBRAS
FROM actoresNacionalidad a
JOIN actricesNacionalidad b ON a.TITULO = b.TITULO AND a.ESTRENO = b.ESTRENO AND a.NACION_ACTOR <> b.NACION_ACTRIZ
GROUP BY a.NOM_ACTOR, a.ID_ACTOR, a.NACION_ACTOR, b.NOM_ACTRIZ, b.ID_ACTRIZ, b.NACION_ACTRIZ
HAVING COUNT(*) = (SELECT COUNT(*) FROM actoresNacionalidad WHERE ID_ACTOR = a.ID_ACTOR)
AND COUNT(*) = (SELECT COUNT(*) FROM actricesNacionalidad WHERE ID_ACTRIZ = b.ID_ACTRIZ)
ORDER BY NUM_OBRAS ASC;

Select * from parejasActores;

EXPLAIN PLAN FOR SELECT count(*) from parejasActores;

SELECT PLAN_TABLE_OUTPUT FROM TABLE(DBMS_XPLAN.DISPLAY());


DROP VIEW actoresNacionalidad;
DROP VIEW actricesNacionalidad;
DROP VIEW parejasActores;
