/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Instruccion.java
 */

import java.util.Stack;

public interface Instruccion
{
    public abstract void muestra();
    public abstract int ejecuta(Stack<Integer> pila, int pc);
}