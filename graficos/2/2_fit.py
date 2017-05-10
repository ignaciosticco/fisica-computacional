import matplotlib.pyplot as plt
import pylab
import numpy as np
import math
import scipy
from   scipy import stats

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

data = np.genfromtxt("resultado2_n128_3.txt", delimiter = '\t')


p = data[:,0]
fuerza_cp = data[:,1]

pc=0.5927
proba = []
fcp = []
i=0
while i<len(p):
     if p[i]>pc:
          proba+= [np.log(p[i]-pc)]
          fcp+= [np.log(fuerza_cp[i])]
     i+=1

coef_fit = np.polyfit(proba[:6],fcp[:6],1)

x = np.linspace(proba[0],proba[6],100)
y = coef_fit[0]*x+coef_fit[1]
print(coef_fit[0])

def rsquared(x, y):
    """ Return R^2 where x and y are array-like."""

    slope, intercept, r_value, p_value, std_err = scipy.stats.linregress(x, y)
    return r_value**2
print (rsquared(proba[:6],fcp[:6]))



#################### PLOT specification ##############################

plt.plot(proba,fcp,'ko',label='$n=128$',markersize=2,zorder=2)
plt.plot(x,y,'r',linewidth=1,zorder=2)

pylab.xlabel('log($p-p_c$)')
pylab.ylabel('log~(P)')
#pylab.ylim(2, 18)
#pylab.xlim(14, 20)
#pylab.xticks(np.arange(14,22,2))
#pylab.yticks(np.arange(2,20,4))
#pylab.show()
#plt.legend(loc='right',labelspacing=-0.1,borderpad=0.3,handletextpad=0.5,fontsize=6,numpoints=1)
pylab.savefig('2_fit.eps', format='eps', bbox_inches='tight')
