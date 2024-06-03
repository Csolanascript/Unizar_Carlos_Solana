/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Jumpif.java
 */

import java.util.Stack;

class Jumpif implements Instruccion
{
    private int l;
    public Jumpif(int n) {
        l = n;
    }
    public int ejecuta(Stack<Integer> pila, int pc) {
        int numero;
        numero = pila.pop();
        if(numero >= 0) {
            pc = l;
        }
        else {
            pc++;
        }
        return pc;
    }
    public void muestra() {
        System.out.print("jumpif " + l);
    }
}