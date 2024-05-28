#include "DAVE.h"                 //Declarations from DAVE Code Generation (includes SFR declaration)
#include <stdio.h>

int _write(int file, char *p, int len) {
	UART_Transmit(&UART_0, p, len);
	return len;
}

int delay1000;

// 1ms
void callback() {
	if(delay1000 > 0)
		delay1000--;

}

void delay(int d) {
	delay1000 = d;
	while(delay1000 > 0)
		;
}

double movingMean(int inValue) {
#define max 100
	static double sum = 0;
	static int buffer[max];
	static int pos = 0;
	// -
	sum -= buffer[pos];
	// c
	buffer[pos] = inValue;
	// +
	sum += buffer[pos];
	pos++;
	pos %= max;
	return sum / (double)max;
}

int main(void)
{
  DAVE_STATUS_t status;

  status = DAVE_Init();           /* Initialization of DAVE APPs  */

  if (status != DAVE_STATUS_SUCCESS)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
    XMC_DEBUG("DAVE APPs initialization failed\n");

    while(1U)
    {

    }
  }

  int id = SYSTIMER_CreateTimer(1000, SYSTIMER_MODE_PERIODIC, callback, NULL);

  SYSTIMER_StartTimer(id);

  printf("start...\n");
  /* Placeholder for user application code. The while loop below can be replaced with user application code. */
  while(1U)
  {
  	ADC_MEASUREMENT_StartConversion(&ADC_MEASUREMENT_0);
  	delay(5);
  	float adcValue1 = movingMean(ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_BatteryVoltage_handle));
  	float adcValue2 = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_Termperature_handle);
  	float adcValue3 = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_lumen_handle);
  	float adcValue4 = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_weight_handle);
  	float adcValue5 = ADC_MEASUREMENT_GetResult(&ADC_MEASUREMENT_current_handle);
  	printf("%f,%f,%f,%f,%f\n", adcValue1, adcValue2, adcValue3, adcValue4, adcValue5);
  }
}
