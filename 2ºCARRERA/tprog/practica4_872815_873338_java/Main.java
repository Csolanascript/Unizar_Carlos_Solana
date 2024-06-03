/*
 *  Asignatura : Tecnología de la Programación
 *  Práctica 4:
 *  Versión: Java
 *  Autores : Mateo Lorente, Diego  --- 873338
 *            Solana Melero, Carlos --- 872815
 *  Fichero : Main.java
 */

import java.util.Scanner;

class Main
{
	public static void main(String[] args)
	{
		Shell sh = new Shell();

		Scanner scn = new Scanner(System.in);
		for (boolean done=false; !done;)
		{
			System.out.print(sh.pwd()+"> ");

			// Leer linea de entrada
			String line;
			try {
				line = scn.nextLine();
			} catch (Exception e) {
				done = true;
				continue;
			}

			// Separar tokens
			String[] cmd = line.split(" ");

			if ((cmd.length<1) || (cmd[0].length()<1))
				continue;

			try
			{
				if (cmd[0].equals("exit") || cmd[0].equals("by"))
				{
					done = true;
				}
				else if (cmd[0].equals("pwd"))
				{
					System.out.println(sh.pwd());
				}
				else if (cmd[0].equals("ls"))
				{
					System.out.println(sh.ls());
				}
				else if (cmd[0].equals("du"))
				{
					System.out.println(sh.du());
				}
				else if (cmd[0].equals("mkdir"))
				{
					sh.mkdir(cmd[1]);
				}
				else if (cmd[0].equals("vi"))
				{
					sh.vi(cmd[1], Integer.parseInt(cmd[2]));
				}
				else if (cmd[0].equals("stat"))
				{
					System.out.println(sh.stat(cmd[1]));
				}
				else if (cmd[0].equals("cd"))
				{
					sh.cd(cmd[1]);
				}
				else if (cmd[0].equals("ln"))
				{
					sh.ln(cmd[1],cmd[2]);
				}
				else if (cmd[0].equals("rm"))
				{
					sh.rm(cmd[1]);
				}
				else
				{
					System.out.println("Error sintactico: comando desconocido");
				}
			}
			catch(ExcepcionArbolFicheros e)
			{
				System.out.println(e);
			}
			catch (ArrayIndexOutOfBoundsException e)
			{
				System.out.println("Error sintactico: parametros insuficientes");
				System.out.println(e);
			}
		}

		System.out.println("\nBy!!");
	}
};
