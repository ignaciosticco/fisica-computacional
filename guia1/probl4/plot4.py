# Sacado de https://scipy.github.io/old-wiki/pages/Cookbook/Matplotlib/LaTeX_Examples.html
import pylab
import numpy as np
import matplotlib.pyplot as plt
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

##def fzeta(a,b,c):
##	f=a/(b*c)
##	return f

def fzeta(a,b):
		f=a/b
		return f

for i in range(0,100):
	i=str(i)

	data = np.genfromtxt('resultado4_%s.txt'%i, delimiter = '\t')
	prob = data[:,0]
	s = data[:,1]
	ns = data[:,2]
	q0 = data[:,3]
	stao = data[:,4]
	zeta = data [:,5]

	data2= np.genfromtxt('nscritico.txt', delimiter = '\t')
	nsc=data2[:,0]

#print (len(ns))
#print (len(nsc))

	

	

	ns=ns[5:100:1]	
	zeta=zeta[5:100:1]
	stao=stao[5:100:1]
	q0=q0[5:100:1]
	nsc=nsc[5:100:1]
	#f=fzeta((ns/(500*64*64)),q0,stao)
	f=fzeta(ns,nsc)
#################### PLOT specification ##############################
	plt.plot(zeta,f,'ko',markersize=1,label='k',zorder=3)
	
pylab.ylabel('f(z)')
pylab.xlabel('z')
#pylab.ylim(2, 18)
pylab.xlim(-5, 5)
#pylab.show()
pylab.savefig('fig_4.eps', format='eps', bbox_inches='tight')
