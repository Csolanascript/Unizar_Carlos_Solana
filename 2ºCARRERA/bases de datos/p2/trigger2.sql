CREATE OR REPLACE TRIGGER CheckGenderActress
BEFORE INSERT OR UPDATE ON Trabaja
FOR EACH ROW
DECLARE
    v_gender VARCHAR2(2);
    v_count INT;
BEGIN
    IF :NEW.PAPEL = 'actress' THEN
        -- Obtener el género de la persona correspondiente
        SELECT SEXO INTO v_gender FROM Persona WHERE ID = :NEW.ID;

        -- Corregir el género si está vacío o incorrecto
        IF v_gender IS NULL THEN
            UPDATE Persona
                SET SEXO = 'f'
                WHERE ID = :NEW.ID;
        END IF;
    END IF;
END;
/

