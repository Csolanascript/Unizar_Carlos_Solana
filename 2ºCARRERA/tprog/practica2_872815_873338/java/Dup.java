/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Dup.java
 */

import java.util.Stack;

class Dup implements Instruccion
{
    public int ejecuta(Stack<Integer> pila, int pc) {
        int num;
        num = pila.peek();
        pila.push(num);
        pc++;
        return pc;
    }
    public void muestra() {
        System.out.print("dup");
    }
}

