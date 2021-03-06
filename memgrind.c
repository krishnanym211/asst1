#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <sys/time.h>

#include "mymalloc.h"

//malloc() 1 byte and immediately free it - do this 150 times
 int testA(){
     int count = 0;
     int total = 0;
     while (count < 100){
         clock_t start = clock(), diff;
         int i;
         for(i = 0;i < 150; i++){
             char * input = (char *) malloc(sizeof(char));
             free(input);
         }
         diff = clock() - start;
         total += diff;
         count++;
     }
     return total/100;
 }

//malloc() 1 byte, store the pointer in an array - do this 150 times.
//Once you've malloc()ed 50 byte chunks, then free() the 50 1 byte pointers one by one.
int testB(){
     int count = 0;
     int total = 0;
     while (count < 100){
         clock_t start = clock(), diff;
         char * inputs[150];
         int i;
         for (i = 0;i<150;i++){
             inputs[i] = (char *) malloc(1);
             if (inputs[i] == NULL){
                 return -1;
             }
         }
         int j;
         for (j = 0;j<150;j++){
             free(inputs[j]);
         }
         diff = clock() - start;
         total+=diff;
         count++;
     }
     return total/100;
 }
//Returns a random integer with the range of min and max
int rand_lim(int min, int max){
    
    return ((double)rand() / (((double)RAND_MAX) + 1.0)) * (max-min+1) + min;
}

//Removes the pointer at index and shifts the values in the array up to fill the vacancy.
void deleteArrayItem( int index, int length, char ** array){
    int i;
    for (i = index; i < length - 1; i++ ){
        array[i] = array[i + 1];
    }
}

 //Randomly choose between a 1 byte malloc() or free()ing a 1 byte pointer
 int testC(){
     int count = 0;
     int total = 0;
     char* input[50];
     
     int mallocIndex = 0;;
     int mallocCounter = 0;
     
     while (count < 100){
        clock_t start = clock(), diff;
         
         int r = rand_lim(0,1);
         while(mallocCounter < 50){
             int pick = rand_lim(0,1);
             if(pick == 0){
                 //perform malloc
                 int size = 1;
                 void * ptr = malloc(size);
                 if (ptr != NULL){
                     input[mallocIndex] = ptr;
                     mallocIndex++;
                 }
                 mallocCounter++;
             }else{
                 //perform free
                 int index = rand_lim(0,mallocIndex-1);
                 free(input[index]);
                 char ** start = &input[0];
                 deleteArrayItem(index, mallocIndex, start);
                 mallocIndex--;
             }
         }
         while(mallocIndex > 0){
             free(input[mallocIndex]);
            mallocIndex--;
         }
         diff = clock() - start;
         total+=diff;
         count++;
     }
     return total/100;
 }

void testDLogic(){

    char* mallocPtrs[100] = { NULL };
    int mallocCount = 0;
    int operation;

    int mallocSize = 0;
    char* ptr;

    //if our random int generator returns 0 call
    //malloc, else call free

    loop: while(mallocCount < 50){
        operation = rand_lim(0, 1);
        if(operation == 0){
            //call malloc
            mallocSize = rand_lim(1, 64);
            ptr = (char*)malloc(sizeof(char)*mallocSize);
            // printf("Malloced ptr: %p\n", ptr);
            mallocPtrs[mallocCount] = ptr;
            mallocCount++;
        }
        else{
            //call free
            //find ptr to free
            int i;
            for(i = 0; i < 50; i++){
                if(mallocPtrs[i]){
                    //free first non-NULL pointer and continue loop
                    free(mallocPtrs[i]);
                    // printf("Freed ptr: %p\n", mallocPtrs[i]);
                    mallocPtrs[i] = NULL;
                    goto loop;
                }
            }
        }

    }

    //free remaining pointers
    int i;
    for(i = 0; i < 50; i++){
        if(mallocPtrs[i]){
            free(mallocPtrs[i]);
            // printf("Freed ptr: %p\n", mallocPtrs[i]);
            mallocPtrs[i] = NULL;
        }
    }
}


int testD(){

    int diffTime = 0;
    int totalTime = 0;
    clock_t start;

    int count = 0;

    while (count < 100){
        start = clock();
        testDLogic();
        diffTime = clock() - start;
        totalTime+=diffTime;
        count++;
     }
     return totalTime/100;
}

void testELogic(){

    char* mallocPtrs[100] = { NULL };
    int mallocCount = 0;
    int operation;

    int mallocSize = 0;
    char* ptr;

    //if our random int generator returns 0 call
    //malloc, else call free

    loop: while(mallocCount < 25){
        operation = rand_lim(0, 1);
        if(operation == 0){
            //call malloc
            mallocSize = rand_lim(1, 100);
            ptr = (char*)malloc(sizeof(char)*mallocSize);
            // printf("Malloced ptr: %p\n", ptr);
            mallocPtrs[mallocCount] = ptr;
            mallocCount++;
        }
        else{
            //call free
            //find ptr to free
            int i;
            for(i = 0; i < 50; i++){
                if(mallocPtrs[i]){
                    //free first non-NULL pointer and continue loop
                    //use random function to decide if we 
                    // try to free real ptr or offset of ptr (not allocated)
                    //tests error checking
                    //tests multiple freeing of same pointer
                    int j;
                    for(j = 0; j < 3; j++){
                        free(mallocPtrs[i]+rand_lim(0, 1));
                    }
                    // printf("Freed ptr: %p\n", mallocPtrs[i]);
                    mallocPtrs[i] = NULL;
                    goto loop;
                }
            }
        }

    }

    //free remaining pointers
    int i;
    for(i = 0; i < 50; i++){
        if(mallocPtrs[i]){
            free(mallocPtrs[i]);
            // printf("Freed ptr: %p\n", mallocPtrs[i]);
            mallocPtrs[i] = NULL;
        }
    }

}


int testE(){

    int diffTime = 0;
    int totalTime = 0;
    clock_t start;

    int count = 0;

    while (count < 100){
        start = clock();
        testELogic();
        diffTime = clock() - start;
        totalTime+=diffTime;
        count++;
     }
     return totalTime/100;

}

int testF(){
	return 0;
}

int main (){
    printf("Average Run Time:\n");
    printf("TestA: %d microseconds\n", testA());
    printf("TestB: %d microseconds\n", testB());
    printf("TestC: %d microseconds\n", testC());
    printf("TestD: %d microseconds\n", testD());
    printf("TestE: %d microseconds\n", testE());
    //printf("TestF: %d microseconds\n", testF());
    return 0;
    
    

    /*//free tests
    free(10);
    char* ptr = (char*)malloc(10);
    free(ptr);
    ptr = (char*)malloc(10);
    free(ptr - 20); //should not work, however myfree should search for address
    ptr = (char*)malloc(10);
    free(ptr - 1000); //should not work, myfree should say address out of range
    
    printf("sizeof int: %lu\n", sizeof(int));
    printf("sizeof metadata %lu\n", sizeof(metadata));
    

    //this testcase makes for an exact fit when allocating char* k,
    //and also tests that the block splits when block size > requested block size
    // int sizeof_i = 10;
    // char* i = (char*)malloc(sizeof_i);
    // printf("i: %p\n", i);

    // char* j = (char*)malloc(4013);
    // printf("j: %p\n", j);

    // char* k = (char*)malloc(1);
    // printf("k: %p\n", k);


    return 0;*/
}
