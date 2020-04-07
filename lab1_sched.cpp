#include "include/lab1_sched_types.h"
#include <bits/stdc++.h>
using namespace std;

/*
 * set_workload
 *
 * Initialize the state of each process using arriv and burst array
 *
 * @arriv - Array of when the process arrived.
 *
 * @burst - Array of time of process burst time.
 */
void set_workload(int *arriv, int *burst, _process *&pg) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
                pg[i].pid = i;
                pg[i].arrival_time = arriv[i];
                pg[i].burst_time = burst[i];
                pg[i].remain_time = burst[i];
        }
}

/*
 * print_result_table
 *
 * Print out the result of scheduling algorithm.
 */
void print_result_table(int *&ret, _process *&pg) {
        char label[] = {'A', 'B', 'C', 'D', 'E'};
        printf("          arr job ");
        for (int i = 0; i < MAX_TIME; i++) {
                printf(" %2d", i);
        }
        printf("\n-------------------------------------------------------------"
               "-------------------\n");
        for (int p = 0; p < MAX_PROCESSES; p++) {
                printf("Process %c  %d  %dms ", label[p], pg[p].arrival_time,
                       pg[p].burst_time);
                for (int t = 0; t < MAX_TIME; t++) {
                        if (ret[t] == p)
                                printf(" **");
                        else
                                printf("   ");
                }
                printf("\n");
        }
        printf("\n");
}

/*
 * reset_job_state
 *
 * Reset all states of each process to initial condition.
 */
void reset_job_state(_process *&pg) {
        for (int p = 0; p < MAX_PROCESSES; p++) {
                pg[p].isDone = false;
                pg[p].isScheduled = false;
                pg[p].remain_time = pg[p].burst_time;
        }
}

/*
 * push_process_to_readyQ
 *
 * Find a process which already arrived.
 * Push them to ready queue.
 */
void push_process_to_readyQ(int curTime, queue<_process *> *&Q, _process *&pg) {
        for (int j = 0; j < MAX_PROCESSES; j++) {
                _process *tmp = &pg[j];
                if ((!tmp->isDone) && (!tmp->isScheduled) &&
                    (tmp->arrival_time <= curTime)) {
                        (*Q).push(tmp);
                        tmp->isScheduled = true;
                }
        }
}

/*
 * FIFO(FCFS)
 *
 * @ret - Indicates which process is running at that time.
 * E.g. ret[0] = 1,
 * means process '1' is running at the '0' momment of timeflow.
 *
 * @pg - (processGroup) Array of _process.
 *
 * @quantum - Useless, but need to fit the function form with others.
 *
 * @curTime - Indicates timeflow, 0 to MAX_TIME
 *
 * @Q - Ready queue.
 *
 * @Q_ptr - Pointer of ready Queue.
 */
void FIFO(int *&ret, _process *&pg, int quantum) {
        int curTime = 0;
        queue<_process *> Q;
        queue<_process *> *Q_ptr = addressof(Q);

        while (curTime < MAX_TIME) {
                push_process_to_readyQ(curTime, Q_ptr, pg);
                /*
                 * Execute all processes in the queue.
                 */
                while (!Q.empty()) {
                        _process *sched = Q.front();
                        Q.pop();
                        for (int i = curTime; i < curTime + sched->burst_time;
                             i++) {
                                ret[i] = sched->pid;
                        }
                        sched->isDone = true;
                        curTime += sched->burst_time;
                }
        }
};

/*
 * RoundRobin
 */
void RoundRobin(int *&ret, _process *&pg, int quantum) {
        /*
         * @quantum - Size of timeslice.
         *
         * @Q - Ready queue.
         *
         * @Q_ptr - Pointer of ready Queue.
         */
        int curTime = 0;
        queue<_process *> Q;
        queue<_process *> *Q_ptr = addressof(Q);

        while (curTime < MAX_TIME) {
                /*
                 * Execute all processes in the queue.
                 */
                _process *sched = NULL;
                if (!Q.empty()) {
                        sched = Q.front();
                        Q.pop();
                        for (int i = 0; i < quantum; i++) {
                                sched->remain_time--;
                                ret[curTime] = sched->pid;
                                curTime++;
                                push_process_to_readyQ(curTime, Q_ptr, pg);
                                if (sched->remain_time == 0) {
                                        sched->isDone = true;
                                        break;
                                }
                        }
                }
                push_process_to_readyQ(curTime, Q_ptr, pg);

                /*
                 * If process has some task remaining, push to queue again.
                 */
                if (sched != NULL)
                        if (!sched->isDone)
                                Q.push(sched);
        }
}

/*
 * MLFQ
 *
 * Time quantum of Q_i is calculated by quantum^i
 */
void MLFQ(int *&ret, _process *&pg, int quantum) {
        /*
         * Priority: Most highest queue is Q0.
         * Q0 > Q1 > Q2
         */
        int curTime = 0;
        queue<_process *> Q0;
        queue<_process *> Q1;
        queue<_process *> Q2;
        queue<_process *> *queueList[] = {&Q0, &Q1, &Q2};
        int Q_timeQuantum[3];

        // queue<_process *> *curQueue = &Q0;
        int curQueueIndex = 0;

        for (int i = 0; i < 3; i++) {
                Q_timeQuantum[i] = (int)pow(quantum, i);
        }

        while (curTime < MAX_TIME) {
                _process *sched = NULL;
                /*
                 * TODO:
                 *      Is it possible to optimize under WHILE loop?
                 */
                while (1) {
                        if (!(*queueList[curQueueIndex]).empty() ||
                            curQueueIndex == 2)
                                break;
                        curQueueIndex++;
                }

                /*
                 * Execute all processes in the current queue.
                 *
                 * TODO:
                 *      Replace under logics into RoundRobin.
                 */
                if (!(*queueList[curQueueIndex]).empty()) {
                        sched = (*queueList[curQueueIndex]).front();
                        (*queueList[curQueueIndex]).pop();
                        for (int i = 0; i < Q_timeQuantum[curQueueIndex]; i++) {
                                sched->remain_time--;
                                ret[curTime] = sched->pid;
                                curTime++;
                                push_process_to_readyQ(curTime, queueList[0],
                                                       pg);
                                if (sched->remain_time == 0) {
                                        sched->isDone = true;
                                        break;
                                }
                        }
                }

                push_process_to_readyQ(curTime, queueList[0], pg);

                /*
                 * TODO:
                 *      Is it possible to optimize 3-indented code?
                 */
                if (sched != NULL)
                        if (!sched->isDone) {
                                if (Q0.empty() && Q1.empty() && Q2.empty()) {
                                        (*queueList[curQueueIndex]).push(sched);
                                } else {
                                        if (curQueueIndex != 2)
                                                curQueueIndex++;
                                        (*queueList[curQueueIndex]).push(sched);
                                }
                        }

                /*
                 * Move to topmost queue.
                 */
                curQueueIndex = 0;
        }
}
