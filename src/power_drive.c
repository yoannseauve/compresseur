#include "power_drive.h"
#include "timer.h"
#include "adc.h"
#include "lookup_math.h"
#include "parameters.h"

//float regParamP = 0.0;
//float regParamI = 0.0;
//float regParamD = 0.0;
float regParamIMax = 1.0;
float regParamIMin = 0.0;

//int targetTemp = 0;
enum regulationMode regulationMode = automatic;

static float setOutputPowerValue; //percent of output power set by "setOutputPower()"

void setOutputPower(float percent)
{
	if(powerGridPeriode != 0)
	{
		if(percent > 1.0)
			percent = 1.0;
		if(percent < 0.0)
			percent = 0.0;
		triacStartTime = powerGridPeriode - __revers_sin_power_lookup__[(int)(percent*100.0)] * (powerGridPeriode - powerGridZeroDuration); //lookup acounts for sin voltage. x% max power != x% sin periode
	}
	setOutputPowerValue = percent;
}

float readOutputPower()
{
	return setOutputPowerValue;
}

void PIDStep()
{
	static float integralError = 0.0;
	static int lastError = 0;
	static int tick_count = 0;

	if (tick_count++ < PID_FREQ_DIV-1)
		return;
	tick_count = 0;

	if(regulationMode == automatic && powerGridPeriode != 0)
	{
		int error = PID_params.var_vue.targetTemp - adc_read_temp();
		int derivativError = error - lastError;

		setOutputPower(error*PID_params.var_vue.regParamP + derivativError*PID_params.var_vue.regParamD + integralError);

		integralError += PID_params.var_vue.regParamI * (float)(error);
		if(integralError > regParamIMax)
			integralError = regParamIMax;
		if(integralError < regParamIMin)
			integralError = regParamIMin;
	}
	else
		setOutputPower(setOutputPowerValue);
}
