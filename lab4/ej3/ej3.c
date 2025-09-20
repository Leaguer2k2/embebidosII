#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#define PN0 0x01u
#define PN1 0x02u
#define PJ0 0x01u

static uint32_t gSysClk;
static volatile uint8_t cnt  = 0;
static volatile uint8_t fast = 0;

static inline void SetPeriod(void) {
    uint32_t ticks = fast ? (gSysClk / 2u) : (gSysClk * 1u);
    MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, ticks);
}

void Timer0AIntHandler(void)
{
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    cnt = (cnt + 1u) & 0x03u;
    uint32_t val = ((cnt & 1u) ? PN0 : 0u) | ((cnt & 2u) ? PN1 : 0u);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN0 | PN1, val);
}

void GPIOJIntHandler(void)
{
    uint32_t st = GPIOIntStatus(GPIO_PORTJ_BASE, true);
    MAP_GPIOIntClear(GPIO_PORTJ_BASE, st);
    SysCtlDelay(gSysClk / (3u * 100u));
    if ((MAP_GPIOPinRead(GPIO_PORTJ_BASE, PJ0) & PJ0) == 0u) {
        fast ^= 1u;
        SetPeriod();
    }
}

int main(void)
{
    gSysClk = MAP_SysCtlClockFreqSet(
        SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
        SYSCTL_USE_PLL    | SYSCTL_CFG_VCO_240,
        120000000);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, PN0 | PN1);
    MAP_GPIOPinWrite(GPIO_PORTN_BASE, PN0 | PN1, 0x00u);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ));
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, PJ0);
    MAP_GPIOPadConfigSet(GPIO_PORTJ_BASE, PJ0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntDisable(GPIO_PORTJ_BASE, PJ0);
    GPIOIntTypeSet(GPIO_PORTJ_BASE, PJ0, GPIO_FALLING_EDGE);
    GPIOIntClear(GPIO_PORTJ_BASE, PJ0);
    MAP_IntEnable(INT_GPIOJ);
    GPIOIntEnable(GPIO_PORTJ_BASE, PJ0);

    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while(!MAP_SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
    MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    SetPeriod();

    MAP_IntEnable(INT_TIMER0A);
    MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    MAP_IntMasterEnable();
    MAP_TimerEnable(TIMER0_BASE, TIMER_A);

    while(1) { }
}

