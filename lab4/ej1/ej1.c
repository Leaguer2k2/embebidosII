// ej1_hex.c
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#define PN1   0x02u
#define BLINK_S  1u

static uint32_t gSysClk;

void Timer0IntHandler(void)
{
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    uint32_t v = MAP_GPIOPinRead(GPIO_PORTN_BASE, PN1);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN1, v ^ PN1);
}

int main(void)
{
    gSysClk = MAP_SysCtlClockFreqSet(
        SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
        SYSCTL_USE_PLL    | SYSCTL_CFG_VCO_240, 120000000);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, PN1);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, gSysClk * BLINK_S);

    MAP_IntEnable(INT_TIMER0A);
    MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    MAP_IntMasterEnable();

    MAP_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) { }
}

