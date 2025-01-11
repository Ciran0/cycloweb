import numpy as np
import matplotlib.pyplot as plt
import os
import sys

# change color of text to white or black, depending on background
text_col = "w" #k"
params = {"ytick.color" : text_col,
          "xtick.color" : text_col,
          "axes.labelcolor" : text_col,
          "axes.edgecolor" : text_col}
plt.rcParams.update(params)

abspath = os.path.abspath(sys.argv[0])
os.chdir(os.path.dirname(abspath))

id_y = 3;
id_phi = 5;
id_fork = 11;

res_names = ['BikeDemo_Speed01_LoadOff_StiffWheel',
             'BikeDemo_Speed01_LoadAtFront_StiffWheel',
             'BikeDemo_Speed01_LoadAtBack_StiffWheel'];

leg_txt = ['No load', 'Load at front', 'Load at back']

cols = ['g','r','b']
ls = ['-','--','-.']

fig, axs = plt.subplots(3,1, sharex=True, figsize=(5, 6.))

fig.patch.set_alpha(0.0)

for i, rn  in enumerate(res_names):
    q = np.loadtxt('./'+rn+'_q.res')
    Qq = np.loadtxt('./'+rn+'_Qq.res')


    ax_phi = axs[0]
    ax_phi.plot(q[:,0],q[:,id_phi]/np.pi*180., color=cols[i], linestyle=ls[i])
    ax_y = axs[1]
    ax_y.plot(q[:,0],q[:,id_y], color=cols[i], linestyle=ls[i])
    ax_trq = axs[2]
    ax_trq.plot(Qq[:,0],Qq[:,id_fork], color=cols[i], linestyle=ls[i])

ax_phi.set_ylabel('Roll [deg]')
ax_y.set_ylabel('Lat. displ. [m]')
ax_trq.set_ylabel('Steering trq. [Nm]')
ax_trq.set_xlabel('Time [s]')
ax_phi.legend(leg_txt)


ax_trq.set_xlim(0,17.5)

fig.tight_layout() 

plt.ion()
plt.show()
plt.savefig('LoadComparison.svg', transparent=False,bbox_inches='tight')
plt.savefig('LoadComparison.png', transparent=False,bbox_inches='tight')
