
#include "Eeprom.h"

void EeepromInit(void) {
    FLASH_CR1 = 0X00;
	FLASH_CR2 = 0X00;
	FLASH_NCR2 = 0XFF;
	FLASH_DUKR = 0XAE;
	FLASH_DUKR = 0X56;
	while(!(FLASH_IAPSR&0X08));
}


void EepromWrite(u8 addr,u8 dat) {
	volatile u8 *p;
    FLASH_DUKR=0xAE;                // 写入第一个密钥
    FLASH_DUKR=0x56;                // 写入第二个密钥	
    while((FLASH_IAPSR&0x04));
	p = (u8 *)(0x4000+addr);
	*p = dat;
	while(!(FLASH_IAPSR&0X40));
    FLASH_IAPSR&=0xF7;              //重新写保护
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
	p = (u8 *)(0x4000+addr);
	return *p;
}



