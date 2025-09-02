import RPi.GPIO as GPIO
import time

class BinaryCounter:
    def __init__(self, leds, btn_inc, btn_dec):
        self.leds = leds
        self.btn_inc = btn_inc
        self.btn_dec = btn_dec
        self.value = 0
        
        GPIO.setwarnings(False)
        GPIO.setmode(GPIO.BCM)
        for led in leds:
            GPIO.setup(led, GPIO.OUT)
        GPIO.setup(btn_inc, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        GPIO.setup(btn_dec, GPIO.IN, pull_up_down=GPIO.PUD_UP)
        
    def update_leds(self):
        for i, led in enumerate(self.leds):
            bit = (self.value >> i) & 1
            GPIO.output(led, GPIO.HIGH if bit else GPIO.LOW)
            
    def print_value(self):
        print(f"Decimal: {self.value}, Binary: {bin(self.value)[2:].zfill(4)}, Hex: {hex(self.value)}")
        
    def check_buttons(self):
        if GPIO.input(self.btn_inc) == GPIO.LOW:
            self.value += 1
            if self.value > 15:
                self.value = 0
            time.sleep(0.2)
        if GPIO.input(self.btn_dec) == GPIO.LOW:
            if self.value > 0:
                self.value -= 1
            time.sleep(0.2)
        
    def run(self):
        try:
            while True:
                self.check_buttons()
                self.update_leds()
                self.print_value()
                time.sleep(0.1)
        except KeyboardInterrupt:
            GPIO.cleanup()

# Pines GPIO para LEDs y botones
LED_PINS = [17, 27, 22, 23]   # Cambia según tus conexiones
BTN_INC = 24
BTN_DEC = 25

counter = BinaryCounter(LED_PINS, BTN_INC, BTN_DEC)
counter.run()