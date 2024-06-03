-- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Sim�n Gay�n   --- 870984


-- Mejoras est�ticas de muestreo de tablas por pantalla
set lines 256
set trimout on
set tab off

-- Vista que contiene todas las jornadas de cada divisi�n de cada temporada con su n�mero de goles
CREATE VIEW goles_jornada
AS SELECT INICIO_TEMPORADA, JORNADA, DIVISION, SUM(GOLES_LOCAL + GOLES_VISITANTE) AS TOTAL_GOLES
FROM Partido
WHERE TO_NUMBER(INICIO_TEMPORADA) >= 2011
GROUP BY INICIO_TEMPORADA, JORNADA, DIVISION;

-- Muestra por pantalla la/las jornada/s de una/varias divisi�n/ones con el mayor n�mero de goles
SELECT INICIO_TEMPORADA, JORNADA, DIVISION, TOTAL_GOLES
FROM goles_jornada gol
WHERE gol.TOTAL_GOLES = (SELECT MAX(TOTAL_GOLES) FROM goles_jornada);

-- Borrado de la vista goles_jornada
DROP VIEW goles_jornada;