# File Name: flowfield.py
# Description: Visualize vector field with arrows for one part of one sim
# Created: Fri Feb 24, 2017 | 11:25pm EST
# Last Modified: Sat Feb 25, 2017 | 02:35pm EST

#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
#                           GNU GPL LICENSE                            #
#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-
#                                                                      #
# Copyright Oliver Evans 2017 <oliverevans96@gmail.com>                #
#                                                                      #
# This program is free software: you can redistribute it and/or modify #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# This program is distributed in the hope that it will be useful,      #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with this program. If not, see <http://www.gnu.org/licenses/>. #
#                                                                      #
#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-

from numpy import *
import matplotlib
matplotlib.use('Agg')
from matplotlib.pyplot import *
from cmocean import cm
from sys import argv
from os import system
from glob import glob


# Color is angle,
# Magnitude by arrow size
def vfld(prefix,outloc=None,alpha=1,scale=1,xmin=0,xmax=1,ymin=0,ymax=1,skip=15):
    xfield = loadtxt(fl)
    yfield = loadtxt(fl.replace('xfield','yfield'))
    fieldnorm = sqrt(xfield**2+yfield**2)
    #unitxfield = xfield/fieldnorm
    #unityfield = yfield/fieldnorm 

    X = linspace(xmax,xmin,xfield.shape[0])
    Y = linspace(ymin,ymax,xfield.shape[1])
    x,y=meshgrid(X,Y,indexing='ij')
    
    angle = arctan2(xfield[::-skip,::skip],yfield[::-skip,::skip])

    xs = x[::skip,::skip]
    ys = y[::skip,::skip]
    xfs = xfield[::-skip,::skip]
    yfs = yfield[::-skip,::skip]
    fn = fieldnorm[::skip,::skip]
    
    vx = xfs * ((xfs**2+yfs**2)**alpha/(xfs**2+yfs**2))**.5
    vy = yfs * vx / xfs

    quiver(xs,ys,vx,vy,angle,cmap=cm.phase,scale=scale)
    
    #quiver(x[::skip,::skip],y[::skip,::skip],xfield[::-skip,::skip],yfield[::-skip,::skip],angle)
    if outloc != None:
        print("Saving to {}".format(outloc))
        savefig(outloc,dpi=160)

# Parse arguments
# e.g. python flowfield.py 40A 1
sim, part = argv[1:]

prefix = '/home/oge1/lammps/sapphire/analysis/results/'

# Create image for each frame in this part of this sim
path = prefix + sim + '/atom' + part + '/'

# Make sure output directory exists
system('mkdir -p '+path+'img/vfield')
flist = glob(path+'fields/x*')

# Create figure
figure(figsize=[8,6])

# Plot bounds
xlo = 0
xhi = sqrt(125500/pi)
ylo = 10
yhi = 142

# Loop through all frames
for i,fl in enumerate(flist):
    print("VF: Frame {}".format(i))

    outname = (fl.split('/')[-1].split('.')[0]+'.png').replace('xfield','vfield')
    tstep = outname.split('.')[0][6:]
    outpath = path+'img/vfield/'+outname

    clf()
    xlabel('$r\,\,(\AA)$')
    ylabel('$z\,\,(\AA)$',rotation='horizontal',labelpad=15)
    title('Flow Field: {}, atom{}. $t=${:5d} ps'.format(sim,part,int(int(tstep)/1000)))

    vfld(fl,outloc=outpath,
            alpha=0.5, scale=0.7, xmin=xlo, xmax=xhi, ymin=ylo, ymax=yhi)

