/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Main.java
 *  Comentario: Compilar mediante javac *.java
 *              Eliminar los .class mediante rm *.class
 */

public class Main
{
    public static void main(String[] args)
    {
        Programa1 p1 = new Programa1();
        Programa2 p2 = new Programa2();
        Programa3 p3 = new Programa3();
        System.out.println("Programa 1: ");
        p1.listar(4);
        System.out.println("Ejecucion: ");
        p1.ejecutar(4);
        System.out.println("Programa 2: ");
        p2.listar(7);
        System.out.println("Ejecucion: ");
        p2.ejecutar(7);
        System.out.println("Programa 3: ");
        p3.listar(14);
        System.out.println("Ejecucion: ");
        p3.ejecutar(14);
    }
}