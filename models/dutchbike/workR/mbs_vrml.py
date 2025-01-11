# -*- coding: utf-8 -*-

# vrml.PY
# @author: Nicolas Docquier
#
# Created on 14/10/2015
# Last update on 14/10/2015
#

#
# PyRobotran
# Copyright 2013, Universite catholique de Louvain, Belgium
# All rights reserved
#
#
# The PyRobotran software has been developed at UCL/iMMC/CEREM (Center 
# for Research in Mechatronics, institute of MEchanics, Materials and
# Civil engineering) for the Université catholique de Louvain (UCL).
#
# PyRobotran is a free software: you can redistribute it and/or modify it under the terms of
# the BSD 3-Clause License as published below.
#
# Redistribution and use in source and binary forms, with or without modification, are permitted
# provided that the following conditions are met:
#   * Redistributions of source code must retain the above copyright notice,
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#   * Neither the name of the Université catholique de Louvain nor the names of
#     its contributors may be used to endorse or promote products derived from
#     this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
# SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
# For any use in commercial products or questions about licensing, you can contact
#  - the Center for Research in Mechatronics at info@cerem.be
#  - the Louvain Technology Transfer Office of UCL at ltto@uclouvain.be
#


# This module defines a set of usefull functions for managing 3D geometries
# with vrml files to be introduced in the 3D model via MBsysPad


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#


import numpy as np

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#


def mbs_vrml_indexedfaceset(points,coordindices,color, edgecolor = None, transparency=0):
  """
 
  WRL = mbs_vrml_indexedfaceset(point,coordindex,color)
 
  Return a string that can be inserted in a vrml file to create an 
  "indexedFaceSet" node :
    - point is a n by 3 matrix containing the coordinate of n points
    - coordindex is a k by l matrix where k is the number of faces and l
      is the number of points per face. coordindex(i,:) contains the index of
      the point used to create the i-th face. 
      WARNING: first index is 0.
    - color is a 1 by 3 vector defining color of the indexedFaceSet
    - indent is the indentation to be added to each line to keep a good
      presentation of the vrml file
  """
  
  coordDefName =   ''
  
  
  # get the edges if there must be plotted
  if edgecolor is not None:
    facesetName = 'faceCoords'
    coordDefName =   'DEF '+facesetName
    edgeindices = []
    for tri in coordindices:
      edgeindices.append(tri[[0, 1]])
      edgeindices.append(tri[[1, 2]])
      edgeindices.append(tri[[2, 0]])
  
  
  indent='    ';

  # write the header part of the faceset
  wrl = indent + 'Shape {\r\n' \
      + indent + '    geometry IndexedFaceSet {\r\n' \
      + indent + '       coord '+coordDefName+' Coordinate{\r\n' \
      + indent + '           point [\r\n' \

  # write the point list    
  for point in points:
    #wrl += indent + '               {} {} {},\r\n'.format(point[0],point[1],point[2]);
    wrl += indent + '               %f %f %f,\r\n'%(point[0],point[1],point[2]);

  wrl += '           ]\r\n'
  wrl += '       }\r\n'
  wrl += '       coordIndex [\r\n'
      
  # write the face list (index of point)    
  for faceci in coordindices:
    wrl += indent + '           '
    for ci in faceci:
      wrl += indent +'%d'%(ci)
      
    wrl += ' -1,\r\n'

  # write the footer part of the faceset
  wrl += '       ]\r\n'
  wrl += '       creaseAngle 0.1\r\n'
  wrl += '       solid FALSE\r\n'
  wrl += '   }\r\n'
  wrl += '   appearance Appearance {\r\n'
  wrl += '       material Material {\r\n'
  #wrl += '           diffuseColor {} {} {}'.format(color[0], color[1], color[2]) + '\r\n'
  wrl += '           diffuseColor %f %f %f'%(color[0], color[1], color[2]) + '\r\n'
  wrl += '           transparency {}'.format(transparency) + '\r\n'
  wrl += '       }\r\n'
  wrl += '   }\r\n'
  wrl += '}\r\n'



  # plot lines if needed
  if edgecolor is not None:

    # write the header part of the faceset
    wrl += indent + 'Shape {\r\n' \
         + indent + '    geometry IndexedLineSet {\r\n' \
         + indent + '       coord USE '+facesetName+' \r\n' 
    wrl += indent + '       coordIndex [\r\n'
        
    # write the face list (index of point)    
    for index in edgeindices:
      wrl += indent + '           '
      for ci in index:
        wrl += indent +'{}'.format(ci)
        
      wrl += ' -1,\r\n'

    # write the footer part of the faceset
    wrl += '       ]\r\n'
    wrl += '   }\r\n'
    wrl += '   appearance Appearance {\r\n'
    wrl += '       material Material {\r\n'
    wrl += '           emissiveColor {} {} {}'.format(edgecolor[0], edgecolor[1], edgecolor[2]) + '\r\n'
    wrl += '       }\r\n'
    wrl += '   }\r\n'
    wrl += '}\r\n'
      
  return wrl
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#


