// calculate the mutual information based on the probabilities

#include "miinclude.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  if(nrhs != 3 && nrhs!=1)
    mexErrMsgTxt("Usage [mutual_info] = progname(jointprob_table, marginprob_1, marginprob2). The last two inputs are optional.");
  if(nlhs > 1)
    mexErrMsgTxt("Too many output argument <mutual_info>.");

  //check if parameters are correct

  long i,j;

  double *pab = mxGetPr(prhs[0]);//take all 2d data to 1d append colum by colum
  long pabhei = mxGetM(prhs[0]);//number of row
  long pabwid = mxGetN(prhs[0]);//number of colum

  double **pab2d = new double * [pabwid];
  for(j=0;j<pabwid;j++)//because pab has colum by colum data
    pab2d[j] = pab + (long)j*pabhei;
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

    for(i=0;i<p1len;i++) //p1len = pabhei
    for(j=0;j<p2len;j++) //p2len = panwid
    {
      //printf("%f ",pab2d[j][i]);
      //calculating marginal probality
      p1[i] += pab2d[j][i];
      p2[j] += pab2d[j][i];
    }
  }

  //calculate the mutual information

  plhs[0] = mxCreateDoubleMatrix(1,1,mxREAL);
  //printf("d %d",plhs[0]);
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
  mInf[0] = muInf;

  if(pab2d){delete []pab2d;}
  if(b_findmarginalprob!=0)
  {
    if(p1) {delete []p1;}
    if(p2) {delete []p2;}
  }

  return;
}
