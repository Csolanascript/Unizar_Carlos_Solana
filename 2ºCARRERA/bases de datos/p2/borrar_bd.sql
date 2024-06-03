
-- Borrar las tablas
DROP TABLE Link;
DROP TABLE InfoPersonaje;
DROP TABLE Trabaja;
DROP TABLE Persona;
DROP TABLE LugarNacimiento;
DROP TABLE Capitulo;
DROP TABLE Serie;
DROP TABLE Pelicula;
DROP TABLE Genero;
DROP TABLE Produccion;


DROP INDEX idx_directorM_ID_ESTRENO;
DROP INDEX idx_secuelasM_Link;
DROP MATERIALIZED VIEW directorM;
DROP MATERIALIZED VIEW secuelasM;