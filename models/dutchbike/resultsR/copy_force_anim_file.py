import numpy as np
import matplotlib.pyplot as plt
import os
import sys
import shutil

from scipy import interpolate

abspath = os.path.abspath(sys.argv[0])
os.chdir(os.path.dirname(abspath))


res_names = ['BikeDemo_Speed01_LoadOff_StiffWheel']#,
             # 'BikeDemo_Speed01_LoadAtFront_StiffWheel',
             # 'BikeDemo_Speed01_LoadAtBack_StiffWheel'];

force_files = ["xFrc_xFrc_wheel_front", "xFrc_xFrc_wheel_rear"]

for r  in res_names:
    # interpolate res file for each force
    for ff in force_files:
        
        #load res file
        res_file = f"{r}_{ff}.res"
        
        # load res file
        frc_res = np.loadtxt(res_file)
        t_fin = frc_res[-1,0]
        
        # determine time vector for interpolation
        frame_rate = 1000 # number of frame per second for the animation file
        dt = 1./frame_rate 
        t_anim = np.arange(0, t_fin, dt)

        # interpolate
        frc_anim = interpolate.interp1d(frc_res[:,0], frc_res, axis=0)(t_anim) # Interpolation of forces and time
        
        # save anim file
        anim_file = f"../animationR/{r}_{ff}.anim"
        np.savetxt(anim_file,frc_anim)
