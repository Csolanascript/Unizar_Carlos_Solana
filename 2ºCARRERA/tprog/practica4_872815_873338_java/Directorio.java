/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Directorio.java
 */

import java.util.ArrayList;
import java.util.*;

public class Directorio extends Nodo{

    public TreeMap<String, Nodo> contenido = new TreeMap<>();

    public Directorio(String nom)
    {
        super(nom);
    }
    @Override
    public Integer tamano()
    {
        Integer size = 0;
        for (Map.Entry<String, Nodo> arch : contenido.entrySet())
        {   
            size = size + arch.getValue().tamano();
        }
        return size;
    }
}