def mbs_vrml_indexedlineset(points,edgeindices, edgecolor):
    """

    WRL = mbs_vrml_indexedlineset(point,coordindex,color)

    Return a string that can be inserted in a vrml file to create an 
    "indexedFaceSet" node :
    - point is a n by 3 matrix containing the coordinate of n points
    - edgeindices 
    - color is a 1 by 3 vector defining color of the indexedFaceSet
    - indent is the indentation to be added to each line to keep a good
      presentation of the vrml file
    """
  
    wrl = ''
    indent='    ';


    # write the header part of the faceset
    wrl += indent + 'Shape {\r\n' 
    wrl += indent + '    geometry IndexedLineSet {\r\n' 
    wrl += indent + '       colorPerVertex FALSE\r\n' 
    wrl += indent + '       coord  Coordinate{\r\n' 
    wrl += indent + '           point [\r\n' 

    # write the point list    
    for point in points:
      #wrl += indent + '               {} {} {},\r\n'.format(point[0],point[1],point[2]);
      wrl += indent + '               %f %f %f,\r\n'%(point[0],point[1],point[2]);

    wrl += '               ]\r\n'
    wrl += '           }\r\n'
    wrl += indent + '       color  Color{\r\n' 
    wrl += indent + '           color [\r\n' 

    wrl += indent + '               %f %f %f,\r\n'%(edgecolor[0], edgecolor[1], edgecolor[2]);

    wrl += '                ]\r\n'
    wrl += '            }\r\n'
    wrl += indent + '       coordIndex [\r\n'
        
    # write the line list (index of point)    
    for index in edgeindices:
      wrl += indent + '           '
      for ci in index:
        wrl += indent +'{}'.format(ci)
        
      wrl += ' -1,\r\n'

    # write the footer part of the lineset
    wrl += '           ]\r\n'
    wrl += indent + '       colorIndex [\r\n'
        
    # write the line list (index of color)    
    wrl += indent + '           '
    for index in edgeindices:
      wrl += '{}'.format(0)
      wrl += ' ,'
    wrl += '\r\n'
    # write the footer part of the lineset
    wrl += '           ]\r\n'
    wrl += '        }\r\n'  # end of IndexedLineSet
    wrl += '        appearance Appearance {\r\n'
    wrl += '            material Material {\r\n'
    wrl += '               diffuseColor {} {} {}'.format(edgecolor[0], edgecolor[1], edgecolor[2]) + '\r\n'
    wrl += '            }\r\n'
    wrl += '        }\r\n'
    wrl += '}\r\n'
      
    return wrl

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#

def mbs_vrml_extrude(points,path,color, fid):
    """"
       Print the representation of an extruded shape in the given file
      - points is a n by 2 numpy array containing the coordinates of n points
      - path is a n by 3 numpy array containing coordinates of points that 
        define the path along which the shape is extruded
        WARNING: the VRML loader used in Robotran accept only
                 extrusion along y.
      - color is a 1 by 3 vector defining color of the indexedFaceSet
      - indent is the indentation to be added to each line to keep a good
        presentation of the vrml file
      - fid is the stream in which to print the result
   
    """

    indent='    ';

    wrl = indent + 'Shape {\r\n'\
        + indent + '    geometry Extrusion {\r\n'\
        + indent + '        crossSection [\r\n'
    fid.write(wrl);

    for pt in points:
        fid.write(indent + '               %f %f,\r\n' % (pt[0],pt[1]));

    wrl = indent + '       ]\r\n'\
        + indent + '       spine [\r\n';
    fid.write(wrl);

    for p in path:
        wrl =  indent + '           ';
        wrl += indent + '%f %f %f,\r\n'%(p[0], p[1], p[2]);
        fid.write(wrl);

    wrl = indent + '       ]\r\n'\
        + indent + '       creaseAngle 0.9\r\n'\
        + indent + '       solid FALSE\r\n'\
        + indent + '   }\r\n'\
        + indent + '   appearance Appearance {\r\n'\
        + indent + '       material Material {\r\n'\
        + indent + '           diffuseColor %f %f %f \r\n'%(color[0], color[1], color[2])\
        + indent + '       }\r\n'\
        + indent + '   }\r\n'\
        + indent + '}\r\n';
    fid.write(wrl);

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#
#                                                                      #
#              Specific functions for wheel and rail                   #
#                                                                      #
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#

