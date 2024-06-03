/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Programa.java
 */

import java.util.Stack;

public class Programa {
    protected Instruccion[] miVector;
    private int pc;

    public Programa() {
        pc = 0;
    }

    public void listar(int size) {
        for (int i = 0; i < size; i++) {
            System.out.print(i + "   ");
            miVector[i].muestra();
            System.out.println("");
        }
    }

    public void ejecutar(int size) {
        Stack<Integer> pila = new Stack<>();
        while (pc < size) {
            pc = miVector[pc].ejecuta(pila, pc);
        }
    }
}
