
#include "Sys.h"
#include "Delay.h"

void SysInit(void) {
    CLK_CKDIVR=0x00;//ʱ��Ԥ��Ƶ��Ĭ��8���䣬0x18.16M-0x00��8M-0x08;4M-0x10;
}

void SysSleep(void) {
    //BottonOpenInt();
    //DelayMs(20);
    //MCUSLEEP
}

void SysOpen(void) {
    //LedSet(0);
    //BottonClooseInt();
}
