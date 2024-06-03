CREATE OR REPLACE TRIGGER Before_Insert_LugarNacimiento
BEFORE INSERT ON LugarNacimiento
FOR EACH ROW
DECLARE
    last_comma_pos INTEGER;
    nationality_part VARCHAR2(150);
BEGIN
    last_comma_pos := INSTR(:NEW.LUGAR_NACIMIENTO, ',', -1, 1);
    IF last_comma_pos > 0 THEN
        nationality_part := SUBSTR(:NEW.LUGAR_NACIMIENTO, last_comma_pos + 1);
        :NEW.NACIONALIDAD := TRIM(nationality_part);
    ELSE
        :NEW.NACIONALIDAD := :NEW.LUGAR_NACIMIENTO;  -- O maneja de otra manera si no hay coma
    END IF;
END;
/

