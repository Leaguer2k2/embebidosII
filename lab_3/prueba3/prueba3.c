
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#define LED1 GPIO_PIN_1   // PN1
#define LED2 GPIO_PIN_0   // PN0
#define LED3 GPIO_PIN_4   // PF4
#define LED4 GPIO_PIN_0   // PF0

#define SW1 GPIO_PIN_0  // PJ0
#define SW2 GPIO_PIN_1  // PJ1



#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif


int
main(void)
{
    volatile uint32_t ui32Loop;
    uint8_t counter = 0;  
    uint8_t sw;


    // Aqui configuramos los pines para los leds
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, LED1 | LED2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED3 | LED4);
    
    // Aqui configuramos los botones
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)) {}
    GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, SW1 | SW2);
    GPIOPadConfigSet(GPIO_PORTJ_BASE, SW1 | SW2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    
    while(1)
    {
       
        // Leer botones (activo en bajo)
        sw = GPIOPinRead(GPIO_PORTJ_BASE, SW1 | SW2);

        // Botón SW1: incrementar contador
        if(!(sw & SW1))
        {
            if(counter < 15) counter++;
            for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++){} 
        }

        // Botón SW2: decrementar contador
        if(!(sw & SW2))
        {
            if(counter > 0) counter--;
            for(ui32Loop = 0; ui32Loop < 200000; ui32Loop++){} 
        }

        // Mostrar contador en LEDs
        GPIOPinWrite(GPIO_PORTN_BASE, LED1 | LED2,
            (counter & 0x01 ? LED1 : 0) | (counter & 0x02 ? LED2 : 0));
        GPIOPinWrite(GPIO_PORTF_BASE, LED3 | LED4,
            (counter & 0x04 ? LED3 : 0) | (counter & 0x08 ? LED4 : 0));
   
}

       
    }
    
    
     




