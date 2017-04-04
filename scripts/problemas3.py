
def collatz(n,contador):

	if n==1:
		return n,contador
	if n%2==0:
		n=n/2
		solucion = collatz(n,contador+1)[0]
		contador = collatz(n,contador+1)[1]
		#print(solucion)
		if solucion==1:
			return 1,contador

	if n%2==1:
		n=3*n+1
		solucion = collatz(n,contador+1)[0]
		contador = collatz(n,contador+1)[1]
		if solucion==1:
			return 1,contador

print(collatz(13,1)[1])

#maximo=0
for i in range(1,13):
	

