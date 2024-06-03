-- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Simón Gayán   --- 870984


-- Trigger para verificar la restricción de nombres de equipos en la tabla Partido
CREATE OR REPLACE TRIGGER before_insert_partido
BEFORE INSERT ON Partido
FOR EACH ROW
BEGIN
    IF :NEW.EQUIPO_LOCAL = :NEW.EQUIPO_VISITANTE THEN
        RAISE_APPLICATION_ERROR(-20002, 'Los nombres de los equipos local y visitante no pueden ser iguales.');
    END IF;
END;
/
