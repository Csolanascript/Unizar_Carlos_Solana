
DROP TYPE episode_table;
DROP TYPE episode_record;

CREATE OR REPLACE TYPE episode_record AS OBJECT (
    serie VARCHAR(100),
    temporada INT,
    numero_capitulo INT
);
/

-- Creación del tipo de tabla basada en el tipo de objeto
CREATE OR REPLACE TYPE episode_table IS TABLE OF episode_record;
/

CREATE OR REPLACE TRIGGER PreventDuplicateEpisode
FOR INSERT ON Capitulo
COMPOUND TRIGGER
    episodes episode_table;

    BEFORE STATEMENT IS 
    BEGIN
        -- Recolecta todos los episodios existentes en una colección.
        SELECT episode_record(TITULO_SERIE, TEMPORADA, NUMERO_CAPITULO) BULK COLLECT
        INTO episodes
        FROM Capitulo;
    END BEFORE STATEMENT;

    BEFORE EACH ROW IS
    BEGIN
        -- Cuenta cuántas veces aparece un episodio con el mismo número, serie y temporada.
        DECLARE
            num_episodes NUMBER;
        BEGIN
            IF :NEW.TEMPORADA IS NOT NULL AND :NEW.NUMERO_CAPITULO IS NOT NULL THEN
                SELECT COUNT(*)
                INTO num_episodes
                FROM TABLE(episodes)
                WHERE serie = :NEW.TITULO_SERIE
                AND temporada = :NEW.TEMPORADA
                AND numero_capitulo = :NEW.NUMERO_CAPITULO; -- si son nulos no se incluyen en el recuento

                -- Si ya existe al menos uno, se lanza un error.
                IF num_episodes > 0 THEN
                    RAISE_APPLICATION_ERROR(-20001, 'Ya existe un episodio con este número en la temporada.');
                END IF;
            END IF;
        END;
    END BEFORE EACH ROW;
END PreventDuplicateEpisode;
/
