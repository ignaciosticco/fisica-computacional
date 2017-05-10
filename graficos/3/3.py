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

data = np.genfromtxt("resultado3.txt", delimiter = '\t')

size = data[:,0]
masa_cperco = data[:,1]

log_size = np.log(size)
log_masa_cperco = np.log(masa_cperco)

coef_fit = np.polyfit(log_size,log_masa_cperco,1)
print(np.polyfit(log_size,log_masa_cperco,1,full=True))
x = np.linspace(0,max(log_size),500)
y = coef_fit[0]*x+coef_fit[1]

def rsquared(x, y):
    """ Return R^2 where x and y are array-like."""

    slope, intercept, r_value, p_value, std_err = scipy.stats.linregress(x, y)
    return r_value**2
print (rsquared(log_size,log_masa_cperco))

#################### PLOT specification ##############################

plt.plot(log_size,log_masa_cperco,'ko',markersize=2,label='k',zorder=3) 
plt.plot(x,y,'r')

pylab.xlabel('log~(L)')
pylab.ylabel('log~($M_{p}$)')
#pylab.ylim(2, 18)
#pylab.xlim(14, 20)
#pylab.xticks(np.arange(14,22,2))
#pylab.yticks(np.arange(2,20,4))
#pylab.show()
pylab.savefig('3_1.eps', format='eps', bbox_inches='tight')
