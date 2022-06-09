#include <system.h>
extern void (*out)();
volatile int state;
char wcmd[128];
char rcmd[128];
int cmdx;
struct file
{
    char fullname[12];char null;
    char name[8];char null2;
    char ext[3];char null3;
    int cluster;
    bool directory;
};
extern struct file opendir[32];
void shel(char c)
{
    if(c=='\b') {cmdx--;wcmd[cmdx] = 0;}
    else if((c=='\n')&&(state==0)) {memcpy(rcmd,wcmd,128);state = 1;cmdx = 0;}
    else {wcmd[cmdx] = c;cmdx++;}
}
void shell_init()
{
    state = 0;
    cmdx = 0;
    out = shel;
}
//int lasty = 0;
extern int sectors_per_cluster;
void probe(uint32_t*)
{

}
volatile int num = 0;
void shell_respond()
{
    hlt();
    num++;
    //printf_("resp:%d\n",num);
    if (state == 1)
    {

        if((rcmd[0]=='d'||rcmd[0]=='D')&&
           (rcmd[1]=='i'||rcmd[1]=='I')&&
           (rcmd[2]=='r'||rcmd[2]=='R'))
        {
            if(atoi(&rcmd[4])==0) fat_load_dir(root(),1);
            fat_load_dir(cluster2sector(atoi(&rcmd[4])),1);
        }
        /*if((cmd[0]=='c'||cmd[0]=='C')&&
           (cmd[1]=='d'||cmd[1]=='D'))
        {
            char buffer[128];
            for (int i = 3;cmd[i]!='\0';i++)
            {
                buffer[i-3] = cmd[i];
                if (i==128) return;
            }
            printf_("%s",buffer);
            for (int i = 0;i<32;i++)
            {

            }
        }*/
        if((rcmd[0]=='r'||rcmd[0]=='R')&&
           (rcmd[1]=='e'||rcmd[1]=='E')&&
           (rcmd[2]=='a'||rcmd[2]=='A')&&
           (rcmd[3]=='d'||rcmd[3]=='D'))
        {
            uint32_t* target;
            if(atoi(&rcmd[5])==0) read_sectors_ATA_PIO(target,0, root(), sectors_per_cluster);
            read_sectors_ATA_PIO(target,0, cluster2sector(atoi(&rcmd[5])), sectors_per_cluster);
            //struct file* file = ;//else if ()
            int i = 0;
            /*while(i < 512*sectors_per_cluster)
            {
                printf_("%x",target[i]);
                i++;
            }*/
            printf_("%s\n",target);//printf_("name:%s ext:%s cluster:%x\n",file->name,file->ext,file->cluster);
        }
        if((rcmd[0]=='e'||rcmd[0]=='E')&&
           (rcmd[1]=='x'||rcmd[1]=='X')&&
           (rcmd[2]=='e'||rcmd[2]=='E')&&
           (rcmd[3]=='c'||rcmd[3]=='C'))
        {
            exec(atoi(&rcmd[5]));
        }
        if((rcmd[0]=='b'||rcmd[0]=='B')&&
           (rcmd[1]=='i'||rcmd[1]=='I')&&
           (rcmd[2]=='n'||rcmd[2]=='N'))
        {
            binload(atoi(&rcmd[4]));
        }
        memset(rcmd,0,sizeof(char)*128);
        state = 0;
    }
}

void shell(char* c)
{

}
