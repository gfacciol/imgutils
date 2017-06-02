/*--------------------------- MegaWave2 Module -----------------------------*/
/* mwcommand
   name = {fconvol};
   version = {"1.2"};
   author = {"Jacques Froment"};
   function = {"2D-direct convolution of a fimage"};
   usage = {
    in->in           "Input fimage",
    filter->filtre   "convolution filter (fimage)",
    out<-out         "Output fimage"
   };
*/
/*----------------------------------------------------------------------
 v1.1: fixed kmin/lmin/dyS bugs (L.Moisan)
 v1.2: fixed mwcommand syntax (JF)
----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
extern "C" {
#include "iio.h"
}

#define min(a,b)        (((a)>(b)) ? (b) : (a))
#define max(a,b)        (((a)>(b)) ? (a) : (b))


// dct type II symmetry at the boundary
int p_sym(int nx, int ny, int x, int y) { 
   if(x < 0)
      x = -x-1;
   if(y < 0)
      y = -y-1;
   if(x >= nx)
      x = -x+2*nx-1;
   if(y >= ny)
      y = -y+2*ny-1;
   return x+nx*y;
}


// Convolve I (of size nc x nr) with the filter in F (fnc x fnr), stores the result in O
// O(t) = \sum_s I(t-s) F(s)
// all the images are stored in row major format. 
// The centered of the filter is at (floor(fnc/2), floor(fnr/2)), 
void convolve(float *ptrI, int nc, int nr,  float *ptrF, int fnc, int fnr, float *ptrO)
{
   int n, m, k, l, kmax, kmin, lmax, lmin, K2, L2;
   double S;

   K2 = fnr / 2;
   L2 = fnc / 2;

   // symmetric boundaries
   for (n = 0; n < nr; n++) // rows
      for (m = 0; m < nc; m++)     // columns
      {
         S = 0.0;

         // scan the structuring element and store the values in pdata
         for (k = 0; k < fnr; k++) // rows
            for (l = 0; l < fnc; l++)    // columns
                  S += ptrI[ p_sym(nc, nr, (m - l + L2), (n - k + K2)) ] * ptrF[fnc * k + l];

         *ptrO++ = (float) S;
      }
}


int main (int argc, char **argv)
{
   /* ppatameter parsing - parameters*/
   if(argc<4) 
   {
      fprintf (stderr, "too few parameters\n");
      fprintf (stderr, "convolve In with a spatial filter to produce Out, symmetric boundary conditions.\n");
      fprintf (stderr, "   usage: %s In Filt Out \n",argv[0]);
      return 1;
   }


   int nc,nr,nch;
   float *in = iio_read_image_float_split(argv[1], &nc, &nr, &nch);

   // if the filter is multi channel forget about them, just use the first
   int fnc,fnr,fnch;
   float *fin = iio_read_image_float_split(argv[2], &fnc, &fnr, &fnch);

   float *out = (float*) malloc(nc*nr*nc*sizeof*out);


   for (int i=0;i<nch;i++)
         convolve(in+nc*nr*i,nc,nr, fin,fnc,fnr, out+nc*nr*i);
   

   iio_save_image_float_split(argv[3], out, nc, nr, nch);

   free(in);
   free(fin);
   free(out);

   return 0;
}



