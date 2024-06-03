#include<stdio.h>
#include<error.h>
#include<signal.h>
#include<error.h> 

int main(int argc, char argv*[])
    int pelota, pid_hijo, pid_padre, contador,tub1[2],tub2[2];
    contador = 100;
    pid_padre = getpid();
    pipe(tub1);
    pipe(tub2);
    switch(pid_hijo = fork()){
        case -1:
            syserr("fallo al crear hijo")
        case 0:
            while(1){
                read(tub1[0], pelota, sizeof(pelota));
                duerme(1)
                pelota++;
                if (pelota == contador){
                    kill(pid_padre, SIGKILL);
                    kill(pid_hijo, SIGKILL);
                }
                write(tub2[1], pelota, sizeof(pelota));
            }
        case 1:
            pelota = 1;
            write(tub1[1], pelota, sizeof(pelota));
            while(1){
                read(tub2[0], pelota, sizeof(pelota));
                duerme(1)
                pelota++;
                if (pelota == contador){
                    kill(pid_padre, SIGKILL);
                    kill(pid_hijo, SIGKILL);
                }
                write(tub1[1], pelota, sizeof(pelota));
            }
            







    }
