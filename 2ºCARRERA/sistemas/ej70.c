/*
 * ej70.c
 * Prueba fork().
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int pid;
    printf("Inicio prueba\n");
    pid = fork();
    printf( "Soy el proceso %d, fork() me ha devuelto %d\n", getpid(), pid);
    exit(0);
}
