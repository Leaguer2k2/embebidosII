#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#define PN0   0x01u
#define PN1   0x02u
#define PN01  (PN0 | PN1)

static uint32_t gSysClk;
static volatile uint8_t cnt = 0;

void Timer0IntHandler(void)
{
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    cnt = (cnt + 1) & 0x3;
    uint32_t val = ((cnt & 1) ? PN0 : 0) | ((cnt & 2) ? PN1 : 0);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN01, val);
}

int main(void)
{
    gSysClk = MAP_SysCtlClockFreqSet(
        SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
        SYSCTL_USE_PLL    | SYSCTL_CFG_VCO_240, 120000000);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, PN01);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, gSysClk * 2u);

    MAP_IntEnable(INT_TIMER0A);
    MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    MAP_IntMasterEnable();

    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN01, 0x00);
    MAP_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) { }
}

