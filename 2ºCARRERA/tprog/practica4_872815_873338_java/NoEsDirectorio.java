/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : NoEsDirectorio.java
 */

class NoEsDirectorio extends ExcepcionArbolFicheros {
    public NoEsDirectorio() {
        super("el nodo no es un directorio y debería serlo");
    }
}