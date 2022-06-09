/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Timer driver
*
*  Notes: No warranty expressed or implied. Use at own risk. */
#include <system.h>

/* This will keep track of how many ticks that the system
*  has been running for */
volatile int timer_ticks = 0;

/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. Why 18.222Hz?*/
void timer_handler(struct regs *r)
{
    /* Increment our 'tick count' */
    timer_ticks++;

    /* Every 18 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 18 == 0)
    {
        puts("One second has passed\n");
    }
}

/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks)
{
    uint16_t count = 5;
    outportb(0x40,count & 0xff);
    outportb(0x40,(count >> 8) & 0xff);
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install()
{
    int freq = 1000;
    uint16_t divisor = (uint16_t)( 1193181 / (uint16_t)freq);
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
    outportw(0x8A00,0x8A00); outportw(0x8A00,0x08AE0);
    outportb(0x43,0x38);
    outportb(0x40,divisor & 0xff);
    outportb(0x40,(divisor >> 8) & 0xff);
}
