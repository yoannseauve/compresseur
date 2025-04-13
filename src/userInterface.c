#include <stddef.h>
#include "userInterface.h"
#include "uart.h"
#include "adc.h"
#include "timer.h"
#include "mini_printf.h"
#include "power_drive.h"

static enum TUIstate state = init;

void TUIInterfaceStep() // manage text User Interface
{
	unsigned int bufferSize;
	char* buffer = uartBufferToRead(&bufferSize);

	unsigned int dataInput, dataInput2;

	if(mini_snscanf(buffer, bufferSize, "help") == 4)
	{
		mini_printf(">------ HELP -----");
		if(regulationMode == automatic)
			mini_printf(">currently in [auto] mode");
		if(regulationMode == manual)
			mini_printf(">currenty in [manual] mode");
		mini_printf(">debug -> enter debug display mode (log all parameters)");
		mini_printf(">manual -> enter manual mode with output power set by user");
		mini_printf(">auto -> enter automatic mode with output power regulated for temp to reach it's target value");
		mini_printf(">set output xx -> set output to xx \\%, enters manual mode");
		mini_printf(">set temp xx -> set target temperature to xx°");
		mini_printf(">PID -> display PID settings");
		mini_printf(">PID set p XX.XXX > set P parameter to XX.XXX");
		mini_printf(">PID set I XXXXX > set I parameter to 0.XXXXX");
		mini_printf(">PID set D XX.XXX > set D parameter to XX.XXX");
	}
	else if(mini_snscanf(buffer, bufferSize, "manual") == 6)
	{
		mini_printf(">entering [manual] mode");
		regulationMode = manual;
	}
	else if(mini_snscanf(buffer, bufferSize, "auto") == 4)
	{
		mini_printf(">entering [auto] mode");
		regulationMode = automatic;
	}
	else if(mini_snscanf(buffer, bufferSize, "set output %u", &dataInput) >= 12)
	{
		regulationMode = manual;
		mini_printf(">output power set to %d \\%", dataInput);
		setOutputPower(((float)dataInput)/100);
	}
	else if(mini_snscanf(buffer, bufferSize, "set temp %u", &dataInput) >= 10)
	{
		mini_printf(">target temperature set to %d \\°C", dataInput);
		targetTemp = dataInput;
	}
	else if(mini_snscanf(buffer, bufferSize, "PID set P %u.%u", &dataInput, &dataInput2) == 16)
	{
		mini_printf(">PID P param set to %d.%d", dataInput, dataInput2);
		regParamP = (float)dataInput + 0.001 * (float)dataInput2;
	}
	else if(mini_snscanf(buffer, bufferSize, "PID set I %u", &dataInput) == 15)
	{
		mini_printf(">PID I param set to 0.%d", dataInput);
		regParamI = 0.00001 * (float)dataInput;
	}
	else if(mini_snscanf(buffer, bufferSize, "PID set D %u.%u", &dataInput, &dataInput2) == 16)
	{
		mini_printf(">PID D param set to %d.%d", dataInput, dataInput2);
		regParamD = (float)dataInput + 0.001 * (float)dataInput2;
	}

	else
		switch(state)
		{
			case init :
				if(buffer != NULL)
				{
					if(mini_snscanf(buffer, bufferSize, "debug") == 5)
						state = debugDisaplay;
					else if (mini_snscanf(buffer, bufferSize, "PID") == 3)
					{
						mini_printf(">target_tmp = %d P = %d.%d I = 0.%d D = %d.%d min_int = %d.%d max_int = %d.%d", targetTemp, (int)regParamP, (int)(regParamP*1000)%1000, (int)(regParamI*100000), (int)regParamD, (int)(regParamD*1000)%1000, (int)(regParamIMin*100.0), (int)(regParamIMin*100000)%1000, (int)(regParamIMax*100.0), (int)(regParamIMax*100000)%1000);
					}
					else
						mini_printf("unrecognized command, type help to list available options");
				}
				break;

			case debugDisaplay :
				mini_printf(">temp = %d target_temp = %d grid_periode = %d set_out_pwr = %d\\%", adc_read_temp(), targetTemp, powerGridPeriode,(int)(readOutputPower()*100));
				if(buffer != NULL)
					state = init;
				break;
		}

	if(buffer != NULL)
		uartBufferTreated();
}
