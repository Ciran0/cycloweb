//---------------------------
//
// Quandyga Engineering
// This software is provided by Quandyga Engineering and the authors “as is” and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall Quandyga Engineering and the authors liable for any claim,
// damages (including, but not limited to loss of use, data, or profits; or business
// interruption) or other liability, whether in an action of contract, tort or
// otherwise, arising from, out of or in connection with the software or the use
// or other dealings in the software.
//
// copyright Quandyga Engineering, 2023
//
//
//---------------------------

#define _USE_MATH_DEFINES  // to use M_PI in Windows
#include "math.h" 

#include "mbs_data.h"
#include "user_model.h"

#include "set_output.h"
#include "user_all_id.h"

double* user_JointForces(MbsData *mbs_data, double tsim)
{

/*-- Begin of user code --*/

    UserModel* um = mbs_data->user_model;
    if( (um->ground.flag_ground == 2 || um->ground.flag_ground == 1 || um->ground.flag_ground == 0) && mbs_data->process==33){
        double K_phi, K_phid, K_delta, K_deltad, K_y, K_yd;
        double phi0, phi, phid, phi_max;
        double delta, deltad;
        double x, y, y_0, yd;

        // retrieve controller values from user models
        K_phi = um->rider.K_phi;
        K_phid = um->rider.K_phid;
        K_y = um->rider.K_y;
        K_yd = um->rider.K_yd;
        K_delta = um->rider.K_delta;
        K_deltad = um->rider.K_deltad;
        phi_max = um->rider.phi_max;

        // retrieve useful kinematics for computing the control
        x = mbs_data->q[T1_Frame_id];
        y = mbs_data->q[T2_Frame_id];
        yd = mbs_data->qd[T2_Frame_id];
        phi = mbs_data->q[R1_Frame_id];
        phid = mbs_data->qd[R1_Frame_id];
        delta = mbs_data->q[R3_Fork_id];
        deltad = mbs_data->qd[R3_Fork_id];

        // lateral target depending on the lane change trajectory
        if(x < um->trajectory.dist_go_to_left){
            y_0 = 0.;
        }
        else if(x < um->trajectory.dist_go_to_right){
            y_0 = um->trajectory.lateral_offset;
        }
        else{
            y_0 = -um->trajectory.lateral_offset;
        }
        
        // compute the roll set point value
        phi0 = K_y*(y-y_0)+K_yd*yd;
        // bounds on the roll ange set point
        if(phi0>phi_max) { phi0=phi_max;}
        if(phi0<-phi_max){ phi0=-phi_max;}

        // compute contribution of roll error
        mbs_data->Qq[R3_Fork_id] = K_phi*(phi0-phi)-K_phid*phid;
        // add a term depending on the steering dof
        mbs_data->Qq[R3_Fork_id] += -K_delta*(delta)-K_deltad*(deltad);
    }    

/*-- End of user code --*/

    return mbs_data->Qq;
}
