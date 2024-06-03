/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Nodo.java
 */

public abstract class Nodo {
    protected String nombre;

    public Nodo(String nom)
    {
        nombre = nom;
    }

    public String nombre()
    {
        return nombre;
    }
    
    public abstract Integer tamano();
}