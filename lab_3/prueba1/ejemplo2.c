#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LED_N0 GPIO_PIN_0  // PN0
#define LED_N1 GPIO_PIN_1  // PN1
#define LED_F0 GPIO_PIN_0  // PF0
#define LED_F4 GPIO_PIN_4  // PF4

int main(void)
{
    volatile uint32_t ui32Loop;

    // Habilitamos GPIO N y F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, LED_N0 | LED_N1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_F0 | LED_F4);
    
    // Encendemos PN0 y PF0
    GPIOPinWrite(GPIO_PORTN_BASE, LED_N0, LED_N0);
    GPIOPinWrite(GPIO_PORTF_BASE, LED_F0, LED_F0);
    for(ui32Loop=0; ui32Loop<4000000; ui32Loop++);
    // Encendemos PN1 y PF4
    GPIOPinWrite(GPIO_PORTN_BASE, LED_N1, LED_N1);
    GPIOPinWrite(GPIO_PORTF_BASE, LED_F4, LED_F4);
    for(ui32Loop=0; ui32Loop<4000000; ui32Loop++);

    // Apagamos PN1 y PF4
    GPIOPinWrite(GPIO_PORTN_BASE, LED_N1, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, LED_F4, 0);
    for(ui32Loop=0; ui32Loop<4000000; ui32Loop++);

    // Apagamos PN0 y PF0
    GPIOPinWrite(GPIO_PORTN_BASE, LED_N0, 0);
    GPIOPinWrite(GPIO_PORTF_BASE, LED_F0, 0);

    while(1){}
}

