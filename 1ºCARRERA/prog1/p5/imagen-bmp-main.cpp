/******************************************************************************\
 * Programación 1. Práctica 5
 * Autores: Miguel Ángel Latre y Rafael Tolosana
 * Modificado por: ¡¡¡PON AQUÍ TU NOMBRE!!!
 * Ultima revisión: 21 de noviembre de 2020
 * Resumen: Fichero de implementación del módulo principal del programa que 
 *          manipula imágenes BMP en la 5ª práctica.
 * Nota: El código de este programa está repartido en varios módulos.
 *       Para compilarlo, hay que ejecutar el comando
 *           make bmp
 *       o, en Windows,
 *           mingw32-make bmp
 *       o ejecutar la tarea "Compilar «bmp»" de VSC.
 * 
 *       Para ejecutarlo, una vez compilado, hay que ejecutar el comando
 *           bin/bmp
 *       o, en Windows,
 *           bin\bmp.exe
 *       o ejecutar la tarea "Ejecutar «bmp»" de VSC.
\******************************************************************************/
#include <iostream>
#include <string>
#include "imagen-bmp.hpp"
using namespace std;

/* 
 * Programa de ejemplo que lee la imagen «datos/prog1.bmp» y genera la imagen
 * «datos/imagen-generada.bmp». Compílalo y ejecútalo una vez para comprobar que
 * genera la imagen «datos/imagen-generada.bmp» y que esta se puede visualizar
 * en Visual Studio Code.
 * Después modifica la función «main» (incluyendo esta especificación) para
 * resolver el problema solicitado en la tarea 2 de la práctica.
 */
int main() {
    static Imagen img, imgEditada;
    string nombreFichero;
    char elegir;
    //pedimos el fichero
    cout<<"Escriba el nombre de un fichero BMP: ";
    cin>> nombreFichero;
  
    if (leerImagen(nombreFichero, img)) {
    //creamos una variable auxiliar y pido una opción
        imgEditada = img;
        do{
        cout << "Escriba una opción (N - negativizar; R - rotar): ";
            cin >> elegir;
        } while ((elegir != 'N' && elegir != 'n') && (elegir != 'R' && elegir != 'r'));

        if (elegir == 'N' || elegir == 'n'){
            //para negativizar itero por todos los píxeles de la matriz y los invierto (255 - valor rgb)
            if (elegir == 'N' || elegir == 'n'){
                const int n = 255;
                for (unsigned int x = 0; x < img.ancho; x++){
                        for (unsigned int y = 0; y < img.alto; y++){
                        imgEditada.pixels[y][x] = (Pixel){
                            (n - img.pixels[y][x].rojo),
                            (n - img.pixels[y][x].verde),
                            (n - img.pixels[y][x].azul)};
                            }}
            }}
            //para rotar itero por todos los píxeles de la matriz y los cambio de sitio usando la fórmula que aparece en el pdf de la práctica
            else{
                for (unsigned int x = 0; x < img.ancho; x++){
                    for (unsigned int y = 0; y < img.alto; y++){
                        imgEditada.pixels[x][y] = img.pixels[img.alto - 1 - x][img.ancho - 1 - y];
                        }}}
        //pido el nombre del fichero destino
        cout << "Escriba el nombre del fichero destino: ";
        cin >> nombreFichero;
        // guardo la imagen
        guardarImagen(nombreFichero, imgEditada);
        return 0;   
    }
    else {
        return 1;
    }
}