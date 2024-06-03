/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : YaExisteNodo.java
 */

class YaExisteNodo extends ExcepcionArbolFicheros {
    public YaExisteNodo() {
        super("ya existe un nodo con ese nombre");
    }
}