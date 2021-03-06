#ifndef GLTEXT_H
#define GLTEXT_H

    const static GLbyte LLL = '~';

    
     static GLbyte E[64]= {    0,0,0,0,0,0,0,0,
                                        0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,LLL,LLL,LLL,LLL,0,0,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                        0,0,0,0,0,0,0,0,};
    
     static GLbyte A[64]= {    0,0,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,0,LLL,0,0,LLL,0,0,
                                        0,0,0,LLL,LLL,0,0,0,
                                        0,0,0,0,0,0,0,0,};
    
         static GLbyte B[64]= {    0,0,0,0,0,0,0,0,
                                            0,LLL,LLL,LLL,LLL,LLL,0,0,
                                            0,LLL,0,0,0,0,LLL,0,
                                            0,LLL,0,0,0,0,LLL,0,
                                            0,LLL,LLL,LLL,LLL,LLL,0,0,
                                            0,LLL,0,0,0,0,LLL,0,
                                            0,LLL,LLL,LLL,LLL,LLL,0,0,
                                            0,0,0,0,0,0,0,0,};
        
             static GLbyte C[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,0,0,0,0,0,0,0,};
        
 static GLbyte D[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};
    
 static GLbyte F[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,0,0,0,0,0,0,};

     static GLbyte G[64] = {   0,0,0,0,0,0,0,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,LLL,LLL,LLL,LLL,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte H[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte I[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,LLL,LLL,LLL,0,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte J[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte K[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,LLL,0,0,
                                    0,LLL,0,0,LLL,0,0,0,
                                    0,LLL,LLL,LLL,0,0,0,0,
                                    0,LLL,0,0,LLL,0,0,0,
                                    0,LLL,0,0,0,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte L[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte M[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,LLL,LLL,0,LLL,0,
                                    0,LLL,LLL,0,0,LLL,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte N[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,LLL,LLL,0,
                                    0,LLL,0,0,LLL,0,LLL,0,
                                    0,LLL,0,LLL,0,0,LLL,0,
                                    0,LLL,LLL,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

     static GLbyte O[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte P[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte Q[64]= {    0,0,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,LLL,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

     static GLbyte R[64]= {   0,0,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,LLL,LLL,LLL,LLL,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,LLL,LLL,LLL,LLL,LLL,0,0,
                                        0,0,0,0,0,0,0,0,};
    
         static GLbyte S[64] = {   0,0,0,0,0,0,0,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,0,0,0,LLL,LLL,0,0,
                                        0,0,LLL,LLL,0,0,0,0,
                                        0,LLL,0,0,0,0,LLL,0,
                                        0,0,LLL,LLL,LLL,LLL,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte T[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte U[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte V[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,0,LLL,LLL,0,0,0,
                                    0,0,LLL,0,0,LLL,0,0,
                                    0,0,LLL,0,0,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte W[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,LLL,0,0,LLL,0,0,
                                    0,LLL,0,LLL,LLL,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte X[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,LLL,0,0,LLL,0,0,
                                    0,0,0,LLL,LLL,0,0,0,
                                    0,0,LLL,0,0,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte Y[64]= {    0,0,0,0,0,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,LLL,0,LLL,0,0,
                                    0,0,LLL,0,0,0,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte Z[64]= {    0,0,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,LLL,0,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,0,LLL,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _1[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,LLL,0,LLL,0,0,0,
                                    0,0,0,LLL,LLL,0,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _2[64]= {   0,0,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _3[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _4[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,0,0,0,LLL,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,LLL,0,0,LLL,0,0,
                                    0,0,0,LLL,0,LLL,0,0,
                                    0,0,0,0,LLL,LLL,0,0,
                                    0,0,0,0,0,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _5[64]= {   0,0,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _6[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,LLL,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _7[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,LLL,0,0,0,0,0,
                                    0,0,0,LLL,0,0,0,0,
                                    0,0,0,0,LLL,0,0,0,
                                    0,0,0,0,0,LLL,0,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,LLL,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _8[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _9[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,LLL,0,
                                    0,LLL,0,0,0,0,LLL,0,
                                    0,0,LLL,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _0[64]= {   0,0,0,0,0,0,0,0,
                                    0,0,0,LLL,LLL,LLL,0,0,
                                    0,0,LLL,0,0,0,LLL,0,
                                    0,0,LLL,0,0,0,LLL,0,
                                    0,0,LLL,0,0,0,LLL,0,
                                    0,0,LLL,0,0,0,LLL,0,
                                    0,0,0,LLL,LLL,LLL,0,0,
                                    0,0,0,0,0,0,0,0,};

 static GLbyte _colon[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _semicolon[64]= {   0,0,0,0,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _lessthan[64]= {    0,0,0,0,0,0,0,0,
                                            0,0,0,0,LLL,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,LLL,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _equal[64]= {       0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _morethan[64]= {    0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,LLL,0,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,LLL,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _question[64]= {    0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,LLL,0,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,LLL,0,0,0,LLL,0,0,
                                            0,0,LLL,LLL,LLL,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _at[64]= {  0,0,0,0,0,0,0,0,
                                    0,0,0,LLL,LLL,LLL,LLL,0,
                                    0,0,LLL,0,0,0,0,0,
                                    0,LLL,0,LLL,LLL,LLL,LLL,LLL,
                                    0,LLL,0,LLL,0,0,LLL,LLL,
                                    0,LLL,0,0,LLL,LLL,0,LLL,
                                    0,0,LLL,0,0,0,0,LLL,
                                    0,0,0,LLL,LLL,LLL,LLL,0,};

 static GLbyte _space[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _exclamation[64]= { 0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _quote[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,LLL,0,0,0,LLL,0,0,
                                        0,0,LLL,0,0,0,LLL,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _number[64]= {  0,0,0,0,0,0,0,0,
                                        0,0,LLL,0,0,LLL,0,0,
                                        0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                        0,0,LLL,0,0,LLL,0,0,
                                        0,0,LLL,0,0,LLL,0,0,
                                        0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                        0,0,LLL,0,0,LLL,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _dollar[64] = {     0,0,LLL,0,0,LLL,0,0,
                                            0,0,LLL,LLL,LLL,LLL,0,0,
                                            0,LLL,0,0,0,0,LLL,0,
                                            0,0,0,0,LLL,LLL,0,0,
                                            0,0,LLL,LLL,0,0,0,0,
                                            0,LLL,0,0,0,0,LLL,0,
                                            0,0,LLL,LLL,LLL,LLL,0,0,
                                            0,0,0,LLL,0,LLL,0,0,};

 static GLbyte _percent[64]= { 0,0,0,0,0,0,0,0,
                                        0,LLL,0,0,0,LLL,0,0,
                                        0,0,LLL,0,LLL,0,LLL,0,
                                        0,0,0,LLL,0,LLL,0,0,
                                        0,0,LLL,0,LLL,0,0,0,
                                        0,LLL,0,LLL,0,LLL,0,0,
                                        0,0,LLL,0,0,0,LLL,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _ampersand[64]= {   0,0,0,0,0,0,0,0,
                                            0,0,LLL,LLL,LLL,0,LLL,0,
                                            0,LLL,0,0,0,LLL,0,0,
                                            0,LLL,0,0,0,LLL,LLL,0,
                                            0,0,LLL,LLL,LLL,0,0,0,
                                            0,LLL,0,0,0,LLL,0,0,
                                            0,0,LLL,LLL,LLL,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _apostrophe[64]= {  0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _leftbrace[64]= {   0,0,0,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,LLL,0,0,0,0,0,
                                            0,0,0,LLL,0,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _rightbrace[64]= {  0,0,0,0,0,0,0,0,
                                            0,0,0,0,LLL,0,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,0,LLL,0,0,
                                            0,0,0,0,LLL,0,0,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _asterisk[64]= {    0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,0,0,0,0,0,0,
                                            0,0,LLL,0,LLL,0,LLL,0,
                                            0,0,0,LLL,LLL,LLL,0,0,
                                            0,0,0,LLL,LLL,LLL,0,0,
                                            0,0,LLL,0,LLL,0,LLL,0,
                                            0,0,0,0,0,0,0,0,};

 static GLbyte _plus[64]= {    0,0,0,0,0,0,0,0,
                                        0,0,0,0,LLL,0,0,0,
                                        0,0,0,0,LLL,0,0,0,
                                        0,0,LLL,LLL,LLL,LLL,LLL,0,
                                        0,0,0,0,LLL,0,0,0,
                                        0,0,0,0,LLL,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _comma[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,LLL,0,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _minus[64]= {   0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,LLL,LLL,LLL,LLL,LLL,LLL,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _dot[64]= {     0,0,0,0,0,0,0,0,
                                        0,0,LLL,LLL,0,0,0,0,
                                        0,0,LLL,LLL,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,
                                        0,0,0,0,0,0,0,0,};

 static GLbyte _slash[64]= {   0,0,0,0,0,0,0,0,
                                        0,LLL,0,0,0,0,0,0,
                                        0,0,LLL,0,0,0,0,0,
                                        0,0,0,LLL,0,0,0,0,
                                        0,0,0,0,LLL,0,0,0,
                                        0,0,0,0,0,LLL,0,0,
                                        0,0,0,0,0,0,LLL,0,
                                        0,0,0,0,0,0,0,0,};


#endif