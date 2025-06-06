#define TRIAC_PULSE_DURATION 20 //5us with with TIM CLK 4MHz

extern unsigned int powerGridPeriode; //zero => no signal or overflow
extern unsigned int powerGridZeroDuration; //duration of "power grid voltage so small it is aproximated to zero
extern unsigned int triacStartTime; //0XFFFF => never fire the triac

void timerInit();
void tim3_INTERRUPT();
