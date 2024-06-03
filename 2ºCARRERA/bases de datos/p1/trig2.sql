-- Asignatura: Bases de Datos
-- Autores: Diego Mateo Lorente  --- 873338
--          Carlos Solana Melero --- 872815
--          Daniel Simón Gayán   --- 870984


-- Trigger para actualizar la tabla Nombres borrando los nombres asociados al equipo que borramos de la tabla Equipo
CREATE OR REPLACE TRIGGER trigger_borrar_nombres
BEFORE DELETE ON Equipo
FOR EACH ROW
BEGIN
    DELETE FROM Nombres WHERE NOMBRE_EQUIPO = :OLD.NOMBRE_EQUIPO;
END;
/