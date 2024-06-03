/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : EnlaceNoEsDirectorio.java
 */

class EnlaceNoEsDirectorio extends ExcepcionArbolFicheros {
    public EnlaceNoEsDirectorio() {
        super("el nodo al que apunta el enlace no es un directorio y debería serlo");
    }
}