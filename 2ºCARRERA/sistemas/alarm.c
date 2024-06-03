#include <signal.h>
main(){
 signal(SIGALRM, SIG_DFL);
 alarm(5);
 pause();

