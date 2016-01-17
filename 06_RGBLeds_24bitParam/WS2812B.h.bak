/** @file WS2812B.h
 *
 * @author Wassim FILALI
 * Inferred from the Datasheet
 * 
 *
 * @compiler IAR STM8
 *
 *
 * started on local project on 10.10.2015
 * this file was created for refactoring on 03.01.2016
 * $Date:
 * $Revision:
 *
 */

#include "commonTypes.h"


// user configuration
#define NbLeds 1

#define SwitchOffLed(); SendLedColor(0, 0, 0);

// shared variables

void RGBLeds_PIO_Init_A2();

void SetLedColor(BYTE LedId, BYTE R, BYTE G, BYTE B);
void SendLedsArray();

void SendLedColor(BYTE R, BYTE G, BYTE B);

void RampColors(BYTE delay,BYTE RUp,BYTE GUp,BYTE BUp,BYTE R,BYTE G,BYTE B);

void BlinkColors(BYTE R, BYTE G, BYTE B);

