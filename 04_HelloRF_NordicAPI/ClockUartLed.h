/** @file ClockUartLed.h
 *
 * @author Wassim FILALI taken over from http://blog.mark-stevens.co.uk/
 *
 * @compiler IAR STM8
 *
 *
 * $Date: 20.09.2015
 * $Revision:
 *
 */

void InitialiseUART();

void UARTPrintf(char const *message);

void UARTPrintfLn(char const *message);

void UARTPrintfHex(unsigned char val);

void UARTPrintfHexLn(unsigned char val);


void InitialiseSystemClock();

void delay(unsigned int n);


// L E D 
void Initialise_TestLed_GPIO_B5();
void Test_Led_On();
void Test_Led_Off();

