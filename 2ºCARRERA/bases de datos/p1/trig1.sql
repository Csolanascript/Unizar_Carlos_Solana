-- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Simón Gayán   --- 870984


-- Trigger para verificar la restricción de fechas en la tabla Temporada
CREATE OR REPLACE TRIGGER trigger_fecha_temporada
BEFORE INSERT OR UPDATE ON Temporada
FOR EACH ROW
DECLARE
BEGIN
    IF :NEW.INICIO_TEMPORADA >= :NEW.FIN_TEMPORADA THEN
        RAISE_APPLICATION_ERROR(-20001, 'La fecha de inicio no puede ser posterior o igual a la fecha de fin de temporada.');
    END IF;
END;
/