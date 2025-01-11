[UpdateST181124]
Compilation instructions have been changed after mbsyscv1.26 and higher ! 
Some information below may be obsolete.

To use:

Current version show how to use two .conf files and different resname and path.
Compile with 

```
cmake .. -DCMAKE_PREFIX_PATH="XXXX/MBsysC/install"
make install
```

and run with (go to install folder )

```
.\exe_DutchBike.exe -c "../../dataR/conf2/DutchBike.conf"
```

Example of plot can be found in resultsR/plot_res.py

-------

# DutchBike multibody model

This model enables to simulate and analyze the dynamics of a bike.

The bike is composed of several rigid bodies
 * A _Frame_ with 6 degrees of freedom (dof) with respect to the 
   groune
 * The _Fork_ with 1 dof (revolute) with respect to the frame
 * The _FrontWheel_ with 1 dof (revolute) with respect to the _Fork_
 * The _RearWheel_ with 1 dof (revolute) with respect to the _Frame_
 * The _Rider_ composed of several boies fixed with respect to each
   other, the first one being fixed to the _Frame_ at the level
   of the saddle. NB: the mass of the _Rider_ is null, all the mass
   being encompassed on the _Frame_
 * A _Box_ rigidly fixed to the frame and that can be moved at different
   places to represent a cargo.
   
The model is implemented in C-code usgin symbolic equations generated
with [Robotran](www.robotran.be) multibody software.
   
## Installing and compiling

1. Install Robotran
    * Download Robotran from: https://www.robotran.be/download/
    * Follow instructions here: https://robotran-doc.git-page.immc.ucl.ac.be/TutorialModellingFeatures/c-code/install.html
    * Download mbsysc source on _dev_ branch from git repository:
      https://git.immc.ucl.ac.be/robotran/mbsysc
2. Compile the project by following [instructions from the tutorial](https://robotran-doc.git-page.immc.ucl.ac.be/TutorialModellingFeatures/c-code/ModellingFeatures/bodiesJoint.html#step-4-run-your-simulation)
   The project can be compiled under Windows, Linux or MacOS. 
   Compiling is configured using CMake and can be set up either via
   the command line or via a graphical interface.
3. For using Simulink,
    * See the specific documentation page: https://git.immc.ucl.ac.be/robotran/mbsysc/-/blob/dev/MBsysC/readme_aux/interface_simulink.md
    * Pay attention to compile the project with the CMake option 
      `FLAG_SEPARATE_BUILD` set to `ON` (otherwise, the Project_userfct
      and Project_symbolic shared libraries will not be found)
    * In case of Seg fault in Simulink when starting the simulation, 
      it is necessary to add call to function `xmlInitParser()` at
      the begining of the `mbs_info_init_and_binderXML` in file 
      `MBsysC/mbs_common/mbs_load_xml/mbs_xml_binder.c` and 
       comment call to xmlCleanupParser (several
      occurences).
      
## Using

* The simulation is started by running the executable file
in the build folder.
* Result files are written in the resultsR folder
* Animation files to be visualized in the 3D environment of 
MBsysPad are written in the animationR folder
* A python script `copy_force_anim_file.py` is available in the resultsR
 folder for writing additional animation files that enables to visualize
 the wheel/ground contact force in the 3D animation.
* An example Simulink model is available in the workR folder 

## The driver model

The driver action is modeled as a joint torque in the _Fork_ joint.

This torque is the sum of:
 * a proportional-derivative law to the _Frame_ roll angle. The
   roll angle set point is computed from a proportional-derivative
   law to the _Frame_ lateral offset position w.r.t to the lane to 
   follow. A maximal value of roll set point can be set to avoid
   too sharp turning,
 * a proportional-derivate law to the _Fork_ angle. Typically
   the proportional gain is set to 0 and the derivative gain is
   set to introduce some damping in the control law.  

## User models and parameters

The following user models are defined:

* **global**, general parameters:
  * **speed** the initial speed of the bike
  * **tf** final time of te simulation
  * **flag_bilateral_normal_force** flag to specify wether to apply a sticking
    normal force (useful to compute the quilibirium: this flag is set to 1
    when computing the equilibrium))
    * 0: unilateral normal force (force if the wheel is in the ground)
    * 1: unilateral normal force (tyre is attracted toward the ground
        when it is above the groud)
  * **flag_tangential_force** (this flag i sset to 0 when computing
      equilibrium)
    * 0: no tangential force
    * 1: Sharp tangential force

