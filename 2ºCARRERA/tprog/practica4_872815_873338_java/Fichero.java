/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Fichero.java
 */

public class Fichero extends Nodo {
    protected Integer tamano;
    public Fichero(String nom, Integer tam)
    {
        super(nom);
        tamano = tam;
    }
    @Override
    public Integer tamano() {
        return tamano;
    }
    public void setTamano(int tamano) {
        this.tamano = tamano;
    }
}