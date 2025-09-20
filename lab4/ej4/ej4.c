#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"


#define PN0   0x01u   
#define PN1   0x02u   
#define PN01  (PN0 | PN1)


#define PF0   0x01u   
#define PF4   0x10u   
#define PF04  (PF0 | PF4)


#define PJ0   0x01u  

static uint32_t gSysClk;
static volatile uint8_t counter = 0;   

static inline void LoadPeriodFromButton(void)
{
    
    bool pressed = (MAP_GPIOPinRead(GPIO_PORTJ_BASE, PJ0) == 0u);
    uint32_t ticks = pressed ? (gSysClk * 3u) : ((gSysClk * 3u) / 2u); 
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, ticks);
}

static inline void WriteLeds(uint8_t v)
{
    
    uint32_t n_val = ((v & 0x01u) ? PN1 : 0u) |  
                     ((v & 0x02u) ? PN0 : 0u);    
    uint32_t f_val = ((v & 0x04u) ? PF4 : 0u) |   
                     ((v & 0x08u) ? PF0 : 0u);    

    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN01, n_val);
    MAP_GPIOPinWrite(GPIO_PORTF_BASE, PF04, f_val);
}

void Timer0AIntHandler(void)
{
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    counter = (counter + 1u) & 0x0Fu; 
    WriteLeds(counter);

    LoadPeriodFromButton(); 
}

int main(void)
{
   
    gSysClk = MAP_SysCtlClockFreqSet(
        SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
        SYSCTL_USE_PLL    | SYSCTL_CFG_VCO_240,
        120000000);

   
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, PN01);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN01, 0u);
    
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, PF04);
    MAP_GPIOPinWrite(GPIO_PORTF_BASE, PF04, 0u);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, PJ0);
    MAP_GPIOPadConfigSet(GPIO_PORTJ_BASE, PJ0,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    LoadPeriodFromButton(); // arranca según estado de PJ0

    MAP_IntEnable(INT_TIMER0A);
    MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    MAP_IntMasterEnable();
    MAP_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) { }
}

