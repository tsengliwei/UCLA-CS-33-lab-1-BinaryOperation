//
//  main.c
//  lab1
//
//  Created by Li-Wei Tseng on 10/15/14.
//  Copyright (c) 2014 Li-Wei Tseng. All rights reserved.
//

#include <stdio.h>

#include <math.h>


void to_binary  ( int  n, int w, int *x, int *o ) ;
void from_binary( int *x, int w, int *n ) ;
void adder( int *x, int *y, int *z, int *o, int w ) ;
void mult ( int *x, int *y, int *z, int *o, int w ) ;

void main()
{
    int a,b,c,d,w,n,o ;
    int x[32], y[32], z[32] ;
    
    o = 0 ;
    w = 9 ; /* word size in bits */
    /* note that x, y, and z must be at least as large as w and may be larger */
    
    b = 1 ; /* add one to each */
    for( c=250;c<260;c++ )
    {
        if ( c > 255 )  /* exceeds 8 bits */
            d = c-512 ;
        else
            d =  c ;
        printf( "%d %d ", d,b ) ;
        to_binary( d,w,x,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        to_binary( b,w,y,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",b ) ;
        adder( x,y,z,&o,w ) ;
        from_binary( z,w,&a ) ;
        printf( "%d ", a ) ;
        if ( o )
            printf( "Overflow in adder! " ) ;
        printf( "\n" ) ;
    }
    
    b = -1 ; /* add -1 to each */
    for( c=250;c<260;c++ )
    {
        if ( c > 255 )
            d = c-512 ;
        else
            d =  c ;
        printf( "%d %d ", d,b ) ;
        to_binary( d,w,x,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        to_binary( b,w,y,&o ) ;
        if ( o )
            printf( "%d Overflow in to_binary! ",d ) ;
        adder( x,y,z,&o,w ) ;
        from_binary( z,w,&a ) ;
        printf( "%d ", a ) ;
        if ( o )
            printf( "Overflow in adder! " ) ;
        printf( "\n" ) ;
    }
    
    
    for( w=8; w<13; w=w+4 )          /* word sizes 8 and 12 bits */
        for( a=99; a >-100; a=a-99 )  /* 99, 0, -99 */
            for( b=4; b>-5; b=b-4 )    /* 4, 0, -4 */
            {
                to_binary( a,w,x,&o ) ;
                if ( o )
                    printf( "%d Overflow in to_binary! ",a ) ;
                to_binary( b,w,y,&o ) ;
                if ( o )
                    printf( "%d Overflow in to_binary! ",b ) ;
                mult( x,y,z,&o,w ) ;
                printf( "%d %d ", a,b ) ;
                from_binary( z,w,&n ) ;
                printf( "%d ", n ) ;
                if ( o )
                    printf( "Overflow in multiplier! " ) ;
                printf( "\n" ) ;
            }
}

void adder( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first addend */
    /* y is an input array of w ints, either 0 or 1 representing the second addend */
    /* z is an output array of w ints, either 0 or 1 representing the sum of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */
    
    
    if (w > 0) { //check sizes
        
        int semiResult[w];
        int carryOut[w+1];
        carryOut[0] = 0; // carryOut shifted left a digit
        //        for (int i = 0; i < w; i++) {
        //            printf("%d", semiResult[i]);
        //        }
        for (int i = 0; i < w; i++) {
            semiResult[i] = x[i] ^ y[i];
            carryOut[i+1] = x[i] & y[i];
            // generate semiResult and carryOut
            
            z[i] = semiResult[i] ^ carryOut[i]; // generate close-to-final result
        }
        
        
        int tempCarry[w+1];
        for (int i = 0; i < w+1; i++) {
            tempCarry[i] = 0;
        }
        
        tempCarry[0] = 0;
        for (int i = 0; i < w; i++) {
            if (semiResult[i] & carryOut[i]) {
                tempCarry[i+1] = 1;
            }
            else {
                if ((semiResult[i] & tempCarry[i]) | (carryOut[i] & tempCarry[i])) {
                    tempCarry[i+1] = 1; // generate tempCarry to be the carryout of sum of semiResult and carryOut
                }
            }
        }
        
        for (int i = 0; i < w; i++) {
            z[i] = tempCarry[i] ^ z[i];
        }
        
        //over flow condition: pos + pos = neg; neg + neg = pos
        if (x[w-1] == y[w-1]) { // check for same signs
            if (x[w-1] != z[w-1]) { // if result has sign change: overflow
                *o = 1; // flag o as overflow
            }
        }
    }
}


void mult ( int *x, int *y, int *z, int *o, int w )
{
    /* x is an input array of w ints, either 0 or 1 representing the first multiplicand */
    /* y is an input array of w ints, either 0 or 1 representing the second multicand */
    /* z is an output array of w ints, either 0 or 1 representing the product of x and y */
    /* o is an output = 1 if an overflow occurred */
    /* w is an input = to the width in bits of x, y, z */
    
    int resultAdders[w][w]; // have a 2 D array to store multiplication result later to be added
    int placeHolder;
    
    // create results adder
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < w; j++) {
            resultAdders[i][j] = 0;
        }
    }
    
    for (int i = 0; i < w; i++) {
        if (y[i]) {
            int k = 0;
            for (int j = i; j < w; j++) {
                resultAdders[i][j] = x[k];
                k++;
            }
        }
    }
    
    
    
    //    // testing
    //    for (int i = 0; i < w; i++) {
    //        for (int j = 0; j < w; j++) {
    //            int d = resultAdders[i][j];
    //            printf("%d", d);
    //        }
    //        printf("\n");
    //    }
    
    
    
    
    // create temp array with 0s
    for (int i = 0; i < w; i++) {
        z[i] = 0;
    }
    
    
    // Add the results
    for (int i = 0; i < w; i++) {
        adder(z, resultAdders[i], z, &placeHolder, w);
    }
    
    
    
    // overflow when sign rules are violated: neg * neg = neg, pos * pos = neg, pos * neg = pos, neg * pos  = pos.
    if (x[w-1] & y[w-1]) {
        if (x[w-1] & z[w-1]) { // overflow: neg * neg = neg
            *o = 1;
        }
        else
        {
            *o = 0;
        }
    } else if (x[w-1] ^ y[w-1]) {
        if (~x[w-1] & (y[w-1] ^ z[w-1])) { // overflow: pos * neg = pos
            *o = 1;
        }
        else if (~y[w-1] & (x[w-1] ^ z[w-1])) { // overflow: neg * pos = pos
            *o = 1;
        }
        else
        {
            *o = 0;
        }
        
    } else {
        if (x[w-1] ^ z[w-1]) { // overflow: pos * pos = neg
            *o = 1;
        }
        else
        {
            *o = 0;
        }
        
    }
    
    int isZeroX, isZeroY, isZeroZ;
    from_binary(x, w, &isZeroX);
    from_binary(y, w, &isZeroY);
    from_binary(z, w, &isZeroZ);
    if (isZeroX == 0 ||  isZeroY == 0) { // if either x or y is zero, the output should be zero
        if (isZeroZ == 0) {
            *o = 0;
        }
    }
}


void to_binary( int n, int w, int *x, int *o )
{
    /* n is an input and is the number to be converted to binary */
    /* w is an input = to the width in bits of x */
    /* x is an output array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* o is an output = 1 if an overflow occurred ( n is too large for w bits ) */
    
    // check if the input has a valid length
    if (w > 0)
    {
        // check if the input is negative
        
        if (n < 0)
        {
            int temp = -n; //convert n to positive first
            
            int i = 0;
            
            *o = 0;
            
            while (temp != 0) // convert to binary number
            {
                if (i > w - 1) {
                    *o = 1; // overflow
                    break;
                }
                else{
                    x[i] = temp % 2;
                    temp  = temp / 2;
                    i++;
                }
                
            }
            for (int j = i; j < w; j++) {
                x[j] = 0;
            }
            
            
            // invert 1 and 0s
            for (int j = 0; j < w; j++) {
                x[j] = (x[j] == 1) ? 0 : 1;
            }
            
            // add one
            for (int k = 0; k < w; k++) {
                if (x[k] == 0) {
                    x[k] = 1;
                    break;
                }
                else if (x[k] == 1)
                {
                    x[k] = 0;
                }
            }
        }
        else
        {
            int i = 0;
            
            *o = 0;
            
            while (n != 0)
            {
                if (i > w - 1) {
                    *o = 1; // overflow
                    break;
                }
                else{
                    x[i] = n % 2;
                    n  = n / 2;
                    i++;
                }
            }
            for (int j = i; j < w; j++) {
                x[j] = 0;
            }
        }
    }
}

void from_binary( int *x, int w, int *n )
{
    /* x is an input array of w ints, either 0 or 1: the binary representation of n */
    /*           x[0] is the least significant bit   */
    /*           x[w-1] is the most signifcant bit, for a signed number, it is the sign */
    /* w is an input = to the width in bits of x */
    /* n is an output and will be equal to the value stored in binary in x */
    
    // check if the input has a valid length
    if(w > 0)
    {
        int decimalNum = 0;
        for (int i = 0; i < w; i++)
        {
            if ((i == w - 1) && (x[i] == 1))
            {
                // check if the input is a negative number
                decimalNum += - x[i] * pow(2, i);
            }
            else
            {
                decimalNum += x[i] * pow(2, i);
            }
        }
        *n = decimalNum;
    }
}
