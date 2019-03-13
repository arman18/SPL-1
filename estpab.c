// Calculate the joint probability of two vectors

#include "miinclude.h"

//return the number of states
template <class T> void copyvecdata(T * srcdata, long len, int * desdata, int& nstate);

template <class T> void copyvecdata(T * srcdata, long len, int * desdata, int& nstate)
{
  if(!srcdata || !desdata)
  {
    printf("NULL points in copyvecdata()!\n");
    return;
  }

  long i;

  //copy data
  int minn,maxx;
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

  //make the vector data begin from 0 (i.e. 1st state)
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

  if(nrhs != 3 && nrhs !=2 && nrhs!=4)
    mexErrMsgTxt("ERR");
  if(nlhs > 5)
    mexErrMsgTxt("Too many output argument ");

  if (!mxIsInt8(prhs[0]) && !mxIsUint8(prhs[0]) && !mxIsDouble(prhs[0]) )
    mexErrMsgTxt("The first input argument must be types of INT8 or UINT8 or DOUBLE.");
  if (!mxIsInt8(prhs[1]) && !mxIsUint8(prhs[1]) && !mxIsDouble(prhs[1]) )
    mexErrMsgTxt("The second input argument must be types of INT8 or UINT8 or DOUBLE.");

  //get and check size information

  long i,j;

  void *img1 = (void *)mxGetData(prhs[0]); //void because we do not know data type if prhs
  //double *image1 = (double *)img1;
  //for(int i=0; i<6;i++) printf("v %lf",image1[i]);
  long len1 = mxGetNumberOfElements(prhs[0]);//length of a
  mxClassID type1 = mxGetClassID(prhs[0]);

  void *img2 = (void *)mxGetData(prhs[1]);
  long len2 = mxGetNumberOfElements(prhs[1]);//length of b
  mxClassID type2 = mxGetClassID(prhs[1]);

  if (!img1 || !img2 || !len1 || !len2)
    mexErrMsgTxt("At least one of the input vectors is invalid.");
  if (len1!=len2)
    mexErrMsgTxt("The two vectors/images should have the same length.");

  int b_findstatenum = 1;
  int nstate1 = 0, nstate2 = 0;
  if (nrhs>=3)
  {
    b_findstatenum = 0;
    long MaxGrayLevel = (long) mxGetScalar(prhs[2]);
    nstate1 = nstate2 = MaxGrayLevel;
    if (MaxGrayLevel<=1)
    {
      printf("The argument #state is invalid. This program will decide #state itself.\n");
      b_findstatenum = 1;
    }
  }

  int b_returnprob = 1;
  if (nrhs>=4)
  {
    b_returnprob = (mxGetScalar(prhs[3])!=0);
  }

  //copy data into new INT type array (hence quantization) and then reange them begin from 0 (i.e. state1)

  int * vec1 = new int[len1];
  int * vec2 = new int[len2];
  int nrealstate1=0, nrealstate2=0;
  switch(type1)
  {
    case mxINT8_CLASS: copyvecdata((char *)img1,len1,vec1,nrealstate1); break;
    case mxUINT8_CLASS: copyvecdata((unsigned char *)img1,len1,vec1,nrealstate1); break;
    case mxDOUBLE_CLASS: copyvecdata((double *)img1,len1,vec1,nrealstate1); break;
  }
  switch(type2)
  {
    case mxINT8_CLASS: copyvecdata((char *)img2,len2,vec2,nrealstate2); break;
    case mxUINT8_CLASS: copyvecdata((unsigned char *)img2,len2,vec2,nrealstate2); break;
    case mxDOUBLE_CLASS: copyvecdata((double *)img2,len2,vec2,nrealstate2); break;
  }
  //printf("nmax1: %d nmax2: %d",nrealstate1,nrealstate2);

   if(nlhs>3) //calculate degrees of fredom
    {
        int* poss;
        double uniq=0;
        for(int i=0; i<len1; i++)  poss[vec1[i]] =1;
        for(int i=0; i<len1; i++)
        {
            if(poss[vec1[i]])
            {
                    uniq++;
                    poss[vec1[i]]=0;
            }
        }

         plhs[3] = mxCreateDoubleMatrix(1,1,mxREAL);
        double *p = mxGetPr(plhs[3]);
        p[0] = uniq;
        //delete []p;
    }
  if(nlhs>4) //calculate degrees of fredom
    {
        int* poss;
        double uniq=0;
        for(int i=0; i<len2; i++)  poss[vec2[i]] =1;
        for(int i=0; i<len2; i++)
        {
            if(poss[vec2[i]])
            {
                    uniq++;
                    poss[vec2[i]]=0;
            }
        }

         plhs[4] = mxCreateDoubleMatrix(1,1,mxREAL);
        double *p = mxGetPr(plhs[4]);
        p[0] = uniq;
        //delete []p;

    }

  //update the #state when necessary
  if (nstate1<nrealstate1)
  {
    nstate1 = nrealstate1;
  }
  if (nstate2<nrealstate2)
  {
    nstate2 = nrealstate2;
    // printf("Second vector #state = %i\n",nrealstate2);
  }

  //generate the joint-distribution table

  plhs[0] = mxCreateDoubleMatrix(nstate1,nstate2,mxREAL);
  double *hab = (double *) mxGetPr(plhs[0]);
  double **hab2d = new double * [nstate2];
  for(j=0;j<nstate2;j++)
    hab2d[j] = hab + (long)j*nstate1;

  for (i=0; i<nstate1;i++)
  for (j=0; j<nstate2;j++)
  {
    hab2d[j][i] = 0;
  }

  for (i=0;i<len1;i++)
  {


    hab2d[vec2[i]][vec1[i]] += 1;
  }

  //return the probabilities, otherwise return count numbers
  if(b_returnprob)
  {
    for (i=0; i<nstate1;i++)
    for (j=0; j<nstate2;j++)
    {
      hab2d[j][i] /= len1;
    }
  }

  //for nlhs>=2

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

  if (nlhs>=3)
  {
    plhs[2] = mxCreateDoubleMatrix(nstate2,1,mxREAL);
    double *hb = (double *)mxGetPr(plhs[2]);
    for (j=0;j<nstate2;j++) {hb[j] = 0;}
    for (i=0;i<nstate1;i++)
    for (j=0;j<nstate2;j++)
    {
      hb[j] += hab2d[j][i];
    }
  }

  //free
  if (hab2d) {delete []hab2d;}
  if (vec1) delete []vec1;
  if (vec2) delete []vec2;

  return;
}
