#include <math.h>
#include <mex.h>
#include<iostream>
using namespace std;

void bubleSort(double *base,double *element,double length)
{
    double temp;
    for(int i=0; i<length-1; i++)
    {
        for(int j=0;j<length-1-i;j++)
        {
            if(base[j]>base[j+1])
            {
                temp = base[j];
                base[j] = base[j+1];
                base[j+1] = temp;
                
                temp = element[j];
                element[j] = element[j+1];
                element[j+1] = temp;
            }
        }
    }
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if(nrhs!=2) mexErrMsgTxt("invalid input");
    double *data = mxGetPr(prhs[0]), *dataToBeSend;
    double length = mxGetNumberOfElements(prhs[0]);
    double *index = new double[length];
    double Q = mxGetScalar(prhs[1]);
    int frequency = (int)length/Q;
    
    //correction: do not change original *data by sorting
    
    
    double *tmpData = new double[length];
    for(int i=0; i<length; i++)
    {
        tmpData[i] = data[i];
    }
    
    for(int i=0; i<length; i++) 
    {
        index[i] = i;
    }
    
    bubleSort(tmpData,index,length);
    
    plhs[0] = mxCreateDoubleMatrix(length,1,mxREAL);
    dataToBeSend = mxGetPr(plhs[0]);
    
   for(int i=0; i<(Q-1)*frequency; i++)
   {
        dataToBeSend[(int)index[i]] = i/frequency + 1;
   }
    for(int i=(Q-1)*frequency;i<length; i++)
    {
        dataToBeSend[(int)index[i]] = Q;
    }
    
}