def mbs_vrml_rail_profile(filename, profile, w=None, axisPt=None):
    """ Export a rail profile to a VRML file
   
        input:
         profile(y): a function defining the rail profile
            the profile function is assumed to use SI units (i.e. meter)
         w: a numpy array defining points along the lateral axis for which
            the profile must be drawn
            the resolution of w define the resolution of the vrml file
         axisPt: a numpy array defining the points for which a section of the rail is drawn
            axisPt(1) and axisPt(end) define the start and end points of the
            shape
       
        example:
        mbs_vrml_rail_profile(@UIC60, 'w', 753.5119257e-3+[-37:37]'*1e-3, 'axisPt', [-1e3:10:1e3])
    """

    #* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    # default value for optional arguments

    color = [.8,.8,.8]

    if w is None:
        w = (np.arange(-50,50,2)+750)*1e-3

    if axisPt is None:
        axisPt = np.array([-500, 0, 500])*1e-3


    #* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    # compute the rail profile
    (zr,_,_) = profile(w)
    
    #* * * * * * * * * * * * * * * * * * * * * * * * * * * *
    # write the vrml file

    fid = open(filename,'w+');

    fid.write('#VRML V2.0 utf8\r\n');
    fid.write('\r\n');
    
    fid.write('Transform {\r\n');
    fid.write('    rotation 0 0 1 1.5707963267948966\r\n')
    fid.write('    children [\r\n')

    mbs_vrml_extrude(np.array([w,zr]).T,
                     np.array([[0, axisPt[0], 0] , [0, axisPt[-1], 0]]),
                     [0.8,0.8,0.8], fid);

    for aPt in axisPt:
        wrl = mbs_vrml_indexedfaceset(np.array([w, aPt*np.ones(w.shape), zr]).T,
                                     [np.arange(w.size)],
                                     color)
        fid.write('\r\n');
        fid.write(wrl);
        
        
    fid.write('    ]')    
    fid.write('}')    
    fid.close();


# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *#


def mbs_vrml_wheel_profile(filename, profile, rnom = 0.415, w = None, angularResolution = 100):
    """
    export a wheel profile to a VRML file

    input:
    profile(y): a function defining the wheel profile
       the profile function is assumed to use SI units (i.e. meter)
    arguments (must be given as a name-value pair, ex: ,'nPtRev', 100
    w: a column vector defining points along the wheel axis for which
       the profile must be drawn
       the reolsution of w define the resolution of the vrml file
    nPtRev: define the number of face per revolution

    """


    if w is None:
        w = (np.arange(-70,50,2))*1e-3

    theta = np.linspace(0, 2*np.pi, angularResolution, endpoint=False)
    #pas = 1./angularResolution;
    #theta = np.arange(0,1-pas,pas)*2*pi;

    (zw, _, _) = profile(w)

    # * * * * * * * * * * * * * * * * * * * * * * * * *
    # compute the points and the faces of the geometry

    rho = -rnom-zw;
    r0 = -0.2
    x = np.hstack((r0, rho, r0))
    y = np.hstack((w[0], w, w[-1]))

    nt = theta.size;
    n = x.size;
    nfaces = (n-1)*(nt);
    ncoord = n*nt;
    coords = np.zeros((ncoord, 3));
    faces = np.zeros((nfaces, 4));

    for i in range(nt):
        ct = np.cos(theta[i]);
        st = np.sin(theta[i]);    
        for j in range(n-1):
            coords[i*n+j,:] = [x[j]*ct, y[j], x[j]*st];
            faces[i*(n-1)+j,:] = np.array([i*n+j, (i+1)*n+j, (i+1)*n+j+1, i*n+j+1])%ncoord;
        
        # compute last point coords on the line
        coords[i*n+n-1,:] = [x[n-1]*ct, y[n-1], x[n-1]*st];
    


    # * * * * * * * * * * * * * * * * * * * * * * * * * * * *
    # save the geometry to the VRML file

    fid = open(filename,'w+');
    
    fid.write('#VRML V2.0 utf8\r\n')
    fid.write('\r\n')
    vrml = mbs_vrml_indexedfaceset(coords,faces,[0.8,0.8,0.8]);
    fid.write(vrml)
    fid.close()



#% * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#% save the geometry to the VRML file

#[filename, pathname] = uiputfile(...
        #{'*.wrl','VRML-files (*.wrl)'},...
         #'Choose an wrl-file name to save','*.wrl');
#if(filename ~= 0)
    #fname = fullfile(pathname, filename);
    
#else
    #disp('vrml_2D_profile >> No File Saved !');
    #return
#end




