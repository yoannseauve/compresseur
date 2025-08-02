#include "stm32f030x6.h"
typedef union
{
struct 
	{
		int targetTemp;
		float regParamP;
		float regParamI;
		float regParamD;
	} var_vue;
	uint16_t mem_vue[4*2];	
} settings;

extern settings PID_params;

void storeMem(void);
//void loadMem(void);
