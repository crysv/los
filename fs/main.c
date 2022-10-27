#include "stdint.h"
int main()
{
    debug_call();
    //void* e = malloc(100);
    //free(e);
    //puts("hello\n");
    uint32_t vidinfo[5];
    //vidinfo[0][x*vidinfo[4] + y*vidinfo[3]];
    init(vidinfo);
    //fillrect(vidinfo,0,0,vidinfo[1],vidinfo[2],0x000000);
    clear(vidinfo,0x000000);
    return 0;
}
