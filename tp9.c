#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>


int copie(const char *fichier1,const char *fichier2){
 
    int f1 = open(fichier1, O_RDONLY);
    int f2 = open(fichier2, O_WRONLY | O_CREAT | O_EXCL, 0666);

    if(f1 == NULL)
        perror(fichier1);    

    struct stat stat_buffer;
    int  status;

    status = stat(fichier1, &stat_buffer);

    int permission = stat_buffer.st_mode;

    while (1){
        // init buffer
        char buffer[4096];
        // on lit un bout ( readcount ) du fichier qu'on stocke dans le buffer
        int rcnt = read(f1,buffer,sizeof(buffer));

        if(rcnt == 0 ) break;

        //  partie ecriture
        int pos = 0;
        while (rcnt != 0)
        {
            // buffer + pos : buffer correspond a un pointeur et pour avoir
            // l'endroit actuel ou on en est du buffer
            int wcnt = write(f2,buffer+pos,rcnt);
            rcnt -= wcnt;
            pos += wcnt; 
        }
        

    }

    fchmod(f2,permission);
    close(f1);
    close(f2);
    return 0;
}

int copieRepertoire(const char *dossier1 , const char *dossier2){
    DIR * d1 = opendir(dossier1);
    int compt = 0; 

    if(d1 == NULL){
        perror(dossier1); 
    }

    struct dirent * structd ;
    // boucle qui liste le contenu du dossier
    while((structd = readdir(d1)) != NULL){
        char *s = structd->d_name;
            if (s != "." || s != ".."){
                    printf("%s\n",s);   
                    while (compt<sizeof(s)){
                        copie(s[compt],"testons.txt");
                        compt++ ; 
                    }
                    
            }
                
    }

    closedir(d1);
    return 0;
}


int main(){
    //int x = 0; 
    //printf("Ce programme permet de copier un fichier.\n");
    //copie("test-emets.txt","test.txt");
    copieRepertoire("testrepertoire","testCopieRepertoire");
    /**
    while (x<10)
    {
        printf(".");
        sleep(0.800);
        x++;
    }
    printf("\n");
    printf("Copie effectuée !\n");
    */
}