""" Read the ground profile from the userfctR library and create
    a corresponding vrml file.

    The project_userfct shared library must be defined

    (c) Quandyga Engineering
"""

import numpy as np

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import ctypes
from numpy.ctypeslib import ndpointer

import mbs_vrml

import os

# make the script directory as the current directory
os.chdir(os.path.dirname(os.path.realpath(__file__)))

# name of the ground profile function
ground_function = 'speed_bump_ground' #'slope_ground' #'sinus_ground'  # 'sinus_quad_groun'

# load the dynamic library containing the ground profile functions
lib_c = np.ctypeslib.load_library("Project_user.dll","./build/Debug/")


# access to the ground profile function using ctypes
ground_fun = getattr(lib_c, ground_function)
ground_fun.argtypes = [ndpointer(ctypes.c_double, flags="C_CONTIGUOUS"),
                               ctypes.POINTER(ctypes.c_double),
                               ctypes.POINTER(ctypes.c_double),
                               ctypes.POINTER(ctypes.c_double)]
ground_fun.restype = ctypes.c_int

# function arguments in ctypes
x_I = np.zeros(4, dtype=np.double)
mu = ctypes.c_double(1.0)
mupx = ctypes.c_double(0.0)
mupy = ctypes.c_double(0.0)

# meshgrid to define points where to compute the ground profile
x = np.arange(-5, 150, .2)
y = np.arange(-5, 5, .2)
yv, xv = np.meshgrid(y, x)

nx = len(x)
ny = len(y)

zv = np.zeros(xv.shape)

# compute the ground profile calling the C-function
for i in range(zv.shape[0]):
    for j in range(zv.shape[1]):
        x_I[1] = xv[i, j]
        x_I[2] = yv[i, j]
        res = ground_fun(x_I, mu, mupx, mupy)
        zv[i, j] = mu.value

# plot the ground profile
# -----------------------
# fig = plt.figure()
# ax = plt.axes(projection='3d')
# surf = ax.plot_surface(xv, yv, zv)
# plt.show()

# generate the vrml
# -----------------
points = np.column_stack((xv.flatten(), yv.flatten(), zv.flatten()))
coordindices = np.zeros([(nx-1)*(ny-1), 4], dtype=int)
for i in range(nx-1):
    for j in range(ny-1):
        coordindices[i*(ny-1)+j] = [i*ny+j, i*ny+j+1, (i+1)*ny+j+1, (i+1)*ny+j]
gridindices = []
for i in range(nx):
    gridindices.append(np.arange(ny)+i*ny)
for j in range(ny):
    gridindices.append(np.arange(nx)*ny+j)

#vrml_mesh = mbs_vrml.mbs_vrml_indexedfaceset(points, coordindices, [.4, .4, .4], edgecolor = [.2, .2, .9], transparency=.8)
vrml_mesh = mbs_vrml.mbs_vrml_indexedfaceset(points, coordindices, [.4, .4, .4], transparency=.8)
# generate a grid representing the ground
vrml_grid = mbs_vrml.mbs_vrml_indexedlineset(points, gridindices, [.5, .5, .9])

fid = open(f'../animationR/vrml/{ground_function}.wrl','w+');
fid.write('#VRML V2.0 utf8\r\n')
fid.write('\r\n')
fid.write(vrml_mesh)
fid.write(vrml_grid)
fid.close()
