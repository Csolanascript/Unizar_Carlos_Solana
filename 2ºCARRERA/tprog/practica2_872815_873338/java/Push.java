/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Push.java
 */

import java.util.Stack;

class Push implements Instruccion
{
    private int num;
    public Push(int n) {
        num = n;
    }
    public int ejecuta(Stack<Integer> pila, int pc) {
       pila.push(num);
       pc++;
       return pc;
    }
    public void muestra() {
        System.out.print("push " + num);
    }
}