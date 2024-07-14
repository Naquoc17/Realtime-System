#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// ********************************************************************************
// RTS SumSem 2024
// Exercise: Execution Times
// Name: Anh Quoc Nguyen
// last change: Sonday May 26 06:00:50 CEST 2024
//
// ********************************************************************************

// please implement the function diff_time
struct timespec diff_time(struct timespec *t_Start, struct timespec *t_Compl) {
	// calculate: dt= t_Compl - t-Start
	// the accuracy has to be nanoseconds
	// note: it is not complicated but not as simple as in the line above
    struct timespec dt;
    dt.tv_nsec = t_Compl->tv_nsec - t_Start->tv_nsec;
    dt.tv_sec = t_Compl->tv_sec - t_Start->tv_sec;

    if (dt.tv_nsec < 0){
        dt.tv_nsec += 1000000000;
        dt.tv_sec -= 1;
    }

	return dt;
};

//  Do not change the function myfunc1 !
int myfunc1 (int in){
    int i, j = in;
    for(i=0; i <= 999; i++) j=j+i;
    return j;
}

  //  Do not change the function myfunc2 !
int myfunc2 (int in){
    int val1, i;
    float val2, val3;
    for (i =0; i < 4500; i++)
    {
        val1 = in*in*i;
        val2 = (in+i) * (in+i) * (in+i);
        val3 = val2 / val1;
    }
    return val3;
}

 // Do 500000 function calls and print the time results.
 // Use the first parameter of argv as the probability for calling myfunc1

int main ( int argc , char ** argv ){
    int i, randomValue;

	// time stamps before and after the loop:
    struct timespec t_Start, t_Compl;
	// note: since we don't have concurrency, in this case dt_Req and dt_Start are the same

	//  variable for the execution time	
	struct timespec dt_exec; 

    // handle probability argument argv
	if(argc > 2){
        printf("The input value is invalid!");
        return 1;
    }

    int probFunc1 = atoi(argv[1]);
    printf("myfunc1 probability: %d\n", probFunc1);
    printf("myfunc2 probability: %d\n", 100-probFunc1);

    // get the time stamp for the start time t_Start
	clock_gettime(CLOCK_REALTIME, &t_Start);

    for (i = 0; i < 500000; i++){ //  e stands for exponent. 5e5 = 5 * 10^5 = 500000
        randomValue = rand()%100;
        if ( randomValue <= probFunc1-1 )
            myfunc1(randomValue);
        else
            myfunc2(randomValue-1234);
    }

    // get the time stamp for the completion time t_Compl
	clock_gettime(CLOCK_REALTIME, &t_Compl);

	// calculate the execution time dt_exec
    dt_exec = diff_time(&t_Start, &t_Compl);

	// print the results
    printf("Start-time      : %11ld s %9ld ns\n", t_Start.tv_sec,  t_Start.tv_nsec);
    printf("Completion-time : %11ld s %9ld ns\n", t_Compl.tv_sec,  t_Compl.tv_nsec);
    printf("Execution-time  : %11ld s %9ld ns\n",  dt_exec.tv_sec,  dt_exec.tv_nsec);
    // printf("Execution-time in sec: %.4f s\n", dt_exec.tv_sec +  dt_exec.tv_nsec);
    return 0;
}
