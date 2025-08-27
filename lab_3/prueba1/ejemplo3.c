#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define LED_N1 GPIO_PIN_1   // PN1
#define SW2    GPIO_PIN_1   // PJ1

int main(void)
{
    // Habilitar GPIO N y J
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)){}

    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, LED_N1);
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, SW2);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    while(1)
    {
        if(!(GPIOPinRead(GPIO_PORTJ_BASE, SW2))) {
            // Si presiono PJ1 → encender PN1
            GPIOPinWrite(GPIO_PORTN_BASE, LED_N1, LED_N1);
        } else {
            // Si no → apagar PN1
            GPIOPinWrite(GPIO_PORTN_BASE, LED_N1, 0);
        }
    }
}

