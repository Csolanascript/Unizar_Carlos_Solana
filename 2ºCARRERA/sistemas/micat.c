#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
int main (int argc, char *argv[]){
        int cuenta,id;
        char buf[BUFSIZ];
	if(argc == 2){
		if((id = open(argv[1],1))==-1){ fprintf(stderr, "El fichero no existe /n");
						exit(1);}
		while ((cuenta = read( 0, buf, sizeof( buf ) )) > 0 ){
			write( id, buf, cuenta );
			}
			if(cuenta == -1){
				fprintf(stderr, "no ha podido copiar /n");
			}

	}


	else{
        	cuenta=read(0,buf,sizeof(buf));
       		write(1,buf,cuenta);
        }
}


