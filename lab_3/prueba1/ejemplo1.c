#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LED1 GPIO_PIN_0   // PN0
#define LED2 GPIO_PIN_1   // PN1

int main(void)
{
    volatile uint32_t ui32Loop;

    // Habilitar GPIO N
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, LED1 | LED2);
    // Aqui encendemos PN0
    GPIOPinWrite(GPIO_PORTN_BASE, LED1, LED1);
    for(ui32Loop=0; ui32Loop<4000000; ui32Loop++);   // ~1s
    // Aqui encendemos PN1
    GPIOPinWrite(GPIO_PORTN_BASE, LED2, LED2);
    for(ui32Loop=0; ui32Loop<4000000; ui32Loop++);   // ~1s
    // Aqui apagamos ambos
    GPIOPinWrite(GPIO_PORTN_BASE, LED1 | LED2, 0);
    while(1){}
}

