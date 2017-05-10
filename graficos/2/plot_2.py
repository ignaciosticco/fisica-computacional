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

data1 = np.genfromtxt("resultado2_n4_1.txt", delimiter = '\t')
data2 = np.genfromtxt("resultado2_n16_1.txt", delimiter = '\t')
data3 = np.genfromtxt("resultado2_n32_1.txt", delimiter = '\t')
data4 = np.genfromtxt("resultado2_n64_1.txt", delimiter = '\t')
data5 = np.genfromtxt("resultado2_n128_2.txt", delimiter = '\t')

p1 = data1[:,0]
fuerza_cp_1 = data1[:,1]
p2 = data2[:,0]
fuerza_cp_2 = data2[:,1]
p3 = data3[:,0]
fuerza_cp_3 = data3[:,1]
p4 = data4[:,0]
fuerza_cp_4 = data4[:,1]
p5 = data5[:,0]
fuerza_cp_5 = data5[:,1]

#################### PLOT specification ##############################

plt.plot(p1,fuerza_cp_1,'b',label='$L=4$',lw=0.6,zorder=2)
plt.plot(p2,fuerza_cp_2,'r',label='$L=16$',lw=0.6,zorder=2)
plt.plot(p3,fuerza_cp_3,'g',label='$L=32$',lw=0.6,zorder=2)
plt.plot(p4,fuerza_cp_4,'c',label='$L=64$',lw=0.6,zorder=2)
plt.plot(p5,fuerza_cp_5,'k',label='$L=128$',lw=0.6,zorder=2)


pylab.xlabel('$p$')
pylab.ylabel('P')
#pylab.ylim(2, 18)
#pylab.xlim(14, 20)
#pylab.xticks(np.arange(14,22,2))
#pylab.yticks(np.arange(2,20,4))
#pylab.show()
plt.legend(loc='best',labelspacing=-0.1,borderpad=0.3,handletextpad=0.5,fontsize=6,numpoints=1)
pylab.savefig('2_1.eps', format='eps', bbox_inches='tight')
