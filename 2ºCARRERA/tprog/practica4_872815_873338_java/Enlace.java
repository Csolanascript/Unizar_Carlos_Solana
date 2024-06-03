/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Enlace.java
 */

public class Enlace extends Nodo {
    protected Nodo enlace;

    public Enlace(String nom, Nodo _enlace)
    {
        super(nom);
        this.enlace = _enlace;
    }
    @Override
    public Integer tamano()
    {
        return enlace.tamano();
    }
    public String nombre_enlace()
    {
        return enlace.nombre();
    }

    public Nodo link()
    {
        return enlace;
    }
} 