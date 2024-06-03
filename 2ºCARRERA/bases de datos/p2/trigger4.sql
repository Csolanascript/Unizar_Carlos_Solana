CREATE OR REPLACE TRIGGER Delete_Cascade_Produccion
BEFORE DELETE ON Produccion
FOR EACH ROW
DECLARE
    v_count_link INT;
    v_count_capitulo INT;
BEGIN


    IF (:OLD.TIPO = 'movie') THEN
        -- Verificar si la película está siendo referenciada en la tabla Link
        SELECT COUNT(*)
        INTO v_count_link
        FROM Link
        WHERE TITULO_LINK = :OLD.TITULO AND ESTRENO_LINK = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        IF v_count_link > 0 THEN
            RAISE_APPLICATION_ERROR(-20002, 'No se puede borrar la película porque es referenciada en otra película.');
        END IF;

        DELETE FROM InfoPersonaje
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Trabaja
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Genero
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Link
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Pelicula
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;
    END IF;

    IF (:OLD.TIPO = 'episode') THEN
        DELETE FROM InfoPersonaje
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Trabaja
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Genero
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Capitulo
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;
    END IF;

    IF (:OLD.TIPO = 'tv series') THEN
        -- Verificar si la serie tiene capítulos referenciados
        SELECT COUNT(*)
        INTO v_count_capitulo
        FROM Capitulo
        WHERE TITULO_SERIE = :OLD.TITULO AND ESTRENO_SERIE = :OLD.ESTRENO AND TIPO_SERIE = :OLD.TIPO;

        IF v_count_capitulo > 0 THEN
            RAISE_APPLICATION_ERROR(-20003, 'No se puede borrar la serie mientras tenga capítulos referenciados.');
        END IF;

        DELETE FROM InfoPersonaje
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Trabaja
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Genero
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;

        DELETE FROM Capitulo
        WHERE TITULO_SERIE = :OLD.TITULO AND ESTRENO_SERIE = :OLD.ESTRENO AND TIPO_SERIE = :OLD.TIPO;

        DELETE FROM Serie
        WHERE TITULO = :OLD.TITULO AND ESTRENO = :OLD.ESTRENO AND TIPO = :OLD.TIPO;
    END IF;

END;
/
