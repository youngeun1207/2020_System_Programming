/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "신영은",              /* name */

    "2018030328",     /* student ID */
    "yesheen98@gmail.com",  /* student email */

    "",                   /* Second member full name (leave blank if none) */
    ""                    /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: Current working version";
void rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    dst += (dim-1)*dim;
    for (i = 0; i < dim; i+=32){
        for (j = 0; j < dim; j++){
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src; src+=dim; dst+=1;
            *dst=*src;
            src++;
            src-= (dim<<5)-dim;
            dst-=31+dim;
        }
        dst+=dim*dim;
        dst+=32;
        src += (dim<<5)-dim;
    }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);   
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";

int pR[530][530],pG[530][530],pB[530][530];
void smooth(int dim, pixel *src, pixel *dst) 
{
    int i,j;
    for(i=1;i<dim+1;i++)
        for(j=1;j<dim+1;j++){
            pR[i][j]=pR[i-1][j]+pR[i][j-1]-pR[i-1][j-1]+src[(i-1)*dim+j-1].red;
            pG[i][j]=pG[i-1][j]+pG[i][j-1]-pG[i-1][j-1]+src[(i-1)*dim+j-1].green;
            pB[i][j]=pB[i-1][j]+pB[i][j-1]-pB[i-1][j-1]+src[(i-1)*dim+j-1].blue;
        }
    dst[0].red=pR[2][2]/4;
    dst[0].green=pG[2][2]/4;
    dst[0].blue=pB[2][2]/4;

    dst[dim-1].red=(pR[2][dim]-pR[2][dim-2])/4;
    dst[dim-1].green=(pG[2][dim]-pG[2][dim-2])/4;
    dst[dim-1].blue=(pB[2][dim]-pB[2][dim-2])/4;

    dst[(dim-1)*dim].red=(pR[dim][2]-pR[dim-2][2])/4;
    dst[(dim-1)*dim].green=(pG[dim][2]-pG[dim-2][2])/4;
    dst[(dim-1)*dim].blue=(pB[dim][2]-pB[dim-2][2])/4;

    dst[(dim-1)*(dim+1)].red=(pR[dim][dim]-pR[dim-2][dim]-pR[dim][dim-2]+pR[dim-2][dim-2])/4;
    dst[(dim-1)*(dim+1)].green=(pG[dim][dim]-pG[dim-2][dim]-pG[dim][dim-2]+pG[dim-2][dim-2])/4;
    dst[(dim-1)*(dim+1)].blue=(pB[dim][dim]-pB[dim-2][dim]-pB[dim][dim-2]+pB[dim-2][dim-2])/4;
    
    for(j=1;j<dim-1;j++){
        dst[j].red=(pR[2][j+2]-pR[2][j-1])/6;
        dst[j].green=(pG[2][j+2]-pG[2][j-1])/6;
        dst[j].blue=(pB[2][j+2]-pB[2][j-1])/6;
    }
    for(j=1;j<dim-1;j++){
        dst[(dim-1)*dim+j].red=(pR[dim][j+2]-pR[dim][j-1]-pR[dim-2][j+2]+pR[dim-2][j-1])/6;
        dst[(dim-1)*dim+j].green=(pG[dim][j+2]-pG[dim][j-1]-pG[dim-2][j+2]+pG[dim-2][j-1])/6;
        dst[(dim-1)*dim+j].blue=(pB[dim][j+2]-pB[dim][j-1]-pB[dim-2][j+2]+pB[dim-2][j-1])/6;
    }
    for(i=1;i<dim-1;i++){
        dst[i*dim].red=(pR[i+2][2]-pR[i-1][2])/6;
        dst[i*dim].green=(pG[i+2][2]-pG[i-1][2])/6;
        dst[i*dim].blue=(pB[i+2][2]-pB[i-1][2])/6;
    }
    for(i=1;i<dim-1;i++){
        dst[i*dim+dim-1].red=(pR[i+2][dim]-pR[i-1][dim]-pR[i+2][dim-2]+pR[i-1][dim-2])/6;
        dst[i*dim+dim-1].green=(pG[i+2][dim]-pG[i-1][dim]-pG[i+2][dim-2]+pG[i-1][dim-2])/6;
        dst[i*dim+dim-1].blue=(pB[i+2][dim]-pB[i-1][dim]-pB[i+2][dim-2]+pB[i-1][dim-2])/6;
    }
    for(i=1;i<dim-1;i++)
        for(j=1;j<dim-1;j++){
            dst[i*dim+j].red=(pR[i+2][j+2]-pR[i+2][j-1]-pR[i-1][j+2]+pR[i-1][j-1])/9;
            dst[i*dim+j].green=(pG[i+2][j+2]-pG[i+2][j-1]-pG[i-1][j+2]+pG[i-1][j-1])/9;
            dst[i*dim+j].blue=(pB[i+2][j+2]-pB[i+2][j-1]-pB[i-1][j+2]+pB[i-1][j-1])/9;
        }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* ... Register additional test functions here */
}

