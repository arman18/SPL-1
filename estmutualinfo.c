// calculate the mutual information based on thr probabilities

#include "miinclude.h"
// #include "mex.h"
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs > 6 )
    mexErrMsgTxt("too many input argumetn <estmutualinfo>");
  if(nlhs > 1)
    mexErrMsgTxt("Too many output argument <estmutualinfo>.");


  long i,j;

  double *pab = mxGetPr(prhs[0]);
  long pabhei = mxGetM(prhs[0]); //number of row
  long pabwid = mxGetN(prhs[0]); //number of colum
  double **pab2d = new double * [pabwid];
  for(j=0;j<pabwid;j++) pab2d[j] = pab + (long)j*pabhei;
  double *p1,*p2;
  long p1len,p2len;
  int b_findmarginalprob = 0;
  


  if(nrhs==3)
  {
    p1 = mxGetPr(prhs[1]);
    p1len = mxGetM(prhs[1])*mxGetN(prhs[1]);

    p2 = mxGetPr(prhs[2]);
    p2len = mxGetM(prhs[2])*mxGetN(prhs[2]);

    if(p1len!=pabhei || p2len!=pabwid)
    {
      if(p1len==pabwid && p2len==pabhei)
      {
            p1 = mxGetPr(prhs[2]);
            p1len = mxGetM(prhs[2])*mxGetN(prhs[2]);
            
            
            p2 = mxGetPr(prhs[1]);
            p2len = mxGetM(prhs[1])*mxGetN(prhs[1]);
      }
      else
      {
            printf("pab size (%i,%i) doesn't match pa size %i and pb size %i\n.",pabhei,pabwid,p1len,p2len);
            b_findmarginalprob = 1;
      }
    }
  }
  else
  {
    b_findmarginalprob = 1;
  }

  //generate marginal probability arrays when necessary
  if (b_findmarginalprob!=0)
  {
    p1len = pabhei;
    p2len = pabwid;
    p1 = new double[p1len];
    p2 = new double[p2len];

    for(i=0;i<p1len;i++) p1[i] = 0;
    for(j=0;j<p2len;j++) p2[j] = 0; 

    for(i=0;i<p1len;i++) 
    for(j=0;j<p2len;j++) 
    {
      
      //calculating marginal probality
      p1[i] += pab2d[j][i];
      p2[j] += pab2d[j][i];
    }
  }

  //calculate the mutual information

  plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
  
  double *mInf = mxGetPr(plhs[0]);
  double muInf = 0;
  
  muInf = 0.0;
  for (j=0;j<pabwid;j++) // should for p2 
  {
    for (i=0;i<pabhei;i++) // should for p1
    {
      if (pab2d[j][i]!=0 && p1[i]!=0 && p2[j]!=0)
      {
            muInf += pab2d[j][i] * log(pab2d[j][i]/p1[i]/p2[j]);
 	
      }
    }
  }

  muInf /= log(2.0000);
    if(nrhs>3)
  {
    double df1 = mxGetScalar(prhs[3]);
    double df2 = mxGetScalar(prhs[4]);
    double m = mxGetScalar(prhs[5]); // feature length
    
    
    double bias = ((df1-1)*(df2-1))/(2 * m * log(2.0000));
    muInf = muInf - bias;
    
  }
  mInf[0] = muInf;

  return;
}
