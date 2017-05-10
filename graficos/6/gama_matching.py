import matplotlib.pyplot as plt
import pylab
import numpy as np
import math

golden_mean = (math.sqrt(5)-1.0)/2.0        # Aesthetic ratio
fig_width = 3+3/8 			    # width  in inches
fig_height = fig_width*golden_mean          # height in inches
fig_size =  [fig_width,fig_height]

params = {'backend': 'ps',
          'axes.titlesize': 8,
          'axes.labelsize': 9,
          'axes.linewidth': 0.5, 
          'axes.grid': True,
          'axes.labelweight': 'normal',  
          'font.family': 'serif',
          'font.size': 8.0,
          'font.weight': 'normal',
          'text.color': 'black',
          'xtick.labelsize': 8,
          'ytick.labelsize': 8,
          'text.usetex': True,
          'legend.fontsize': 8,
          'figure.dpi': 300,
          'figure.figsize': fig_size,
          'savefig.dpi': 300,
         }

pylab.rcParams.update(params)
#################### DATA ##############################

data = np.genfromtxt('resultado6_n128.txt', delimiter = '\t')

p = data[:,0]
m2 = data[:,1]
maximo =  max(m2)
lugar_max = m2.tolist().index(max(m2))-20
pc = p[lugar_max]
print("pc=", pc)
p = p -pc 

gama_mas=[]
gama_menos=[]
vector_p =[]

i=10
cant_puntos = 8
maximo_step = 60
while (i<maximo_step):
     gama_mas+=[np.polyfit(np.log(p[lugar_max+i+1:lugar_max+1+i+cant_puntos]),np.log(m2[lugar_max+1+i:lugar_max+1+i+cant_puntos]),1)[0]] 
     gama_menos+=[np.polyfit(np.log(abs(p[lugar_max-cant_puntos-i:lugar_max-i])),np.log(abs(m2[lugar_max-cant_puntos-i:lugar_max-i])),1)[0]]
     vector_p += [np.median(p[lugar_max+i+1:lugar_max+1+i+cant_puntos])]
     i+=1


#################### PLOT specification ##############################
'''
i=43
coef_fit_mas = np.polyfit(np.log(p[lugar_max+i+1:lugar_max+1+i+cant_puntos]),np.log(m2[lugar_max+1+i:lugar_max+1+i+cant_puntos]),1)
t_mas = np.linspace(np.log(p[lugar_max+i+1]),np.log(p[lugar_max+1+i+cant_puntos]),100)
plt.plot(t_mas,coef_fit_mas[0]*t_mas+coef_fit_mas[1],'r',linewidth=0.5,zorder=2)

i=22
coef_fit_menos = np.polyfit(np.log(abs(p[lugar_max-cant_puntos-i:lugar_max-i])),np.log(m2[lugar_max-cant_puntos-i:lugar_max-i]),1) 
t_menos = np.linspace(np.log(abs(p[lugar_max-cant_puntos-i])),np.log(abs(p[lugar_max-i])),100)
plt.plot(t_menos,coef_fit_menos[0]*t_menos+coef_fit_menos[1],'b',linewidth=0.5,zorder=2)

print("pend gama mas:",coef_fit_mas[0])
print("pend gama menos:",coef_fit_menos[0])

'''
resta = abs(np.subtract(gama_mas,gama_menos))
lugar_gama = resta.tolist().index(min(resta))
print(gama_mas[lugar_gama])


#plt.plot(vector_p,gama_mas,'ro',markersize=2,label='$\gamma_{+}$',zorder=3) 
#plt.plot(vector_p,gama_menos,'bo',markersize=2,label='$\gamma_{-}$',zorder=3) 
#plt.plot(np.log(abs(p)),np.log(m2),'wo',markersize=1,label='k',zorder=1)

pylab.xlabel('$ |p-p_c| $')
pylab.ylabel('$\gamma$')
#pylab.ylim(2, 18)
#pylab.xlim(14, 20)
#pylab.xticks(np.arange(14,22,2))
#pylab.yticks(np.arange(2,20,4))
#pylab.show()
plt.legend(loc='best',fontsize=6,numpoints=1)
#pylab.savefig('gama_matching1.eps', format='eps', bbox_inches='tight')
