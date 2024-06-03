/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Programa3.java
 */

public class Programa3 extends Programa {
    public Programa3() {
        super();
        miVector = new Instruccion[14];
        miVector[0] = new Push(1);
        miVector[1] = new Read();
        miVector[2] = new Swap();
        miVector[3] = new Over();
        miVector[4] = new Mul();
        miVector[5] = new Swap();
        miVector[6] = new Push(-1);
        miVector[7] = new Add();
        miVector[8] = new Dup();
        miVector[9] = new Push(-2);
        miVector[10] = new Add();
        miVector[11] = new Jumpif(2);
        miVector[12] = new Swap();
        miVector[13] = new Write();
    }
}