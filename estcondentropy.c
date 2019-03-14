//calculate the conditional entropy H(X|Y)

#include "miinclude.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs != 2 && nrhs!=1)
    mexErrMsgTxt("The last two inputs are optional.");
  if(nlhs > 1)
    mexErrMsgTxt("Too many output argument <mutual_info>.");


  long i,j;

  double *pab = mxGetPr(prhs[0]);
  long pabhei = mxGetM(prhs[0]);
  long pabwid = mxGetN(prhs[0]);

  double *pb = mxGetPr(prhs[1]);
  long pblen =  mxGetM(prhs[1])*mxGetN(prhs[1]);

  if (pblen!=pabwid)
  {
    mexErrMsgTxt("Unmatched size: length of the second much be the same as the column of the first.");
  }

  double **pab2d = new double * [pabwid];
  for(j=0;j<pabwid;j++)
    pab2d[j] = pab + (long)j*pabhei;

  //calculate the conditional entropy

  double muInf = 0;
  
  muInf = 0.0;
  for (j=0;j<pabwid;j++) // should for pb
  {
    for (i=0;i<pabhei;i++) // should for pa
    {
      if (pab2d[j][i]!=0 && pb[j]!=0)
      {
	muInf += pab2d[j][i] * log(pb[j]/pab2d[j][i]);
      }
    }
  }

  muInf /= log(2.000000);

  plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
  *mxGetPr(plhs[0]) = muInf;

  if(pab2d){delete []pab2d;}

  return;
}
