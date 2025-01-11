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
#include "mbs_project_interface.h"
#include "user_all_id.h"

#include "user_model.h"
#include "mbs_matrix.h"
#include "user_model.h"
#include "set_output.h"
#include "user_all_id.h"
#include "mbs_1D_array.h"

#include "tgc_kine_wheel_plane3D.h"

double* user_ExtForces(double PxF[4], double RxF[4][4], 
                       double VxF[4], double OMxF[4], 
                       double AxF[4], double OMPxF[4], 
                       MbsData *mbs_data, double tsim,int ixF)
{
    // initialize the dxF
    double dxF[4] = { 0.0, 0.0, 0.0, 0.0 };

    // retrieve te SWr pointer for storing force output
    double *SWr = mbs_data->SWr[ixF];

    // force and torque array in the inertial frame
    double Fwhl_I[4], Mwhl_I[4];

    // default value to 0
    Fwhl_I[0] = 0.; Fwhl_I[1] = 0.; Fwhl_I[2] = 0.; Fwhl_I[3] = 0.;
    Mwhl_I[0] = 0.; Mwhl_I[1] = 0.; Mwhl_I[2] = 0.; Mwhl_I[3] = 0.;

    // default application point of the force: anchor point to which it is attached
    int idpt = 0;
    idpt = mbs_data->xfidpt[ixF];
    dxF[1] = mbs_data->dpt[1][idpt];
    dxF[2] = mbs_data->dpt[2][idpt];
    dxF[3] = mbs_data->dpt[3][idpt];

    // compute external forces for wheels
    if (ixF==xFrc_wheel_rear_id || ixF==xFrc_wheel_front_id){

        // tyre parameters
        double K_tyre, C_tyre, R0_tyre;
        double CFx, CFy, CMz, CMz2, CMz_ratio, Cphi;
        // usefull values to compute the contact force
        double Fz;

        // wheel kinematics variables
        double pen, pend, rz, angslip, angcamb, slip, Pcon[4], Vcon[4];
        double Rt_ground[4][4] = {0.};
        Pcon[0]=3, Pcon[1]=0., Pcon[2]=0., Pcon[3]=0.;
        Vcon[0]=3, Vcon[1]=0., Vcon[2]=0., Vcon[3]=0.;

        // tire/ground contact forces variables
        double Fwhl_R[4], Mwhl_R[4];
        // User model shortcut
        UserModel* um;

        // retrieve UserModel structure
        um = mbs_data->user_model;

        // retrieve parameters from the user model
        R0_tyre = um->tyre.R0;  // nominal radius
        K_tyre = um->tyre.K;    // tyre stiffness
        C_tyre = um->tyre.C;    // tyre damping
        // parameters for the tangential force model
        CFx = um->tyre.CFx;
        CFy = um->tyre.CFy;
        CMz_ratio = um->tyre.CMz_ratio;

        // intialize force/torque to 0
        Fwhl_R[1] = 0.;
        Fwhl_R[2] = 0.;
        Fwhl_R[3] = 0.;
        Mwhl_R[1] = 0.;
        Mwhl_R[2] = 0.;
        Mwhl_R[3] = 0.;

        // compute the kinematics
        tgc_kine_wheel_plane3D(PxF, RxF, VxF, OMxF, R0_tyre, um->ground.ground_profile,
                               &pen, &pend, &rz, &angslip, &angcamb, &slip,
                               Pcon, Vcon, Rt_ground, dxF);

        if(pen>0 || um->global.flag_bilateral_normal_force==1){
            // compute normal force
            Fwhl_R[3] = K_tyre*pen - C_tyre*pend;
            Fz = Fwhl_R[3];

            // compute the tangential force according to Sharp model
            if (um->global.flag_tangential_force==1){
                CMz = CFy*CMz_ratio;
                CMz2 = CMz;
                Cphi = Fz;
                Fwhl_R[1] = CFx*slip;
                Fwhl_R[2] = -CFy*angslip - Cphi*angcamb ;
                Mwhl_R[3] = CMz2*angslip;

                if (um->tyre.flag_saturation==1){
                    double F_tg_lin, F_tg_sat, mu, ratio, satur_coef;
                    // retrieve friction
                    mu = um->tyre.friction;
                    // compute the norm of tangential components
                    F_tg_lin = sqrt(Fwhl_R[1]*Fwhl_R[1]+Fwhl_R[2]*Fwhl_R[2]);
                    // compute saturated tangential force with tanh
                    F_tg_sat = Fz*mu*tanh(F_tg_lin/Fz/mu);
                    // compute ratio
                    ratio = F_tg_sat/F_tg_lin;
                    Fwhl_R[1]*=ratio;
                    Fwhl_R[2]*=ratio;
                }
            }
        }

        // transpose force/torque to inertial frame
        matrix_product(Rt_ground, Fwhl_R, Fwhl_I);
        matrix_product(Rt_ground, Mwhl_R, Mwhl_I);

        // save force/torque value if it is not the equilibrium
        if (mbs_data->process != 2){
            double frc_mat[15] = {0.};
            copy_dvec_0(&Fwhl_I[1], &frc_mat[0], 3);
            copy_dvec_0(&Mwhl_I[1], &frc_mat[3], 3);
            copy_dvec_0(&dxF[1], &frc_mat[6], 3);
            frc_mat[9] = Rt_ground[1][1];
            frc_mat[10] = Rt_ground[2][1];
            frc_mat[11] = Rt_ground[3][1];
            frc_mat[12] = Rt_ground[1][2];
            frc_mat[13] = Rt_ground[2][2];
            frc_mat[14] = Rt_ground[3][2];
            if(ixF==xFrc_wheel_front_id){
                set_output_vector(frc_mat, "xFrc_xFrc_wheel_front");
            }
            else if(ixF==xFrc_wheel_rear_id){
                set_output_vector(frc_mat, "xFrc_xFrc_wheel_rear");
            }
        }
    }

    // store the force/torque/dpt to the output
    SWr[1] = Fwhl_I[1];    SWr[2] = Fwhl_I[2];    SWr[3] = Fwhl_I[3];
    SWr[4] = Mwhl_I[1];    SWr[5] = Mwhl_I[2];    SWr[6] = Mwhl_I[3];
    SWr[7] = dxF[1];       SWr[8] = dxF[2];       SWr[9] = dxF[3];



    return SWr;
}

 
