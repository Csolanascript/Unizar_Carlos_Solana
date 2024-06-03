/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 2:
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Programa1.java
 */

public class Programa1 extends Programa {
    public Programa1() {
        super();
        miVector = new Instruccion[4];
        miVector[0] = new Read();
        miVector[1] = new Read();
        miVector[2] = new Add();
        miVector[3] = new Write();
    }
}