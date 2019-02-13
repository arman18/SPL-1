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
