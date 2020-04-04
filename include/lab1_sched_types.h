#pragma once
#define MAX_PROCESSES 5
#define MAX_TIME 20

/**
 * _process - Contains various value and array that indicate process
 *
 * @pid - process id.
 * @arrival_time - Indicate the time of arrival
 * @burst_time - Time to complete a given job.
 * @remain_time - Remain time while job is scheduled.
 */
class _process {
      public:
        int pid;
        int arrival_time;
        int burst_time;
        int remain_time;
        bool isDone;
        _process() {
                arrival_time = 0;
                burst_time = 0;
                remain_time = 0;
                isDone = false;
        };
};

/*
 * Comment about the functions are written in "lab1_sched.cpp"
 */
void set_workload(int *arriv, int *burst, _process *&pg);

void print_result_table(const int *ret);

void reset_job_state(_process *&pg);

void FIFO(int *&ret, _process *&pg);

void RoundRobin(int *&ret, _process *&pg, int quantum);