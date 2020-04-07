#pragma once
#define MAX_PROCESSES 5
#define MAX_TIME 20
/*
 *      _process class:
 *
 *      - Contains various values indicate process's state.
 *
 * @pid - process id.
 * @arrival_time -
 * @burst_time - Time to complete a whole job.
 * @remain_time - Remaining time while job is scheduled.
 * @isDone - Default is false, turn to true when job is ended.
 *
 * Maybe introducing some variable which represents state of process can be
 * useful to manage scheduling queue. Also, mapping the state in the
 * hashmap STL can improve readability of codes.
 *
 */
class _process {
      public:
        int pid;
        int arrival_time;
        int burst_time;
        int remain_time;
        bool isDone;
        bool isScheduled;
        _process() {
                arrival_time = 0;
                burst_time = 0;
                remain_time = 0;
                isDone = false;
                isScheduled = false;
        };
};

/*
 * Comment about the functions are written in "lab1_sched.cpp"
 */
void set_workload(int *arriv, int *burst, _process *&pg);

void print_result_table(int *&ret, _process *&pg);

void reset_job_state(_process *&pg);

void push_process_to_readyQ(int curTime, ...);

/*
 * Scehduling Algorithm
 */
void FIFO(int *&ret, _process *&pg, int quantum);

void RoundRobin(int *&ret, _process *&pg, int quantum);

void MLFQ(int *&ret, _process *&pg, int quantum);