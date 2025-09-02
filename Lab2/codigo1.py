import RPi.GPIO as GPIO
import time

LED1 = 17
LED2 = 27
BUTTON = 22

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)
GPIO.setup(BUTTON, GPIO.IN, pull_up_down=GPIO.PUD_UP)

state = 1

def blink_alternating():
    while state == 1:
        GPIO.output(LED1, GPIO.HIGH)
        GPIO.output(LED2, GPIO.LOW)
        time.sleep(1)
        GPIO.output(LED1, GPIO.LOW)
        GPIO.output(LED2, GPIO.HIGH)
        time.sleep(1)
        if check_button():
            break

def blink_simultaneous():
    while state == 2:
        GPIO.output(LED1, GPIO.HIGH)
        GPIO.output(LED2, GPIO.HIGH)
        time.sleep(2)
        GPIO.output(LED1, GPIO.LOW)
        GPIO.output(LED2, GPIO.LOW)
        time.sleep(2)
        if check_button():
            break

def leds_on():
    GPIO.output(LED1, GPIO.HIGH)
    GPIO.output(LED2, GPIO.HIGH)
    while state == 3:
        if check_button():
            break
        time.sleep(0.1)

def leds_off():
    GPIO.output(LED1, GPIO.LOW)
    GPIO.output(LED2, GPIO.LOW)
    while state == 4:
        if check_button():
            break
        time.sleep(0.1)

def check_button():
    global state
    if GPIO.input(BUTTON) == GPIO.LOW:
        state += 1
        if state > 4:
            state = 1
        time.sleep(0.3)
        return True
    return False

try:
    while True:
        if state == 1:
            blink_alternating()
        elif state == 2:
            blink_simultaneous()
        elif state == 3:
            leds_on()
        elif state == 4:
            leds_off()
except KeyboardInterrupt:
    GPIO.cleanup()