/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "gpio.h"
#include "SysTick.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define T0H 0.0000004
#define T0L 0.0000008
#define T1H 0.0000008
#define T1L 0.0000006

#define T0H_COUNT (T0H * SYSTICK_ISR_FREQUENCY_HZ)
#define T0L_COUNT (T0L * SYSTICK_ISR_FREQUENCY_HZ)
#define T1H_COUNT (T1H * SYSTICK_ISR_FREQUENCY_HZ)
#define T1L_COUNT (T1L * SYSTICK_ISR_FREQUENCY_HZ)

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
static void SysTick_Driver(void);
static void delay(uint32_t ticks);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	// Initializing the systick
	// SysTick_Init(SysTick_Driver);

	// Setting the pin modes
	gpioMode(PIN_DATA, OUTPUT);
	gpioMode(PIN_TRIGGER, OUTPUT);

	// Initializing the pin status
	gpioWrite(PIN_DATA, LOW);
	gpioWrite(PIN_TRIGGER, LOW);

	// Setting and initializing the PIT, yes...
	PIT->MCR = 0;
	PIT->CHANNEL[0].LDVAL = 10;
	PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TEN_MASK;
}

/* Función que se llama constantemente en un ciclo infinito */
uint8_t byteBuffer;
uint8_t highCount;
uint8_t lowCount;
uint8_t mask;
bool started;

void App_Run (void)
{
	gpioWrite(PIN_TRIGGER, HIGH);
	byteBuffer = 0x0F;
	for (mask = 0x80 ; mask > 0 ; mask >>= 1)
	{
		if (byteBuffer & mask)
		{
			highCount = T1H_COUNT;
			lowCount = T1L_COUNT;
		}
		else
		{
			highCount = T0H_COUNT;
			lowCount = T0L_COUNT;
		}

		gpioWrite(PIN_DATA, HIGH);
		delay(highCount);
		gpioWrite(PIN_DATA, LOW);
		delay(lowCount);
	}
	gpioWrite(PIN_TRIGGER, LOW);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void SysTick_Driver(void)
{
	if (started)
	{
		gpioWrite(PIN_DATA, HIGH);
		started = false;
	}
	else if (highCount)
	{
		highCount--;
		if (highCount == 0)
		{
			gpioWrite(PIN_DATA, LOW);
		}
	}
	else if (lowCount)
	{
		lowCount--;
	}
}

static void delay(uint32_t ticks)
{
	PIT->CHANNEL[0].TFLG = 0x01;
	while(ticks)
	{
		while((PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) == 0);
		ticks--;
	}
}


/*******************************************************************************
 ******************************************************************************/
