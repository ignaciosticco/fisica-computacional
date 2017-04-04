import math


a = 1
b = 0
fibo = 0
suma_pares = 0
while fibo < 4000000:
	fibo = b + a 
	b = a
	a = fibo
	if fibo % 2 == 0:
		suma_pares = fibo + suma_pares
print  (suma_pares)