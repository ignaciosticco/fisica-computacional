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

data = np.genfromtxt("resultado1d_prob0.592700_n1024", delimiter = '\t')

'''
s = data[:1500,0]
ns =data[:1500,1]
log_s=np.log(s)
log_ns=np.log(ns)
'''
log_s=[]
log_ns=[]


i=0
while (data[i,1]>250):
	log_s+=[np.log(data[i,0])]
	log_ns+=[np.log(data[i,1])]
	i+=1


coef_fit = np.polyfit(log_s,log_ns,1)
print(coef_fit)
x = np.linspace(0,max(log_s),1000)
y = coef_fit[0]*x+coef_fit[1]

#################### PLOT specification ##############################

plt.plot(log_s,log_ns,'ko',markersize=1,label='k',zorder=3) 
plt.plot(x,y,'r')

pylab.xlabel('log(s)')
pylab.ylabel('log($n_s$)')
#pylab.ylim(2, 18)
#pylab.xlim(14, 20)
#pylab.xticks(np.arange(14,22,2))
#pylab.yticks(np.arange(2,20,4))
#pylab.show()
pylab.savefig('1d_1.eps', format='eps', bbox_inches='tight')
