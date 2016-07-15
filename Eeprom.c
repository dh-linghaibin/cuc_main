
#include "Eeprom.h"

#define EEPROM_STARTADDR 0x1000  

void EeepromInit(void) {
    FLASH_CR1 = 0X00;
	FLASH_CR2 = 0X00;
//	FLASH_NCR2 = 0XFF;
	FLASH_DUKR = 0XAE;
	FLASH_DUKR = 0X56;
    FLASH_CR2=0x00;  
	while(!(FLASH_IAPSR&0X08));
}


void EepromWrite(u8 addr,u8 dat) {
    /*
	volatile u8 *p;
	p = (u8 *)(0x4000+addr);
	*p = dat;
	while(!(FLASH_IAPSR&0X40));*/
   // u16 T=0;  
    u8 *EEP;  
    FLASH_DUKR=0xAE;                // 写入第一个密钥
    FLASH_DUKR=0x56;                // 写入第二个密钥	
    while((FLASH_IAPSR&0x04));
    EEP=(u8 *)(EEPROM_STARTADDR+addr);  
   // FLASH_DUKR=0xAE;  
   // FLASH_DUKR=0x56;  
    //FLASH_CR2=0x00;  
    //while((!(FLASH_IAPSR&0X08))&(T<0xFFFF))T++;  
    *EEP = dat;
    while(!(FLASH_IAPSR&0X40));
    FLASH_IAPSR&=0xF7;              //重新写保护
    /*while(Length--){  
        *RxBuffer++=*EEP++;  
    } */ 
}
/***************************************
Function: Eeprom_Read
Function: Read variable corresponding to the address saved
Input: Address
Output: None
Explanation:
***************************************/
u8 EepromRead(u8 addr) {
	volatile u8 *p;
	p = (u8 *)(EEPROM_STARTADDR+addr);
	return *p;
}