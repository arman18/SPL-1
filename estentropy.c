//calculate the entropy of a scalar variable 

#include "miinclude.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs!=1)
    mexErrMsgTxt("err");
  if(nlhs > 1)
    mexErrMsgTxt("Too many output argument");

  double *pa = mxGetPr(prhs[0]);
  long totaln = (long)mxGetM(prhs[0])*mxGetN(prhs[0]);

  double sum = 0.0;
  double entropy = 0.0;
  for (long i=0;i<totaln;i++)
  {
    double val = pa[i];
    if (val<0) 
    {
      printf("Negative Probability!! Wrong data.\n");
    }
    sum += val;
    if (val!=0) 
    {
      entropy -= val*log(val);
    }
  }

  if (sum-1>1e-10)
  {
    printf("Dubious data! Sum is not 1.\n");
  }

  entropy /= log(2.0000);

  plhs[0] = mxCreateDoubleMatrix(1,1, mxREAL);
  *mxGetPr(plhs[0]) = entropy;

  return;
}

