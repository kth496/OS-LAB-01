#include "include/lab1_sched_types.h"
#include <bits/stdc++.h>
using namespace std;

/*
 * Set a workload here
 */
int arrival_times[MAX_PROCESSES] = {0, 2, 4, 6, 8};
int burst_times[MAX_PROCESSES] = {3, 6, 4, 5, 2};

int main() {
        /*
         * Declare pg(Array of _process, means Process Group)
         * and initialize each process's property using set_workload()
         *
         * Cause this program handles all argument as a memory reference, it is
         * necessary to declare *ptr and *ptr_result.
         *
         */
        _process pg[MAX_PROCESSES];
        _process *ptr = pg;
        int result[MAX_TIME];
        int *ptr_result = result;
        set_workload(arrival_times, burst_times, ptr);

        /*
         *
         */
        FIFO(ptr_result, ptr);
        // print_result_table(ptr_result);
        reset_job_state(ptr);

        RoundRobin(ptr_result, ptr, 1);
        print_result_table(ptr_result);

        return 0;
}