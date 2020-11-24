#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


int copie(const char *fichier1,const char *fichier2){
 
    int f1 = open(fichier1, O_RDONLY);
    int f2 = open(fichier2, O_WRONLY | O_CREAT | O_EXCL, 666);

    while (1){
        // init buffer
        char buffer[4096];
        // on lit un bout ( readcount ) du fichier qu'on stocke dans le buffer
        int rcnt = read(f1,buffer,sizeof(buffer));

        if(rcnt == 0 ) break;

        //  partie ecriture
        int pos = 0;
        while (rcnt =! 0)
        {
            // buffer + pos : buffer correspond a un pointeur et pour avoir
            // l'endroit actuel ou on en est du buffer
            int wcnt = write(f2,buffer+pos,rcnt);
            rcnt -= wcnt;
            pos += wcnt; 
        }
        

    }

    close(f1);
    close(f2);
    return 0;
}


int main(){
    copie("test-emets.txt","test.txt");
}
