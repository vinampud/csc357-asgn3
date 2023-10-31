#define _GNU_SOURCE
#include "tree.h"
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <dirent.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#define NUM_BYTES 1024

int main(int argc, char *argv[]){
    //0 false, 1 true
    int i;
    int find_hidden = 0;
    int find_size = 0;
    int justcwd = 1;

    for (i = 1; i<argc; i++){
        if (strcmp(argv[i], "-a") == 0){
            find_hidden = 1;
        }

        else if (strcmp(argv[i], "-s") == 0){
            find_size = 1;
        }

        else {
            justcwd = 0;
        }
    }

    //if no directory/file specified in command prompt, print tree for cwd
    if(justcwd){
        char * cwd = get_current_dir_name();
        printf("Current Working Directory: %s ", cwd);
        free(cwd);
        print_tree(".", 0, find_hidden, find_size);
    }

    // call function and print a tree for every file specified on command prompt


    for (i = 1; i<argc; i++){
        if (strcmp(argv[i], "-a")!= 0 && strcmp(argv[i], "-s") != 0){
            FILE *f = fopen(argv[i], "r");
            if (f){
                fclose(f);
                printf("%s ", argv[i]);
                print_tree(argv[i], 0, find_hidden, find_size);
            }
            
        }
    }


    return 0;

}


struct array * array_list_new(int c){
    struct array *a = malloc(sizeof(struct array));
    a->capacity = c;
    a->length = 0;
    a->list = malloc(c * sizeof(char *));
    return a;

}

void array_list_add_to_end(struct array * a, char * s){
    if (a->length >= a->capacity){
        a->capacity *= 2;
        a->list = realloc(a->list, a->capacity * sizeof(char*));
    }

    a->list[a->length] = s;
    a->length++;

}

void print_size(char *name){
    struct stat st; 
    off_t s = 0;

    stat(name, &st);
    s = st.st_size;
    printf("[size: %lld]\n", (long long)s);
}

int compare (const void *arg1, const void *arg2)                                
{                                                                               
               /* Compare all of both strings */                                
   return(strcmp(*(char **)arg1, *(char **)arg2));                              
}

int is_regular_file(char *path)
{
    //printf("PATH: %s\n", path);
    struct stat path_stat;
    // int p = ;
    // printf("%d\n", p);

    if (stat(path, &path_stat) == -1){
        exit(0);
    }
    
    return S_ISREG(path_stat.st_mode);
}

void freearrlist(struct array * sorter)
{
    int i;

    for (i = 0; i < sorter->length; i++){
        free( sorter->list[i] );
    }
    

    free(sorter->list);
    free(sorter);
}

void print_tree(char *dirname, int space, int hidden, int size){

    DIR *dir;
    struct dirent * entry;
    struct array * sorter = array_list_new(10);

    dir = opendir(dirname);
    if(!dir){
        return;
    }

    else{

        if (size){
                print_size(dirname);
            }

        else{
            printf("\n");
        
        }

        while ((entry = readdir(dir)) != NULL) {
            
            
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
                continue;
            }


            if(!hidden && ((entry->d_name)[0] == '.')){
                continue;
            }
            
            char * insert = strdup(entry->d_name);
            array_list_add_to_end(sorter, insert);

        }

        closedir(dir);
        
        //sort alphabetically
        qsort(sorter->list, sorter->length, sizeof(sorter->list[0]), compare);

        char alpath[NUM_BYTES];
        int i;
        for (i = 0; i < sorter->length; i++){
            char * curname = sorter->list[i];

            //int r = is_regular_file(curname);
            //printf("--- TEST: %d ---\n", r);

            snprintf(alpath, sizeof(alpath), "%s/%s", dirname, curname);

            if(is_regular_file(alpath)){ //regular file
                if (space == 0){
                    printf("|%*s-- %s ", space, "", curname);
                }
                else{
                    printf("|%*s|-- %s ", space, "", curname);
                }

                if (size){
                    //snprintf(alpath, sizeof(alpath), "%s/%s", dirname, curname);
                    print_size(alpath);
                }

                else{
                    printf("\n");
                }
            }

            else{ //directory

                //snprintf(alpath, sizeof(alpath), "%s/%s", dirname, curname);

                //print directory name
                if (space == 0){
                    printf("|%*s-- %s ", space, "", curname);
                }
                else{
                    printf("|%*s|-- %s ", space, "", curname);
                }

                //recursive call to process this directory 
                print_tree(alpath, space + 2, hidden, size);

            }
        }

    }

    freearrlist(sorter);
    
}
