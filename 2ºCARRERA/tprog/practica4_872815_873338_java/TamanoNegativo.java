/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : TamanoNegativo.java
 */

class TamanoNegativo extends ExcepcionArbolFicheros {
    public TamanoNegativo() {
        super("el nodo no puede tener tamaño negativo");
    }
}