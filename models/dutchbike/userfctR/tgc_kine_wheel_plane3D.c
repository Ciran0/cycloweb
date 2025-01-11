//---------------------------
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
// Creation : 29/10/2023
//---------------------------

#include "mbs_matrix.h"
#include "math.h"
#include "useful_functions.h"
#include "tgc_kine_wheel_plane3D.h"

/**
 * @brief Fill the given rotation matrix with the unit vector. Vector components
 *        are stored as the lines of the rotation matrix.
 * @param R the matrice to fill (e.g. R_X_I, rotation matrix from I to X)
 * @param e1 the 1st unit vector (e.g. eX1_I, components of unit vector X1 in I frame)
 * @param e2 the 2nd unit vector (e.g. eX2_I)
 * @param e3 the 3rd unit vector (e.g. eX3_I)
 */
void fill_rot_mat_from_vec(double R[4][4], double e1[4], double e2[4], double e3[4]){
    R[1][1] = e1[1];
    R[1][2] = e1[2];
    R[1][3] = e1[3];
    R[2][1] = e2[1];
    R[2][2] = e2[2];
    R[2][3] = e2[3];
    R[3][1] = e3[1];
    R[3][2] = e3[2];
    R[3][3] = e3[3];
}


void tgc_kine_wheel_plane3D(double x_I[4],double R_Y_I[4][4],
                    double xd_I[4],double omYI_I[4],double rnom,
                    TgcGroundLevel *ground_level,
                    double *pen_p, double *pend_p, double *rz_p, double *angslip_p, double *angcamb_p,
                    double *slip_p, double p_I[4], double pmd_I[4], double Rt_R_I[4][4], double dxF[4])
{
    // usefull variables
    int error_code;
    // temporary vector
    double v_tmp[4];
    // kinematics output
    double cos_angcamb, sin_angcamb, pen, pend, rz, angslip, slip;
    // ground profile variables
    double mu, mupx, mupy;
    // ground normal direction
    double n_I[4], delp_delx_I[4], delp_dely_I[4];

    // angular velocity of the wheel in the local frame {Y}
    double omYI_Y[4] = {3.0, 0.0, 0.0, 0.0};
    // angular velocity of the frozen wheel in the wheel frame {Y}
    double omXI_Y[4] = {3.0, 0.0, 0.0, 0.0};
    // angular velocity of the frozen wheel in the inertial frame {I}
    double omXI_I[4] = {3.0, 0.0, 0.0, 0.0};

    // velocity of the geometrical contact point in {I} frame
    double pgd_I[4] = {3.0, 0.0, 0.0, 0.0};
    // velocity of the geometrical contact point in {R} frame
    double pgd_R[4] = {3.0, 0.0, 0.0, 0.0};


    // vector from wheel center to ground, under the wheel center
    double wq_I[4] = {3.0, 0.0, 0.0, 0.0}, wq_R[4]= {3.0, 0.0, 0.0, 0.0};
    // vector from wheel center
    double wp_I[4] = {3.0, 0.0, 0.0, 0.0}, wp_R[4]= {3.0, 0.0, 0.0, 0.0};

    // unit vector of rotation matrices (NB: eR3_I=n_I)
    double eR1_I[4] = {3.0, 0.0, 0.0, 0.0}, eR2_I[4] = {3.0, 0.0, 0.0, 0.0};

    // rotation matrix from {I} to ground frame {R} (to be computed)
    double R_R_I[4][4] = { {3.0, 0.0, 0.0, 0.0},
                           {3.0, 0.0, 0.0, 0.0},
                           {3.0, 0.0, 0.0, 0.0},
                           {3.0, 0.0, 0.0, 0.0} };

    // rotation matrix from wheel frame {Y} to inertial frame {I} (to be computed)
    double R_I_Y[4][4] = { {3.0, 0.0, 0.0, 0.0},
                           {3.0, 0.0, 0.0, 0.0},
                           {3.0, 0.0, 0.0, 0.0},
                           {3.0, 0.0, 0.0, 0.0} };

    // compute the transpose of {I}=>{Y} martix
    transpose(R_Y_I, R_I_Y);

    // compute the ground properties under the wheel center
    // call the ground level function if it is defined
    if (ground_level){
        error_code = ground_level(x_I, &mu, &mupx, &mupy);
        // tangent vectors in I1-I3 plane and I2-I3 plane
        delp_delx_I[1] = 1.; delp_delx_I[2] = 0.; delp_delx_I[3] = mupx;
        delp_dely_I[1] = 0.; delp_dely_I[2] = 1.; delp_dely_I[3] = mupy;
        // normal to ground vector
        cross_product(delp_delx_I, delp_dely_I, n_I);
        // normalize the normal vector
        normalize(n_I, n_I);
    }
    // otherwise, consider flat ground
    else{
        mu = 0.;
        mupx = 0.;
        mupy = 0.;
        n_I[1] = 0.; n_I[2] = 0.; n_I[3] = 1.;
    }


    // compute the longitudinal direction (T1=R1)
    cross_product(R_Y_I[2] ,n_I, v_tmp);
    // normalize
    normalize(v_tmp, eR1_I);

    // compute the lateral direction R2 of the ground frame
    cross_product(n_I, eR1_I, eR2_I);

    // store vectors in the rotation matrix
    fill_rot_mat_from_vec(R_R_I, eR1_I, eR2_I, n_I);
    // compute its transpose
    transpose(R_R_I, Rt_R_I);

    // compute the camber angle as the angle btw Y2 and R2
    cos_angcamb = scalar_product(R_Y_I[2], R_R_I[2]);
    sin_angcamb = scalar_product(R_Y_I[2], R_R_I[3]);

    // compute the vector from wheel center to ground point below
    wq_I[3] = mu-x_I[3];
    // transpose it to local contact frame R
    matrix_product(R_R_I, wq_I, wq_R);

    // get the vector from wheel center to approximated contact point
    // (keep the component of wq along R3)
    wp_R[3] = wq_R[3];
    // compute the deformed wheel radius
    rz = -wp_R[3]/cos_angcamb;
    // compute lateral component (in {R}) of wp
    wp_R[2] = rz*sin_angcamb;
    // compute wp in the inertial frame
    matrix_product(Rt_R_I, wp_R, wp_I);
    // compute components of wq in wheel frame, i.e. dxF components
    matrix_product(R_Y_I, wp_I, dxF);

    // absolute position of the contact point
    vector_sum(x_I, wp_I, p_I);

    // penetration of the wheel in the ground, along ground normal (eR3 axis)
    pen = wp_R[3] + rnom*cos_angcamb;

    // VELOCITY RELATED KINEMATICS
    // velocity of material contact point (neglecting radius time derivative)
    cross_product(omYI_I, wp_I, v_tmp);   // omYI_I x
    vector_sum(xd_I, v_tmp, pmd_I);
    // get angular velocity in wheel frame
    matrix_product(R_Y_I, omYI_I, omYI_Y);
    // angular velocity of the frozen wheel (omXI_Y[2]=0)
    omXI_Y[1] = omYI_Y[1];
    omXI_Y[3] = omYI_Y[3];
    // compute frozen wheel angular velocity in the inertial frame
    matrix_product(R_I_Y, omXI_Y, omXI_I);
    // velocity of geomtrical contact point (approximated)
    cross_product(omXI_I, wp_I, v_tmp);   // omYI_I x
    vector_sum(xd_I, v_tmp, pgd_I);
    // velocity of geomtrical contact point in R frame
    matrix_product(R_R_I, pgd_I, pgd_R);

    // compute the slip angle from the velocity of geom ctc point
    angslip = atan2(pgd_R[2], pgd_R[1]);

    // compute the slip coefficient
    slip = -scalar_product(pmd_I, R_R_I[1]) / scalar_product(xd_I, R_R_I[1]);

    // compute the penetration derivative
    pend = scalar_product(xd_I, R_R_I[3]);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // store output variables to given pointers
    *slip_p = slip;
    *angcamb_p = atan2(sin_angcamb, cos_angcamb);
    *angslip_p = angslip;
    *pen_p = pen;
    *pend_p = pend;
    *rz_p = rz;

}
