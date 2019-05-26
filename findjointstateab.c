
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  if(nrhs != 3 && nrhs !=2 && nrhs!=4)
  {
    mexErrMsgTxt("Err");
  }
  if(nlhs > 5)     
    mexErrMsgTxt("Too many output arguments.");

  long i,j;

  void *var1 = (void *)mxGetData(prhs[0]);
  long len1 = mxGetNumberOfElements(prhs[0]);
  

  void *var2 = (void *)mxGetData(prhs[1]);
  long len2 = mxGetNumberOfElements(prhs[1]);
  	
  if (!var1 || !var2 || !len1 || !len2)
    mexErrMsgTxt("At least one of the input vectors is invalid.");
  if (len1!=len2)
    mexErrMsgTxt("The two vectors/images should have the same length.");

  int b_findstatenum = 1;
  int nstate1 = 0, nstate2 = 0;

  int * vec1 = new int[len1];
  int * vec2 = new int[len2];
  int nrealstate1=0, nrealstate2=0;
  int minn1,maxx1,minn2,maxx2;  
    
  copyvecdata((double *)var1,len1,vec1,nrealstate1,minn1,maxx1);

  copyvecdata((double *)var2,len2,vec2,nrealstate2,minn2,maxx2);

  if (nstate1<nrealstate1)
  {
    nstate1 = nrealstate1;
  }
  if (nstate2<nrealstate2)
  {
    nstate2 = nrealstate2;
  }

  mxArray * m_JointProbab = mxCreateDoubleMatrix(nstate1,nstate2,mxREAL);
  double *hab = (double *) mxGetPr(m_JointProbab);
  double **hab2d = new double * [nstate2];
  
  for(j=0;j<nstate2;j++) 
  {
    hab2d[j] = hab + (long)j*nstate1;
  }
    
  for (i=0; i<nstate1;i++)
  {
    for (j=0; j<nstate2;j++)
      {
        hab2d[j][i] = 0;
      }
  }
  
  
  for (i=0;i<len1;i++)
  {
    hab2d[vec2[i]][vec1[i]] += 1;
  }
  
  
  for (i=0; i<nstate1;i++)
  {
    for (j=0; j<nstate2;j++)
    {
      hab2d[j][i] /= len1;
    }
     
  }
    

  plhs[0] = mxCreateDoubleMatrix(len1,1,mxREAL);
  double * jslist = (double *)mxGetPr(plhs[0]);
  
  long * tmphab = new long [long(nstate1)*nstate2];
  long nmaxstate = 0;
  for (i=0;i<(long)nstate1*nstate2;i++)
  {
    if (hab[i]!=0) 
    {
        nmaxstate++;
        tmphab[i]=nmaxstate;
    }
    else tmphab[i]=-1;
  }

  for (i=0;i<len1;i++)
  {
    jslist[i] = tmphab[vec2[i]*nstate1+vec1[i]];
  }
