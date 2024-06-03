/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : ExcepcionArbolFicheros.java
 */

import java.util.*;

// Clase base para excepciones relacionadas con el árbol de ficheros
class ExcepcionArbolFicheros extends Exception {
    protected String mensaje; // Mensaje de error asociado a la excepción

    // Constructor
    public ExcepcionArbolFicheros(String mensaje) {
        this.mensaje = mensaje;
    }

    // Obtener el mensaje de error
    public String getMessage() {
        return mensaje;
    }
}