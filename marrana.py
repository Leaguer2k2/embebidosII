# Parte 1: Sumar n números
def sumar_numeros():
    print("---- SUMAR N NÚMEROS ----")
    n = int(input("¿Cuántos números quieres sumar? "))
    suma = 0
    for i in range(n):
        numero = float(input(f"Ingrese el número {i + 1}: "))
        suma += numero
    print(f"La suma total es: {suma}\n")


# Parte 2: Invertir un número (ejemplo: 619 → 916)
def invertir_numero():
    print("---- INVERTIR UN NÚMERO ----")
    numero = input("Escribe un número para invertirlo: ")
    invertido = numero[::-1]
    print(f"El número invertido es: {invertido}\n")


# Parte 3: Pedir nombre, edad y profesión, y mostrar mensaje
def mostrar_info_usuario():
    print("---- INFORMACIÓN DEL USUARIO ----")
    nombre = input("¿Cómo te llamas? ")
    edad = input("¿Cuántos años tienes? ")
    profesion = input("¿Cuál es tu profesión? ")

    print(f"\n¡Hola {nombre}! Tienes {edad} años y eres {profesion}.\n")


# Parte 4: Pedir x números y mostrar solo los que no se repiten
def mostrar_valores_unicos():
    print("---- VALORES ÚNICOS ----")
    x = int(input("¿Cuántos números vas a ingresar? "))
    numeros = []

    for i in range(x):
        num = input(f"Ingrese el número {i + 1}: ")
        numeros.append(num)

    unicos = list(set(numeros))
    print(f"Los valores únicos son: {unicos}\n")


# Programa principal: ejecutar todo
def main():
    sumar_numeros()
    invertir_numero()
    mostrar_info_usuario()
    mostrar_valores_unicos()


# Iniciar programa
main()
