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

        if(rcnt == 0 ) {
            printf("test c: %s\n",fichier1);
            break;
        }

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

int copieRecursive( const char * fichierOuDossierOnSaitPas, const char * dest){

    // On prépare l'ouverture du dossier source
    DIR * foud = opendir(fichierOuDossierOnSaitPas);

    // levée d'erreur si avérée
    if(foud == NULL){
         printf("test copie: %s\n",fichierOuDossierOnSaitPas);
        copie(fichierOuDossierOnSaitPas,dest); 
    }else{
        // coder une fonction copier dossier 
        
        // gestion des permissions du dossier source 
        struct stat stat_buffer_folder_source;
        int status_folder_source = stat(fichierOuDossierOnSaitPas, &stat_buffer_folder_source);
        int permission_source_folder = stat_buffer_folder_source.st_mode;

        // on Vérifie bien que le dossier e destination existe, sinon, on le crée
        // On lui donne les mêmes permissions que le dossier source
        int target_folder_creation = mkdir(dest, permission_source_folder);

        // On prépare une structure servant de buffer à l'analyse du dossier source
        struct dirent * structd ;

        // boucle qui liste le contenu du dossier
        while((structd = readdir(foud)) != NULL){

            // récupération du nom du fichier source courant 
            char *source_filename = structd->d_name; 
            printf("test fichier : %s\n",source_filename);
            if (!(strcmp(source_filename, ".") ==0 || strcmp(source_filename,"..") ==0 )) {
                printf("test proute: %s\n",source_filename);
                
                // allocation de la mémoire pour le chemin source
                char *source_path = malloc(strlen(fichierOuDossierOnSaitPas) + strlen(source_filename)+1);
                strcpy(source_path,fichierOuDossierOnSaitPas);
                strcat(source_path,"/");
                strcat(source_path,source_filename);

                char * destchar = malloc(strlen(dest)+strlen(fichierOuDossierOnSaitPas)+1); 
                strcpy(source_path,dest);
                strcat(destchar,"/");
                strcat(destchar,fichierOuDossierOnSaitPas);
                

                // dest = dest+/+fichierOuDossierOnSaitPas;
                copieRecursive(source_path,destchar);   
                free(source_path); 
                free(destchar);  
                
            }
            //else perror("erreur");  // debuggage condition
                
    }

    };

    closedir(foud);
    return 0;
}
