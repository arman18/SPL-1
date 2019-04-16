#include "miinclude.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  if(nlhs >3)
    mexErrMsgTxt("too many output...from estpab");

  long i,j;

  double * vec1 = mxGetPr(prhs[0]); 
  long len1 = mxGetNumberOfElements(prhs[0]);


  double * vec2 = mxGetPr(prhs[1]);
  long len2 = mxGetNumberOfElements(prhs[1]);

  if (len1!=len2)
    mexErrMsgTxt("The two vectors should have the same length.");

  

  int nstate1 = int(mxGetScalar(prhs[2])), nstate2 = int(mxGetScalar(prhs[3]));
 
     
  double *hab = (double *) mxGetPr(plhs[0]);
  double **hab2d = new double * [nstate2];


  for (i=0; i<nstate1;i++)
  for (j=0; j<nstate2;j++)
  {
    hab2d[j][i] = 0;
  }
  int n1,n2;

  
    for (i=0; i<nstate1;i++)
    for (j=0; j<nstate2;j++)
    {
      hab2d[j][i] /= len1;
    }

  if (nlhs>=2)
  {
    plhs[1] = mxCreateDoubleMatrix(nstate1,1,mxREAL);
    double *ha = (double *)mxGetPr(plhs[1]);
    for (i=0;i<nstate1;i++) {ha[i] = 0;}
    for (i=0;i<nstate1;i++)
    for (j=0;j<nstate2;j++)
    {
      ha[i] += hab2d[j][i];
    }
  }
  


  return;
}
