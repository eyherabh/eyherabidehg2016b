/*
 This software is provided as supplementary material for the following publication:

 Eyherabide HG, Neural Stochastic Codes, Encoding and Decoding, arXiv (2017),
 https://arxiv.org/abs/1611.05080.

 Should you use this code, I kindly request you to cite the aforementioened publication.

 DESCRIPTION:

 Determines pairs of vectors which components which absolute different does not exceed
 a specified threshold. It is used for determining trials that are indistinguisheble.

 The code can be compiled as follows
 
   mex -v GCC='/usr/bin/gcc-4.7' -lm adjacencyMatrix.c

 where you should replace /usr/bin/gcc-4.7 for the appropriate folder
 and C compiler compatible with your Matlab installation.

 VERSION CONTROL

 V1.000 Hugo Gabriel Eyherabide (10 Feb 2017)

 Should you find bugs, please contact Hugo Gabriel Eyherabide (neuralinfo@eyherabidehg.com)

 LICENSE

 Copyright (c) 2017, Hugo Gabriel Eyherabide
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1.  Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

 2.  Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

 3.  Neither the name of the copyright holder nor the names of its contributors
     may be used to endorse or promote products derived from this software
     without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 OF SUCH DAMAGE.
*/



#include<mex.h>
#include<math.h>

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    /*
      
    FORMAT
        
      In Matlab, the function can be called as follows:
        
        adjmat = adjacencyMatrix(trials,threshold);
        
    INPUT ARGUMENTS:
      - trials: matrix with trials in the rows and time-bins in the columns.
      - threshold: the maximum value of the difference between bins at the same time
      but from two different trials that are considered indistinguishable.
      
    OUTPUT ARGUMENTS:
    
      - adjmat: an upper-triangular matrix with as many rows as number of trials, 
      indicating with unity in the row i and column j that the trials of index i and j
      are indistinguishable.
      
     */
    double      *sptrain = (double*) mxGetPr(prhs[0]);
    int         *sptsize = (int*) mxGetDimensions(prhs[0]);
    int         numtrials   = sptsize[0];
    int         numbins     = sptsize[1];
    int         sptlength   = numtrials*numbins;
    int         numtrialsm1 = numtrials-1;
    
    /* Using positive and negative thresholds is faster than fabs*/
    double      threshold   = (double) *mxGetPr(prhs[1]);
    double      thresholdm  = -threshold;    

    /*Each element of sptrial points to a pointer that points to a different trial*/
    double      **sptrial = (double**) mxMalloc(numtrials*sizeof(double*));  
    int         indt1; /* Trial index*/
    int         indt2; /* Trial index*/
    int         indb;  /* Bin index*/
    
    sptrial 
    for(indt1=0; indt1<numtrials; indt1++) 
    {
        sptrial[indt1] = sptrain; 
        sptrain+=numbins;
    }
    sptrain-=sptlength;


    double  *sptnow1;
    double  *sptnow2;
    double  sptaux;

    plhs[0] = mxCreateDoubleMatrix(numtrials,numtrials, mxREAL);
    double  *pairs = mxGetPr(plhs[0]);

    for(indt1=0; indt1<numtrialsm1; indt1++)
    {
        sptnow1 = sptrial[indt1];
        
        for(indt2=indt1+1; indt2<numtrials; indt2++)
        {   
            sptnow2 = sptrial[indt2];
        
            for(indb=0; indb<numbins; indb++)
                if((sptaux=sptnow1[indb]-sptnow2[indb])>threshold || sptaux<thresholdm) break;
                
            if(indb==numbins) pairs[indt1+indt2*numtrials] = 1; 
        }
    }
}
     

