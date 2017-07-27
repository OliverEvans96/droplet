# plotSubstrate2D.py
# Oliver Evans
# 11-15-2016

# Plot substrate COM as calculated by substratePosition.cpp as a function of x

# Plot for each time step

# Don't require X server for generating plots
import matplotlib
matplotlib.use('Agg')

from sys import argv
from matplotlib.pyplot import *
from numpy import *
from mpl_toolkits.mplot3d import Axes3D
from os import chdir,mkdir

# Arg 1: 30A/atom4
chdir('/home/oge1/lammps/sapphire/analysis/results/{}'.format(argv[1]))

# Make image directory
try:
	mkdir('img/substrate')
	print('Created image directory.')
except(OSError):
	print('Image directory exists.')

# Load data
data = genfromtxt('substrate_position.txt')

# Load simulation box bounds
with open('substrate_position.txt') as f:
	for i,line in enumerate(f):
		print((i,line.split()))
		if i == 1:
			dx = float(line.split()[-1])
		elif i == 2:
			xbounds = array(line.split()[-2:],dtype=float)
		elif i == 3:
			zbounds = array(line.split()[-2:],dtype=float)
		elif i > 3:
			break

# Separate first column (timestep number) from substrate position
ts = data[:,0].astype(int)
z_sim = data[:,1:]

# Number of timesteps & x bins
nsteps,nx = shape(z_sim)

# Create x grid
xmin = xbounds[0]
xmax = xmin + nx*dx
x = linspace(xmin,xmax,nx)

# Convert z from simulation units [0,1] to physical units (Angstroms)
z = zbounds[0] + z_sim*diff(zbounds)

# Create figure
fig = figure(figsize=[12,8])
ax = gca()

# Loop through timesteps
for i,step in enumerate(ts):
	# Print step number
	print('Step {}/{}: {}'.format(i,nsteps,step))

	# Clear axes
	cla()

	# Plot surface
	plot(x,z[i,:])

	# Title & stuff
	title('Substrate: {} : {}ps'.format(argv[1],step/1000))
	xlim(xbounds)
	ylim(-40,40)
	xlabel('$x (\AA)$')
	ylabel('$z (\AA)$')

	# Save figure
	savefig('img/substrate/substrate{:08d}.png'.format(step))

print('Done!')
