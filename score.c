/*
 * score.c
 *
 *  Created on: Jun 6, 2018
 *      Author: imoralessirgo
 */

#include "score.h"
#include <stdio.h>

int sum_all(int arr[]){
    int i,s;
    for(s=0, i=0; i<10; i++){
        s += arr[i];
    }
    return s;
}

void fix_A(int arr[],int sum){
    int i;
    for(i=0; i<10; i++){
        if((arr[i]==1) && ((sum + 10)<=21)){
            sum += 10;
        }
    }
}

