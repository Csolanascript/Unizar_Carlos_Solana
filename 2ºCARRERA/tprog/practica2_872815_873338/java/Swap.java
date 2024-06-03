/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Swap.java
 */

import java.util.Stack;

public class Swap implements Instruccion {
    public int ejecuta(Stack<Integer> pila, int pc) {
        int num = pila.pop();
        int num2 = pila.pop();
        pila.push(num);
        pila.push(num2);
        pc++;
        return pc;
    }

    public void muestra() {
        System.out.print("swap");
    }
}
