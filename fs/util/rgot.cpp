#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "../../include/libfeat.h"
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
char *strremove(char *str, const char *sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            memmove(q, p, r - p);
            q += r - p;
        }
        memmove(q, p, strlen(p) + 1);
    }
    return str;
}
/* when return 1, scandir will put this dirent to the list */
 static int parse_ext(const struct dirent *dir)
 {
   if(!dir)
     return 0;

   if(dir->d_type == DT_REG) { /* only deal with regular file */
       const char *ext = strrchr(dir->d_name,'.');
       if((!ext) || (ext == dir->d_name))
         return 0;
       else {
         if(strcmp(ext, ".s") == 0)
           return 1;
       }
   }

   return 0;
 }
int rgot(char* name)
{
    FILE * fp;
    FILE * outfp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(name, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    char oldname[32];
    strcpy(oldname,name);
    strcat(name,".mod");
    outfp = fopen(name,"a");
    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, "@GOT"))
        {
            strremove(line,"@GOT");
            int len = sizeof(char)/sizeof(line);
            char* substr = &line[7];
            bool lib = false;
            removeChar(line,'\n');
            for(int i = 0;i < libflsize;i++)
                if (strcmp(libflist[i],substr)==0)
                    lib = true;
            if (!lib)
            {
                removeChar(line,'*');
            }
            printf("%s\n", line);
            strcat(line,"\n");
        }
        fputs(line,outfp);
    }

    fclose(fp);
    if (line)
        free(line);

    if (remove(oldname) !=0) {
        printf("The file is not deleted.\n");
    }

    if (rename(name,oldname) !=0) {
        printf("The file is not renamed.\n");
    }
}
int main(void)
{
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, parse_ext, alphasort);
    if (n < 0) {
        perror("scandir");
        return 1;
    }
    else {
        while (n--) {
            printf("%s\n", namelist[n]->d_name);
            rgot(namelist[n]->d_name);
            free(namelist[n]);
        }
        free(namelist);
    }

    return 0;
}
