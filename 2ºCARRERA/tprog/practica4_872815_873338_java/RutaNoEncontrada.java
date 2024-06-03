/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : RutaNoEncontrada.java
 */

class RutaNoEncontrada extends ExcepcionArbolFicheros {
    public RutaNoEncontrada() {
        super("La ruta no pertenece al shell");
    }
}