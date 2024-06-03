/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Read.java
 */

import java.util.Stack;
import java.util.Scanner;

class Read implements Instruccion
{
    public int ejecuta(Stack<Integer> pila, int pc) {
        int numero;
        Scanner scanner = new Scanner(System.in);
        System.out.print("? ");
        numero = scanner.nextInt();
        pila.push(numero);
        pc++;
        return pc;
    }
    public void muestra() {
        System.out.print("read");
    }
}