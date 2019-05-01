/*
 * shuffle.c
 *
 *  Created on: May 31, 2018
 *      Author: imoralessirgo
 */

#include <stdlib.h>
#include <stdio.h>
#include "shuffle.h"

void shuffle(int a[], int n, int seed){
    srand(seed);

    int i;
    for (i = n-1; i > 0; i--){

        // generating a number between 0 and i
        int o = rand() % (i+1);

        // making the switch
        int t = a[i];
        a[i] = a[o];
        a[o] = t;
    }

}


