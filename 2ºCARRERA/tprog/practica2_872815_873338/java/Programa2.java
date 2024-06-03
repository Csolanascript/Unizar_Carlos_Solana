/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Programa2.java
 */

public class Programa2 extends Programa {
    public Programa2() {
        super();
        miVector = new Instruccion[7];
        miVector[0] = new Read();
        miVector[1] = new Dup();
        miVector[2] = new Write();
        miVector[3] = new Push(-1);
        miVector[4] = new Add();
        miVector[5] = new Dup();
        miVector[6] = new Jumpif(1);
    }
}