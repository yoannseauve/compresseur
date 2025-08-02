#include <stddef.h>
#include "parameters.h"

settings PID_params __attribute__((section(".stored_settings"))) =
{.var_vue.targetTemp=0,
.var_vue.regParamP=0,
.var_vue.regParamI=0,
.var_vue.regParamD=0};

extern volatile char _start_stored_settings; //defined by linker sript
settings* settings_flash_location = (settings*)(&_start_stored_settings); //create new pointer with right time to avoid aliasing related issues on flash acces (has to ensure 16bit access)

void flashUnlock(void)
{
	
	/*Check if the Lock is active*/
	if(FLASH->CR & FLASH_CR_LOCK)
	{
		FLASH->KEYR = 0x45670123U;
		FLASH->KEYR = 0xCDEF89ABU;
	}
}

void flashLock(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}

void flashErase()
{
	//Wait for BSY flag to be idle
	while(((FLASH->SR)&(FLASH_SR_BSY)));
	
	//Set the PER bit in the FLASH_CR register to enable page erasing
	FLASH->CR |= FLASH_CR_PER;
	
	//Select page to erase
	FLASH->AR = (uint32_t)(&_start_stored_settings);

	//Set the STRT bit in the FLASH_CR register to start the erasing
	FLASH->CR |= FLASH_CR_STRT;

	//Wait for BSY flag to be idle
	while(((FLASH->SR)&(FLASH_SR_BSY)));

	//Check the EOP flag in the FLASH_SR register
	if ((FLASH->SR & FLASH_SR_EOP) != 0)
	{
		//Clear EOP flag by software by writing EOP at 1
		FLASH->SR = FLASH_SR_EOP;
	}

	//Reset the PER Bit to disable the page erase 
		FLASH->CR &= ~FLASH_CR_PER;
}

void storeMem(void)
{
	
	flashUnlock();
	flashErase();
	
	//Wait for BSY flag to be idle
		while(((FLASH->SR)&(FLASH_SR_BSY))); 

	//Set PG bit
		FLASH->CR |= FLASH_CR_PG;
	
	for (unsigned int i=0; i<sizeof(settings)/sizeof(uint16_t); i++)
	{
		settings_flash_location->mem_vue[i] = PID_params.mem_vue[i];

		//Wait for BSY flag to be idle
		while(((FLASH->SR)&(FLASH_SR_BSY)));
	}

	//Check the EOP flag in the FLASH_SR register
	if ((FLASH->SR & FLASH_SR_EOP) != 0)
	{
		//clear it by software by writing it at 1
		FLASH->SR = FLASH_SR_EOP;
	}

	//Disable PG bit
	FLASH->CR &=~ FLASH_CR_PG;

	flashLock();
}

