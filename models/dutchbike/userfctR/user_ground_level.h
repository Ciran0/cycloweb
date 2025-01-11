

#ifndef USER_GROUND_LEVEL_h
#define USER_GROUND_LEVEL_h


/**
 * @brief A ground profile with sinusoidal bumps in the x direction.
 *        The ground if flat for x_I[1]<3 .
 *
 * Parameters according to the TgcGroundLevel function prototype
 * (see tgc_kine_wheel_plane3D.h)
 */
int sinus_ground(double* x_I, double* mu, double* mupx, double* mupy);

/**
 * @brief A ground profile with sinusoidal bumps in the x direction
 *        and a half-pipe with quadratic profile in the y direction
 *        The ground if flat for x_I[1]<3 .
 *
 * Parameters according to the TgcGroundLevel function prototype
 * (see tgc_kine_wheel_plane3D.h)
 */
int sinus_quad_ground(double* x_I, double* mu, double* mupx, double* mupy);

/**
 * @brief A ground profile with a downwards slope (10 meters) then upwards (0 meters) in the x direction
 *        The ground if flat for x_I[1]<3 .
 *
 * Parameters according to the TgcGroundLevel function prototype
 * (see tgc_kine_wheel_plane3D.h)
 */
int slope_ground(double* x_I, double* mu, double* mupx, double* mupy);

/**
 * @brief A ground profile with a speed bump.
 *
 * Parameters according to the TgcGroundLevel function prototype
 * (see tgc_kine_wheel_plane3D.h)
 */
int speed_bump_ground(double* x_I, double* mu, double* mupx, double* mupy);


#endif
