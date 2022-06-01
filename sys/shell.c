#include <system.h>
extern void (*out)();
volatile int state;
char cmd[128];
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
    if(state==0)
    {
        if(c=='\b') {cmdx--;cmd[cmdx] = 0;}
        else if(c=='\n') {state = 1;cmdx = 0;}
        else {cmd[cmdx] = c;cmdx++;}
    }
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
        if((cmd[0]=='d'||cmd[0]=='D')&&
           (cmd[1]=='i'||cmd[1]=='I')&&
           (cmd[2]=='r'||cmd[2]=='R'))
        {
            if(atoi(&cmd[4])==0) fat_load_dir(root(),1);
            fat_load_dir(cluster2sector(atoi(&cmd[4])),1);
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
        if((cmd[0]=='r'||cmd[0]=='R')&&
           (cmd[1]=='e'||cmd[1]=='E')&&
           (cmd[2]=='a'||cmd[2]=='A')&&
           (cmd[3]=='d'||cmd[3]=='D'))
        {
            uint32_t* target;
            if(atoi(&cmd[5])==0) read_sectors_ATA_PIO(target,0, root(), sectors_per_cluster);
            read_sectors_ATA_PIO(target,0, cluster2sector(atoi(&cmd[5])), sectors_per_cluster);
            //struct file* file = ;//else if ()
            int i = 0;
            /*while(i < 512*sectors_per_cluster)
            {
                printf_("%x",target[i]);
                i++;
            }*/
            printf_("%s\n",target);//printf_("name:%s ext:%s cluster:%x\n",file->name,file->ext,file->cluster);
        }
        if((cmd[0]=='e'||cmd[0]=='E')&&
           (cmd[1]=='x'||cmd[1]=='X')&&
           (cmd[2]=='e'||cmd[2]=='E')&&
           (cmd[3]=='c'||cmd[3]=='C'))
        {
            exec(atoi(&cmd[5]));
        }
        if((cmd[0]=='b'||cmd[0]=='B')&&
           (cmd[1]=='i'||cmd[1]=='I')&&
           (cmd[2]=='n'||cmd[2]=='N'))
        {
            binexec(atoi(&cmd[4]));
        }
        memset(cmd,0,sizeof(char)*128);
        state = 0;
    }
}

void shell(char* c)
{

}


