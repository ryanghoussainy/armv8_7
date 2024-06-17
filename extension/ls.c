#include "ls.h"

void ls(Shell* shell)
{
    struct dirent* de;

    DIR* dir = opendir(shell->path);

    if (dir == NULL)
    { 
        printf("Could not open directory: %s", shell->path); 
        return; 
    } 
  
    while ((de = readdir(dir)) != NULL) 
            fprintf(shell->out, "%s\n", de->d_name); 
  
    closedir(dir);
}