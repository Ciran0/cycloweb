/* ---------------------------
 * Robotran - MBsysC
 * 
 * Template file for direct dynamics module
 * 
 * This files enable the user to call custom at
 * specific places in the time simulation. It is a template
 * file that can be edited by the user.
 * 
 * This software is provided by Quandyga Engineering and the authors “as is” and
 * any express or implied warranties, including, but not limited to, the implied
 * warranties of merchantability and fitness for a particular purpose are disclaimed.
 * In no event shall Quandyga Engineering and the authors liable for any claim,
 * damages (including, but not limited to loss of use, data, or profits; or business
 * interruption) or other liability, whether in an action of contract, tort or
 * otherwise, arising from, out of or in connection with the software or the use
 * or other dealings in the software.
 *
 * copyright Quandyga Engineering, 2023
 *
 */

#define _USE_MATH_DEFINES  // to use M_PI in Windows
#include "math.h"

#include "mbs_data.h"
#include "mbs_dirdyn_struct.h"
#include "user_model.h"
#include "mbs_project_interface.h"
#include "mbs_sensor.h"
#include "user_all_id.h"
#include "user_IO.h"
#include "set_output.h"
#include "mbs_1D_array.h"

/*! \brief user own initialization functions
 *
 * \param[in,out] mbs_data data structure of the model
 * \param[in,out] mbs_dd general structure of the direct dynamic module (for advance users)
 *
 * For beginners, it is advised to only use the MbsData structure.
 * The field MbsDirdyn is provided for more advance users.
 */
void user_dirdyn_init(MbsData *mbs_data, MbsDirdyn *mbs_dd)
{
    // define custom output to store tyre/ground force info
    define_output_vector("xFrc_xFrc_wheel_front", 15);
    define_output_vector("xFrc_xFrc_wheel_rear", 15);

    // initialize the sensor for computing IMU data
    mbs_data->user_model->utility.sens_IMU = mbs_new_sensor(mbs_data);
}

/*! \brief user own loop functions
 *
 * \param[in,out] mbs_data data structure of the model
 * \param[in,out] mbs_dd general structure of the direct dynamic module (for advance users)
 *
 * For beginners, it is advised to only use the MbsData structure.
 * The field MbsDirdyn is provided for more advance users.
 */
void user_dirdyn_loop(MbsData *mbs_data, MbsDirdyn *mbs_dd)
{
    // retrieve the sensor structure to store results
    MbsSensor* sens = mbs_data->user_model->utility.sens_IMU;
    // compute the sensor kinematics
    mbs_comp_S_sensor(sens, mbs_data, IMU_id);
    // store it to user_IO
    copy_dvec_0(&sens->A[1], &mbs_data->user_IO->acc[1], 3);
    copy_dvec_0(&sens->OM[1], &mbs_data->user_IO->omega[1], 3);
}

/*! \brief user own finishing functions
 *
 * \param[in,out] mbs_data data structure of the model
 * \param[in,out] mbs_dd general structure of the direct dynamic module (for advance users)
 *
 * For beginners, it is advised to only use the MbsData structure.
 * The field MbsDirdyn is provided for more advance users.
 */
void user_dirdyn_finish(MbsData *mbs_data, MbsDirdyn *mbs_dd)
{
    // free the sensor structure
    mbs_delete_sensor(mbs_data->user_model->utility.sens_IMU);
}
