typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
#define VSYSCALL(a,b,c,d) __asm__ __volatile__ ( "int $0x7f" : : "a" (a), "b"(b), "c"(c), "d"(d))
#define stdcall __attribute__((stdcall))
void init(uint32_t* vidinfo)
{
    VSYSCALL(0,1,vidinfo,0);
}
void stdcall putpixel(uint32_t* vidinfo, int x,int y, int color) {
    uint32_t* screen = vidinfo[0];
    unsigned where = x*vidinfo[4] + y*vidinfo[3];
    screen[where] = color;
}
void stdcall fillrect(uint32_t* vidinfo,int x,int y,int w,int h,uint32_t color) {
    uint32_t *screen = vidinfo[0];
    int i, j;

    uint32_t where = x*vidinfo[4] + y*vidinfo[3];
    for (i = 0; i < w; i++) {
        for (j = 0; j < h; j++) {
            screen[j*(uint32_t)((uint32_t)vidinfo[4]/32)] = color;
        }
        screen+=vidinfo[3]/4;
    }
}
void clear(uint32_t* vidinfo,uint32_t color)
{
    uint32_t *screen = vidinfo[0];
    for (int i = 0;i < vidinfo[1]*vidinfo[2];i++) screen[i] = color;
}
