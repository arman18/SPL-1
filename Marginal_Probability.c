// Calculate the probability of one vector

#include "miinclude.h"

//return the number of states
template <class T> void copyvecdata(T * srcdata, long len, int * desdata, int& nstate, int &minn, int& maxx);

template <class T> void copyvecdata(T * srcdata, long len, int * desdata, int& nstate, int &minn, int& maxx)
{
  if(!srcdata || !desdata)
  {
    printf("NULL points in copyvecdata()!\n");
    return;
  }

  long i;
  if (srcdata[0]>0)
    maxx = minn = int(srcdata[0]+0.5);
  else
    maxx = minn = int(srcdata[0]-0.5);

  int tmp;
  double tmp1;
  for (i=0;i<len;i++)
  {
    tmp1 = double(srcdata[i]);
    tmp = (tmp1>0)?(int)(tmp1+0.5):(int)(tmp1-0.5);//round to integers
    minn = (minn<tmp)?minn:tmp;
    maxx = (maxx>tmp)?maxx:tmp;
    desdata[i] = tmp;
    //    printf("%i ",desdata[i]);
  }
  //printf("\n");

  for (i=0;i<len;i++)
  {
    desdata[i] -= minn;
  }

  //return the #state
  nstate = (maxx-minn+1);

  return;
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  // check the arguments

  if(nrhs != 1 && nrhs !=2 && nrhs!=3)
    mexErrMsgTxt("Err");
  if(nlhs > 3)
    mexErrMsgTxt("Too many output argument");

  if (!mxIsInt8(prhs[0]) && !mxIsUint8(prhs[0]) && !mxIsDouble(prhs[0]) )
    mexErrMsgTxt("The first input argument must be types of INT8 or UINT8 or DOUBLE.");

  //get and check size information

  long i,j;

  void *img1 = (void *)mxGetData(prhs[0]);
  long len1 = mxGetNumberOfElements(prhs[0]);
  mxClassID type1 = mxGetClassID(prhs[0]);

  if (!img1 || !len1)
    mexErrMsgTxt("The input vector is invalid.");

  int b_findstatenum = 1;
  int nstate1 = 0;
  if (nrhs>=2)
  {
    b_findstatenum = 0;
    long MaxGrayLevel = (long) mxGetScalar(prhs[1]);
    nstate1 = MaxGrayLevel;
    if (MaxGrayLevel<=1)
    {
      printf("The argument #state is invalid. ");
      b_findstatenum = 1;
    }
  }

  int b_returnprob = 1;
  if (nrhs>=3)
  {
    b_returnprob = (mxGetScalar(prhs[2])!=0);
  }

  //copy data into new INT type array (hence quantization) and then reange them begin from 0 (i.e. state1)

  int * vec1 = new int[len1];
  int nrealstate1=0, minn,maxx;
  switch(type1)
  {
    case mxINT8_CLASS: copyvecdata((char *)img1,len1,vec1,nrealstate1,minn,maxx); break;
    case mxUINT8_CLASS: copyvecdata((unsigned char *)img1,len1,vec1,nrealstate1,minn,maxx); break;
    case mxDOUBLE_CLASS: copyvecdata((double *)img1,len1,vec1,nrealstate1,minn,maxx); break;
  }

  //update the #state when necessary
  if (nstate1<nrealstate1)
  {
    nstate1 = nrealstate1;
  }

  //generate the marginal-distribution list

  plhs[0] = mxCreateDoubleMatrix(nstate1,1,mxREAL);
  double *ha = (double *) mxGetPr(plhs[0]);

  for (i=0; i<nstate1;i++)
  {
    ha[i] = 0;
  }

  for (i=0;i<len1;i++)
  {
    ha[vec1[i]] += 1;
  }

  //return the probabilities
  if(b_returnprob)
  {
    for (i=0; i<nstate1;i++)
    {
      ha[i] /= len1;
    }
  }

