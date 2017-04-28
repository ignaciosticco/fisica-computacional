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

data = np.genfromtxt('resultado1a_redgrande.txt', delimiter = '\t')

L = data[:,0]
pc = data[:,1]
std = data[:,2]

coef_fit = np.polyfit(std,pc,1)
print(np.polyfit(std,pc,1,full=True))
x = np.linspace(0,max(std),200)
y = coef_fit[0]*x+coef_fit[1]


#################### PLOT specification ##############################

plt.plot(std,pc,'ko',markersize=3,label='k',zorder=3) 
plt.plot(x,y,'r')

pylab.xlabel('std')
pylab.ylabel('pc')
#pylab.ylim(2, 18)
#pylab.xlim(14, 20)
#pylab.xticks(np.arange(14,22,2))
#pylab.yticks(np.arange(2,20,4))
#pylab.show()
pylab.savefig('1c_2.eps', format='eps', bbox_inches='tight')
