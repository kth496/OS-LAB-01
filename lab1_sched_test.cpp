#include "include/lab1_sched_types.h"
#include <bits/stdc++.h>
using namespace std;

/** Set a workload here **/
/* CASE 1 */
int arrival_times[MAX_PROCESSES] = {0, 2, 4, 6, 8};
int burst_times[MAX_PROCESSES] = {3, 6, 4, 5, 2};

/* CASE 2 */
// int arrival_times[MAX_PROCESSES] = {0, 6, 4, 13, 11};
// int burst_times[MAX_PROCESSES] = {10, 3, 2, 4, 1};

/* CASE 3 */
// int arrival_times[MAX_PROCESSES] = {0, 6, 4, 5, 8};
// int burst_times[MAX_PROCESSES] = {7, 5, 5, 1, 2};

/* Stride tickets */
int tickets[MAX_PROCESSES] = {50, 100, 25, 10, 20};

int main() {
        /*
         * @pg - Array of _process, means Process Group
         *
         * Cause this program handles all argument as a memory reference, it is
         * necessary to declare *ptr and *ptr_result.
         *
         */
        _process pg[MAX_PROCESSES];
        _process *ptr = pg;
        int result[MAX_TIME];
        int *ptr_result = result;
        set_workload(arrival_times, burst_times, ptr, tickets);

        /*
         * Simulation
         */
        // printf("\nFIFO\n");
        // FIFO(ptr_result, ptr);
        // print_result_table(ptr_result, ptr);
        // reset_job_state(ptr);

        // printf("\nRound Robin  time quantum = 1\n");
        // RoundRobin(ptr_result, ptr, 1);
        // print_result_table(ptr_result, ptr);
        // reset_job_state(ptr);

        // printf("\nRound Robin  time quantum = 4\n");
        // RoundRobin(ptr_result, ptr, 4);
        // print_result_table(ptr_result, ptr);
        // reset_job_state(ptr);

        // printf("\nMLFQ  time quantum = 1\n");
        // MLFQ(ptr_result, ptr, 1);
        // print_result_table(ptr_result, ptr);
        // reset_job_state(ptr);

        printf("\nMLFQ  time quantum = 2i\n");
        MLFQ(ptr_result, ptr, 2);
        print_result_table(ptr_result, ptr);
        reset_job_state(ptr);

        printf("\nStride Scheduler\n");
        Stride(ptr_result, ptr);
        print_result_table(ptr_result, ptr);
        reset_job_state(ptr);
        return 0;
}