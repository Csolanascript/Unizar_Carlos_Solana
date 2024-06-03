/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Mul.java
 */

import java.util.Stack;

public class Mul implements Instruccion {
    public int ejecuta(Stack<Integer> pila, int pc) {
        int num1,suma;
        suma = pila.pop();
        suma = suma * pila.pop();
        pila.push(suma);
        pc++;
        return pc;
    }
    public void muestra() {
    System.out.print("mul");
    }
};

