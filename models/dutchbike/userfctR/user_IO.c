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
 * Last update : Wed Jan 17 16:06:11 2024
 * --------------------------------------------------------
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include "user_IO.h"
#include "mbs_data.h"
#include "set_output.h"
#include "useful_functions.h"

// ============================================================ //


int user_synch_output(MbsData *mbs_data)
{
    return set_mbs_to_output(mbs_data);
}

// ============================================================ //


UserIO* mbs_new_user_IO(UserIoInfo* ioInfo) 
{
    UserIO *uio;
    uio = (UserIO*) malloc(sizeof(UserIO));

    // init T_fork
    ioInfo->ptr_in[0] = &uio->T_fork;

    // init T_rear_wh
    ioInfo->ptr_in[1] = &uio->T_rear_wh;

    // init acc
    uio->acc = (double*) malloc(sizeof(double)*(ioInfo->size_out[0]+1));
    ioInfo->ptr_out[0] = uio->acc+1;

    // init omega
    uio->omega = (double*) malloc(sizeof(double)*(ioInfo->size_out[1]+1));
    ioInfo->ptr_out[1] = uio->omega+1;

    return uio;
}

void mbs_delete_user_IO(UserIO *uio)
{
    free(uio->acc);
    free(uio->omega);

    free(uio);
}

 void mbs_print_user_IO(UserIO* uio) 
{
    printf("UserIO->T_fork=%f\n", uio->T_fork);
    printf("UserIO->T_rear_wh=%f\n", uio->T_rear_wh);
    printf("UserIO->acc : \n");
    print_dvec_0(uio->acc + 1, 3);
    printf("UserIO->omega : \n");
    print_dvec_0(uio->omega + 1, 3);
}


void mbs_save_user_IO(FILE* stream, UserIO *uio)
{
    // Restoring UserIoInfo
    fprintf(stream, "        s->__user_IO_info = (UserIoInfo*)malloc(sizeof(UserIoInfo));\n");
    fprintf(stream, "        s->__user_IO_info->n_in = 2;\n");
    fprintf(stream, "        s->__user_IO_info->n_out = 2;\n");
    fprintf(stream, "        s->__user_IO_info->size_in = (int*)malloc(sizeof(int)*2);\n");
    fprintf(stream, "        s->__user_IO_info->size_out = (int*)malloc(sizeof(int)*2);\n");
    fprintf(stream, "        s->__user_IO_info->size_in[0] = 1;\n");
    fprintf(stream, "        s->__user_IO_info->size_in[1] = 1;\n");
    fprintf(stream, "        s->__user_IO_info->size_out[0] = 3;\n");
    fprintf(stream, "        s->__user_IO_info->size_out[1] = 3;\n");
    fprintf(stream, "        s->__user_IO_info->ptr_in = (double**)malloc(sizeof(double*)*2);\n");
    fprintf(stream, "        s->__user_IO_info->ptr_out = (double**)malloc(sizeof(double*)*2);\n");
    // Allocating UserIo
    fprintf(stream, "        s->user_IO = (UserIO*) malloc(sizeof(UserIO));\n");
    // Restoring UserIo
    fprintf(stream, "        s->__user_IO_info->ptr_in[0]=&s->user_IO->T_fork;\n");
    fprintf(stream, "        s->user_IO->T_fork=%.*e;\n", DBL_DECIMAL_DIG - 1, uio->T_fork);
    fprintf(stream, "        s->__user_IO_info->ptr_in[1]=&s->user_IO->T_rear_wh;\n");
    fprintf(stream, "        s->user_IO->T_rear_wh=%.*e;\n", DBL_DECIMAL_DIG - 1, uio->T_rear_wh);
    fprintf(stream, "        s->user_IO->acc = (double*)malloc(sizeof(double)*4);\n");
    fprintf(stream, "        s->user_IO->acc[0]=3;\n");
    fprintf(stream, "        s->__user_IO_info->ptr_out[0]= s->user_IO->acc+1;\n");
    fprintf(stream, "        s->user_IO->acc[1] = %.*e;\n", DBL_DECIMAL_DIG - 1, uio->acc[1]);
    fprintf(stream, "        s->user_IO->acc[2] = %.*e;\n", DBL_DECIMAL_DIG - 1, uio->acc[2]);
    fprintf(stream, "        s->user_IO->acc[3] = %.*e;\n", DBL_DECIMAL_DIG - 1, uio->acc[3]);
    fprintf(stream, "        s->user_IO->omega = (double*)malloc(sizeof(double)*4);\n");
    fprintf(stream, "        s->user_IO->omega[0]=3;\n");
    fprintf(stream, "        s->__user_IO_info->ptr_out[1]= s->user_IO->omega+1;\n");
    fprintf(stream, "        s->user_IO->omega[1] = %.*e;\n", DBL_DECIMAL_DIG - 1, uio->omega[1]);
    fprintf(stream, "        s->user_IO->omega[2] = %.*e;\n", DBL_DECIMAL_DIG - 1, uio->omega[2]);
    fprintf(stream, "        s->user_IO->omega[3] = %.*e;\n", DBL_DECIMAL_DIG - 1, uio->omega[3]);
}

void mbs_get_user_IO_size(int *n_in, int *n_out, int *n_user_IO) 
{
    *n_in  = 2; 
    *n_out = 2; 
    *n_user_IO = 4; 
}

// ============================================================ //
 