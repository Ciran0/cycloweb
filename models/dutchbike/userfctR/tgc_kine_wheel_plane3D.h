//---------------------------
//
// Definition of type and function for computing the tyre-ground contact model,
// with particuliar cases for uneven ground.
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

#ifndef TGC_KINE_WHEEL_PLANE3D_h
#define TGC_KINE_WHEEL_PLANE3D_h


/** Function pointer definition to define the ground profile
 * 
 * @param [in] double[4] x_I the position of the center of the wheel in the inertial frame
 * @param [out] double* mu the pointer to the elevation of the ground (along I3)
 * @param [out] double* mupx the pointer to the partial derivative of the ground along I1
 * @param [out] double* mupy the pointer to the partial derivative of the ground along I2
 *
 * @return int error code
 */
typedef int (TgcGroundLevel)(double* x_I, double* mu, double* mupx, double* mupy);


/**
 * @brief Compute the wheel kinematics by approximative the ground as a tilted
 * plane tangent to the ground below the center of the wheel.
 * 
 * @param x_I [in] components of the position vector of the wheel center in the inertial frame
 * @param R_Y_I [in] the rotation matrix from the inertial frame to the wheel fixed frame
 *                   [Y] = R_Y_I * [I]  or v_Y = R_Y_I * v_I (v_Y components of a vector in {Y} frame,
 *                   v_I components of the same vector in inertial frame {I})
 * @param xd_I [in] component of the velocity vector of the wheel center in the inertial frame
 * @param omYI_I [in] angular vecolity vector in the inertial frame
 * @param rnom [in] the nominal radius of the wheel
 * @param ground_level [in] a pointer to a function to compute the ground profile according to
 *                   TgcGroundLevel prototype. Use NULL to compute on a flat ground.
 * @param pen_p [out] penetration of the wheel in the ground (along the direction normal to
 *                   ground plane)
 * @param pend_p [out] the derivative of the penetration of the wheel in the ground computed as
 *                   the projection on ground normal of the speed of the center of the wheel
 * @param rz_p [out] pointer to store the crushed radius of the deformed wheel
 * @param angslip_p [out] pointer to store the slip angle
 * @param angcamb_p [out] pointer to store the camber angle
 * @param slip_p [out] pointer to store the slip coefficient. Slip coef is equal to
 *                      slip = - pmd_R[1]/xd_R[1] (pay attention to the sign !!!)
 * @param p_I [out] absolute position of the contact point, expressed in the
 *                  inertial frame {I}
 * @param pmd_I [out] absolute velocity of the material contact point, expressed in
 *                  the inertial frame {I} (neglected wheel radius time derivative)
 * @param Rt_R_I [out] pointer to matrix to store the rotation matrix from ground frame
 *                   to inertial frame: [I] = Rt_R_I * [R]  or v_I = Rt_R_I * v_R
 *                  (v_R components of a vector in {R} frame,
 *                   v_I components of the same vector in inertial frame {I})
 * @param dxF [out] pointer to store the vector from the wheel center to the approximate
 *                  contact point, components in the wheel attached frame ({Y} frame)
 *
 */ 
void tgc_kine_wheel_plane3D(double x_I[4],double R_Y_I[4][4],
                    double xd_I[4],double omYI_I[4],double rnom,
                    TgcGroundLevel *ground_level,
                    double *pen_p, double *pend_p, double *rz_p, double *angslip_p, double *angcamb_p,
                    double *slip_p, double p_I[4], double pmd_I[4], double Rt_R_I[4][4], double dxF[4]);


#endif
