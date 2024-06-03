/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Write.java
 */

import java.util.Stack;

class Write implements Instruccion
{
    public int ejecuta(Stack<Integer> pila, int pc) {
        int numero;
        numero = pila.pop();
        System.out.println(numero);
        pc++;
        return pc;
    }
    public void muestra() {
        System.out.print("write");
    }
}