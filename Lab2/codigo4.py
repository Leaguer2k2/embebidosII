import RPi.GPIO as GPIO
import time

# Pines GPIO
LEDS = [17, 27, 22, 23]
BTN_LED = 24
BTN_TIME = 25

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
for led in LEDS:
    GPIO.setup(led, GPIO.OUT)
GPIO.setup(BTN_LED, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(BTN_TIME, GPIO.IN, pull_up_down=GPIO.PUD_UP)

led_index = 0
led_time = 1

def next_led(index):
    return (index + 1) % len(LEDS)

def increase_time(current_time):
    return current_time + 1

def reset_leds():
    list(map(lambda l: GPIO.output(l, GPIO.LOW), LEDS))

def update_led(index, duration):
    reset_leds()
    GPIO.output(LEDS[index], GPIO.HIGH)
    time.sleep(duration)

try:
    while True:
        if GPIO.input(BTN_LED) == GPIO.LOW:
            led_index = next_led(led_index)
            led_time = 1
            time.sleep(0.3)
        if GPIO.input(BTN_TIME) == GPIO.LOW:
            led_time = increase_time(led_time)
            time.sleep(0.3)
        update_led(led_index, led_time)

except KeyboardInterrupt:
    GPIO.cleanup()