* **cargo**, a user model to define the position of the cargo on the bike
  * **flag\_load\_case** define where to place the pay load on the frame
    (NB: this parameter can be overriden by passing a value when calling
     the program through the command line)
    * 0 : no payload
    * 1 : load at front
    * 2 : load at back
  * **load_at_front_m** the cargo mass for load case 1
  * **load_at_front_x** the position of cargo along x axis in the _Frame_ fixed frame
  * **load_at_front_z** the position of cargo along z axis in the _Frame_ fixed frame
  * **load_at_front_alpha** the rotation of the cargo w.r.t. the _Frame_ fixed frame
  * **load_at_back_m** the cargo mass for load case 1
  * **load_at_back_x** the position of cargo along x axis in the _Frame_ fixed frame
  * **load_at_back_z** the position of cargo along z axis in the _Frame_ fixed frame
  * **load_at_back_alpha** the rotation of the cargo w.r.t. the _Frame_ fixed frame

* **rider**, the parameters for the steering control
  * **K_phi** the roll to torque proportional gain
  * **K_phid** the roll to torque derivative gain
  * **K_delta** the steering angle to torque proportional gain
  * **K_deltad** the steering angle to torque derivative gain
  * **K_y** the lateral offset to roll angle proportional gain
  * **K_yd** the lateral offset to roll angle derivative gain
  * **phi_max** the maximal set point value for the roll angle

* **trajectory**, the parameters for defingin the lane change trajectory
  * **lateral_offset** the lateral offset of the trajectory
  * **dist_go_to_left** the distance after which the rider go to the left
  * **dist_go_to_right** the distance after which the rider go to the right
    (`dist_go_to_right>dist_go_to_left`)

* **tyre**, the parameters of the tyre/ground model
  * **K** the stiffness of the equivalent spring for the normal force
  * **C** the damping coefficient for the normal force
  * **R0** the nominal radius of the wheels
  * **CFx** the longitudinal proportional constant to slip coef
  * **CFy** the lateral proportional constant to slip angle
  * **CMz_ratio** the ratio linking the torque constant to the _CFy_
  * **flag_saturation** determine wether saturation is active
    * 0 : no saturation
    * 1: saturation of the tangential force using a hyperbolic tangent.
         Longitudinal and lateral components are scaled by comparing
         their resultant to mu*Fn (mu: friction coefficient, Fn: normal
         force)
  * **friction** the friction coefficient used for the tangential force
          saturation       
  
* **ground**,  the definition of the ground elevation profile
  * **flag_ground** to select the ground profile
    * 0: flat ground with elevation = 0 
    * 1: a series of sinusoidal bumps along x
    * 2: a series of sinusoidal bumbps in a qudratic half pipe
  * **ground_profile** te pointer to the ground profile function  

* **utility**, a user model to store utility variable
  * **sens_IMU** a pointer to MbsSensor structure to compute the IMU data

## User input/output

The following user variables are set to provide input and output
if the model is embeded in a Simulink S-Function:

* **T\_fork** a scalar input that can be used for steering control
     (must be implemented in user_JointForces)
* **T\_rear_wh**  scalar input that can be used for speed control
     (must be implemented in user_JointForces)
* **acc** a 3-element vector output to retrieve the acceleration of 
     a sensor (to be implemented in user_dirdyn_IO)
* **omega** a 3-element vector output to retrieve the angular velocity of 
     a sensor (to be implemented in user_dirdyn_IO)     

The input/output of the Simulink block are thus:
* input
  * the actuated joint force/torque
  * the driven joint trajectories
  * the steering torque _T\_Fork_
  * the motor torque _T\_Fork_
* output
  * the generalized position vector
  * the generalized velocity vector
  * the generalized acceleration vector
  * the sensor acceleration (if implemented)
  * the sensor angular vecolity (if implemented)
