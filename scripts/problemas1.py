import math 

mult_5=0
sum_mult_5=0
i=1
while mult_5<10:
	mult_5 = 5*i
	sum_mult_5 = mult_5+sum_mult_5
	i+=1
	mult_5 = 5*i
print(sum_mult_5)

mult_3=0
sum_mult_3=0
i=1
while mult_3<10:
	#print(mult_3)
	mult_3 = 3*i
	sum_mult_3 = mult_3+sum_mult_3
	i+=1
	mult_3 = 3*i
print(sum_mult_3)

suma_total = sum_mult_3+sum_mult_5
print(suma_total)

# ACAAAAAAAAAAAAAAAAAAAa