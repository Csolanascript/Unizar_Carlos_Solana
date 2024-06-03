--CREATE MATERIALIZED VIEW actoresM 
--REFRESH ON COMMIT
--AS 
--   SELECT
--        p.NOMBRE AS NOM_ACTOR, 
--        t.ID as ID_ACTOR, 
--        t.TITULO, 
--        t.ESTRENO, 
---        ln.NACIONALIDAD AS NACION_ACTOR,
--        t.PAPEL
--    FROM Trabaja t
 --   JOIN Persona p ON t.ID = p.ID
 --   JOIN LugarNacimiento ln ON p.LUGAR_NACIMIENTO = ln.LUGAR_NACIMIENTO
--    WHERE t.TIPO = 'movie' AND t.ESTRENO BETWEEN 1980 AND 2010;

CREATE MATERIALIZED VIEW directorM 
REFRESH ON COMMIT
AS SELECT p.NOMBRE, t.ID, t.TITULO, t.ESTRENO
FROM Trabaja t
JOIN Persona p ON t.ID = p.ID
WHERE (t.PAPEL = 'director') AND t.TIPO = 'movie';


CREATE MATERIALIZED VIEW secuelasM
REFRESH ON COMMIT
AS SELECT TITULO_LINK, ESTRENO_LINK, TITULO, ESTRENO
FROM Link
WHERE LINK = 'follows' AND ESTRENO > ESTRENO_LINK;


CREATE INDEX idx_directorM_ID_ESTRENO ON directorM (ID, ESTRENO);
CREATE INDEX idx_secuelasM_Link ON secuelasM (TITULO_LINK, ESTRENO_LINK);  


CREATE INDEX idx_trabaja_titulo_estreno_tipo ON Trabaja(TITULO, ESTRENO, TIPO);

CREATE INDEX idx_persona_on_birthplace ON Persona (LUGAR_NACIMIENTO);

CREATE BITMAP INDEX idx_tipo_trabaja ON Trabaja(TIPO);
CREATE BITMAP INDEX idx_tipo_produccion ON Produccion(TIPO);
CREATE BITMAP INDEX idx_papel_trabaja ON Trabaja(PAPEL);

-- Eliminando las columnas no particionadas del índice
CREATE INDEX idx_pelicula_global ON Pelicula (ESTRENO)
GLOBAL PARTITION BY RANGE (ESTRENO) (
    PARTITION p_before_1980 VALUES LESS THAN (1980),
    PARTITION p_1980_to_2010 VALUES LESS THAN (2011),
    PARTITION p_after_2010 VALUES LESS THAN (MAXVALUE)
);

--CREATE BITMAP INDEX id_pareja ON Trabaja (Persona.ID)
--FROM Trabaja, Persona
--WHERE Trabaja.ID = Persona.ID


--CREATE CLUSTER persona_trabaja_hash_cluster (num number)
--HASHKEYS 1000;