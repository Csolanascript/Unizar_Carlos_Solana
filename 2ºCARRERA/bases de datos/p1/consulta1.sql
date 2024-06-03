-- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Simón Gayán   --- 870984


-- Mejoras estéticas de muestreo de tablas por pantalla
set lines 256
set trimout on
set tab off

-- Vista que contiene todos los partidos de primera división
CREATE VIEW partidos_primera_div
AS SELECT INICIO_TEMPORADA, JORNADA, EQUIPO_LOCAL, EQUIPO_VISITANTE, GOLES_LOCAL, GOLES_VISITANTE 
    FROM PARTIDO 
    WHERE DIVISION='1' 
    ORDER BY INICIO_TEMPORADA DESC;


-- Vista que contiene todas las victorias de cada equipo en cada temporada
CREATE VIEW victorias 
AS( SELECT INICIO_TEMPORADA, EQUIPO_LOCAL AS EQUIPO FROM partidos_primera_div WHERE GOLES_LOCAL>GOLES_VISITANTE
    UNION ALL 
    SELECT INICIO_TEMPORADA, EQUIPO_VISITANTE AS EQUIPO FROM partidos_primera_div WHERE GOLES_VISITANTE>GOLES_LOCAL);

-- Vista que contiene todas los empates de cada equipo en cada temporada    
CREATE VIEW empatados 
AS( SELECT INICIO_TEMPORADA, EQUIPO_LOCAL AS EQUIPO FROM partidos_primera_div WHERE GOLES_LOCAL=GOLES_VISITANTE
    UNION ALL 
    SELECT INICIO_TEMPORADA, EQUIPO_VISITANTE AS EQUIPO FROM partidos_primera_div WHERE GOLES_VISITANTE=GOLES_LOCAL);

-- Vista que contiene los puntos de cada equipo
CREATE VIEW puntos
AS  SELECT INICIO_TEMPORADA, EQUIPO, COUNT(*) AS PTOS 
    FROM empatados
    GROUP BY INICIO_TEMPORADA,EQUIPO
    UNION ALL
    SELECT INICIO_TEMPORADA, EQUIPO, 3*COUNT(*) AS PTOS
    FROM victorias
    GROUP BY INICIO_TEMPORADA, EQUIPO;
    
-- Vista que contiene los puntos de cada equipo en cada temporada    
CREATE VIEW puntos_por_temporada
AS SELECT INICIO_TEMPORADA, EQUIPO, SUM(PTOS) as PUNTOSTOTALES
    FROM puntos
    GROUP BY INICIO_TEMPORADA, EQUIPO
    ORDER BY PUNTOSTOTALES ASC;

-- Vista que contiene el número máximo de puntos en cada temporada    
CREATE VIEW max_puntos_temporada
AS SELECT INICIO_TEMPORADA, MAX(PUNTOSTOTALES) AS PUNTOS
    FROM puntos_por_temporada
    GROUP BY INICIO_TEMPORADA;

-- Vista que contiene el/los equipos ganadores de la liga en cada temporada    
CREATE VIEW ganadores_ligas
AS SELECT INICIO_TEMPORADA, EQUIPO, PUNTOSTOTALES
FROM puntos_por_temporada pts
WHERE pts.PUNTOSTOTALES=(SELECT PUNTOS FROM max_puntos_temporada max WHERE pts.INICIO_TEMPORADA=max.INICIO_TEMPORADA);

-- Vista que contiene el número de ligas de cada equipo
CREATE VIEW num_ligas
AS SELECT EQUIPO, COUNT(*) AS NUM_LIGAS
FROM ganadores_ligas
GROUP BY EQUIPO;

-- Muestra por pantalla el/los equipos con el mayor número de ligas
SELECT EQUIPO, NUM_LIGAS
FROM num_ligas nli
WHERE nli.NUM_LIGAS=(SELECT MAX(NUM_LIGAS) FROM num_ligas);


-- Borrado de la vista partidos_primera_div
DROP VIEW partidos_primera_div;

-- Borrado de la vista victorias
DROP VIEW victorias;

-- Borrado de la vista empatados
DROP VIEW empatados;

-- Borrado de la vista puntos
DROP VIEW puntos;

-- Borrado de la vista puntos_por_temporada
DROP VIEW puntos_por_temporada;

-- Borrado de la vista max_puntos_temporada
DROP VIEW max_puntos_temporada;

-- Borrado de la vista ganadores_ligas
DROP VIEW ganadores_ligas;

-- Borrado de la vista num_ligas
DROP VIEW num_ligas;