#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int main(void)
{
    FILE * fp;
    FILE * outfp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("main.s", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    outfp = fopen("main.mod.s" ,"a");
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

    if (remove("main.s") !=0) {
        printf("The file is not deleted.\n");
    }

    if (rename("main.mod.s","main.s") !=0) {
        printf("The file is not renamed.\n");
    }

    exit(EXIT_SUCCESS);
}
