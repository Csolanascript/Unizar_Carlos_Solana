-- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Simón Gayán   --- 870984


-- Mejoras estéticas de muestreo de tablas por pantalla
set lines 256
set trimout on
set tab off

-- Vista que contiene todos los equipos que han participado en segunda división las últimas 10 temporadas
CREATE VIEW equiposSegunda
AS (SELECT DISTINCT INICIO_TEMPORADA, DIVISION, EQUIPO_LOCAL AS EQUIPO 
FROM PARTIDO 
WHERE DIVISION='2' AND TO_NUMBER(INICIO_TEMPORADA) >=2006);

-- Vista que contiene todos los equipos que han participado en primera división las últimas 10 temporadas
CREATE VIEW equiposPrimera
AS (SELECT DISTINCT INICIO_TEMPORADA, DIVISION, EQUIPO_LOCAL AS EQUIPO 
FROM PARTIDO 
WHERE DIVISION='1' AND TO_NUMBER(INICIO_TEMPORADA) >=2006);

-- Vista que contiene todos los equipos que han ascendido a primera división las últimas 10 temporadas
CREATE VIEW ascendidos
AS (SELECT *
FROM equiposSegunda 
WHERE EQUIPO IN ( SELECT EQUIPO 
FROM equiposPrimera 
WHERE TO_NUMBER(INICIO_TEMPORADA)=TO_NUMBER(equiposSegunda.INICIO_TEMPORADA)+1));

-- Vista que contiene todos los equipos que han descendido a segunda división las últimas 10 temporadas
CREATE VIEW descendidos
AS (SELECT *
FROM equiposPrimera 
WHERE EQUIPO IN ( SELECT EQUIPO 
FROM equiposSegunda
WHERE TO_NUMBER(INICIO_TEMPORADA)=TO_NUMBER(equiposPrimera.INICIO_TEMPORADA)+1));

-- Muestra por pantalla los equipos que hayan ascendido en una temporada y a la temporada siguiente hayan descendido
SELECT seg.EQUIPO AS EQUIPO
FROM equiposSegunda seg
WHERE seg.EQUIPO IN(SELECT ascen.EQUIPO 
                    FROM ascendidos ascen 
                    WHERE TO_NUMBER(seg.INICIO_TEMPORADA)=TO_NUMBER(ascen.INICIO_TEMPORADA))
AND seg.EQUIPO IN(SELECT descen.EQUIPO 
                    FROM descendidos descen 
                    WHERE TO_NUMBER(seg.INICIO_TEMPORADA)+1=TO_NUMBER(descen.INICIO_TEMPORADA));

-- Borrado de la vista equiposSegunda
DROP VIEW equiposSegunda;

-- Borrado de la vista equiposPrimera
DROP VIEW equiposPrimera;

-- Borrado de la vista ascendidos
DROP VIEW ascendidos;

-- Borrado de la vista descendidos
DROP VIEW descendidos;