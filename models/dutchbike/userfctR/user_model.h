/* --------------------------------------------------------
 * This code was generated automatically by MBsysC modules.
 * MBsysC modules are distributed as part of the ROBOTRAN 
 * software. They provides functionalities for dealing with
 * symbolic equations generated by ROBOTRAN. 
 *
 * More info on www.robotran.be 
 *
 * Universite catholique de Louvain, Belgium 
 *
 * Last update : Mon Nov 18 13:52:09 2024
 * --------------------------------------------------------
 *
 */
#ifndef USERMODEL_h
#define USERMODEL_h


#include "tgc_kine_wheel_plane3D.h"
#include "mbs_sensor_struct.h"

#include "mbs_user_interface.h"
// ============================================================ //


struct UserModel 
{
    struct rider{
        double K_phi;
        double K_phid;
        double K_y;
        double K_yd;
        double K_delta;
        double K_deltad;
        double phi_max;
    } rider;
 
    struct tyre{
        double K;
        double C;
        double R0;
        double CFx;
        double CFy;
        double CMz_ratio;
        int flag_saturation;
        double friction;
    } tyre;
 
    struct cargo{
        int flag_load_case;
        double load_at_front_m;
        double load_at_front_x;
        double load_at_front_z;
        double load_at_front_alpha;
        double load_at_back_m;
        double load_at_back_x;
        double load_at_back_z;
        double load_at_back_alpha;
    } cargo;
 
    struct global{
        double speed;
        double tf;
        double flag_bilateral_normal_force;
        double flag_tangential_force;
        //!> Results name (string)
        char *res_name;
        //!> path to resultsR
        char *res_path;
    } global;
 
    struct trajectory{
        double lateral_offset;
        double dist_go_to_left;
        double dist_go_to_right;
    } trajectory;
 
    struct ground{
        int flag_ground;
        TgcGroundLevel* ground_profile;        // pointer must be initialized and freed by the user
    } ground;
 
    struct utility{
        MbsSensor* sens_IMU;        // pointer must be initialized and freed by the user
    } utility;
 
};

// ============================================================ //
 
# endif