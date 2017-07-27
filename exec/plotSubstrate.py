# plotSubstrate.py
# Oliver Evans
# 11-14-2016

# Plot substrate COM as calculated by substratePosition.cpp using python.

# Only plot first timestep

# Don't require X server for generating plots
import matplotlib
matplotlib.use('Agg')

from sys import argv
from matplotlib.pyplot import *
from numpy import *
from mpl_toolkits.mplot3d import Axes3D
from os import chdir

# Arg 1: 30A/atom4
chdir('/home/oge1/lammps/sapphire/analysis/results/{}'.format(argv[1]))

# Only load first timestep
f = open('substrate_position.txt')
ts_lines = []
read = False
data=[]
for lineNum,line in enumerate(f):
	if 'TIMESTEP' in line:
		ts_lines.append(lineNum)
		if read:
			break
		read = ~read
		data.append

print(ts_lines)

if len(ts_lines)>1:
	Z = genfromtxt('substrate_position.txt',skip_header=ts_lines[0],max_rows=ts_lines[1]-ts_lines[0])
else:
	Z = genfromtxt('substrate_position.txt',skip_header=ts_lines[0])

# Ignore first and last three rows and columns
Z = Z[3:-3,:]

print("Shape: {}".format(shape(Z)))

# Create grid the same size as Z
x,y = [arange(n) for n in shape(Z)]
X,Y = meshgrid(x,y,indexing='ij')

# Create figure
fig = figure(figsize=[12,8])
#ax = gca(projection='3d')
ax = gca()

# Plot surface
#ax.plot_wireframe(X,Y,Z)
contourf(X,Y,Z)
colorbar()

# Title & stuff
title('Substrate: {}'.format(argv[1]))
ax.set_xlabel('x')
ax.set_ylabel('y')
#ax.set_zlabel('z')

# Save figure
savefig('surf.png')

print('Done!')
