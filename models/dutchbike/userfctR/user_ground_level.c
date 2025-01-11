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

#include "math.h"
#include "user_ground_level.h"


int sinus_ground(double* x_I, double* mu, double* mupx, double* mupy){
    
    double x, y;
    // distance befor to start
    double x0;
    double z_max, lambda;

    // parameters of the ground profile
    x0 = 3.;
    z_max = 150e-3;
    lambda = 6.;

    // retrieve position of the wheel center
    x = x_I[1];
    y = x_I[2];

    if(x<x0){
        *mu = 0.;
        *mupx = 0.;
        *mupy = 0.;

    }else{
        double coef = 2*M_PI/lambda;
        *mu = z_max*(1-cos(coef*(x-x0)));
        *mupx = z_max*coef*sin(coef*(x-x0));
        *mupy = 0.;
    }

    return 0;
    
}


int sinus_quad_ground(double* x_I, double* mu, double* mupx, double* mupy){

    double x, y;
    // distance befor to start
    double x0;
    double z_max, lambda, ay;

    // parameters of the ground profile
    // start of the profile
    x0 = 3.;
    // sinus parameters
    z_max = 150e-3;
    lambda = 6.;
    // parabola parameters along y
    ay = .1;


    // retrieve position of the wheel center
    x = x_I[1];
    y = x_I[2];

    if(x<x0){
        *mu = 0.;
        *mupx = 0.;
        *mupy = 0.;

    }else{
        double coef = 2*M_PI/lambda;
        *mu = z_max*(1-cos(coef*(x-x0))) + ay*y*y;
        *mupx = z_max*coef*sin(coef*(x-x0));
        *mupy = 2.*ay*y;
    }

    return 0;

}


int slope_ground(double* x_I, double* mu, double* mupx, double* mupy) {

    double x, y;
    // distance befor to start
    double x0, x1, x2, x3, deltax;
    double height, slope;

    // parameters of the ground profile
    // start of the profile
    deltax = 3.0; // m
    x0 = 5.; //m
    x1 = x0 + deltax; // m
    x2 = x1+x0;
    x3 = x2+(x1-x0);

    // slope parameters
    height = 1.0; // m
    slope = height/(x1 - x0);

    // retrieve position of the wheel center
    x = x_I[1];
    y = x_I[2];

    if ( (x < x0) || (x >=x3) ) {
        *mu = 0.;
        *mupx = 0.;
        *mupy = 0.;

    }
    else if( x>=x0 && x < x1) {
        *mu =  - slope * (x-x0);
        *mupx = -slope;
        *mupy = 0.;
    }
    else if (x > +x1 && x < x2) {
        *mu = -height;
        *mupx = 0.;
        *mupy = 0.;
    }
    else if (x >=x2 && x < x3) {
        *mu = -height + slope * (x - x2);
        *mupx = slope;
        *mupy = 0.;
    }
    else  {
        *mu = 0.;
        *mupx = 0.;
        *mupy = 0.;
    }

    return 0;

}

int speed_bump_ground(double* x_I, double* mu, double* mupx, double* mupy) {

    double x, y;
    // distance before to start
    double x0, x1, x2, x3, deltax;
    double height, slope;

    // parameters of the ground profile
    // start of the profile
    deltax = 0.2; // m
    x0 = 5.; //m
    x1 = x0 + deltax; // m
    x2 = x1 + x0;
    x3 = x2 + (x1 - x0);

    // slope parameters
    height = 0.05; // m
    slope = height / (x1 - x0);

    // retrieve position of the wheel center
    x = x_I[1];
    y = x_I[2];

    if ((x < x0) || (x >= x3)) {
        *mu = 0.;
        *mupx = 0.;
        *mupy = 0.;

    }
    else if (x >= x0 && x < x1) {
        *mu = slope * (x - x0);
        *mupx = slope;
        *mupy = 0.;
    }
    else if (x > +x1 && x < x2) {
        *mu = height;
        *mupx = 0.;
        *mupy = 0.;
    }
    else if (x >= x2 && x < x3) {
        *mu = height - slope * (x - x2);
        *mupx = slope;
        *mupy = 0.;
    }
    else {
        *mu = 0.;
        *mupx = 0.;
        *mupy = 0.;
    }

    return 0;

}
