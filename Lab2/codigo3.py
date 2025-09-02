import RPi.GPIO as GPIO
import time
import random

# Pines GPIO
HEATER = 17  # LED rojo simula calefactor
FAN = 27     # LED simula ventilador

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(HEATER, GPIO.OUT)
GPIO.setup(FAN, GPIO.OUT)

# Temperatura óptima
TEMP_MIN = 12
TEMP_MAX = 20

try:
    while True:
        # Simulación de temperatura aleatoria
        temperature = random.randint(5, 25)
        print(f"Temperatura actual: {temperature}°C")

        if temperature < TEMP_MIN:
            GPIO.output(HEATER, GPIO.HIGH)
            GPIO.output(FAN, GPIO.LOW)
        elif temperature > TEMP_MAX:
            GPIO.output(HEATER, GPIO.LOW)
            GPIO.output(FAN, GPIO.HIGH)
        else:
            GPIO.output(HEATER, GPIO.LOW)
            GPIO.output(FAN, GPIO.LOW)

        time.sleep(2)  # Espera 2 segundos antes de nueva lectura

except KeyboardInterrupt:
    GPIO.cleanup()