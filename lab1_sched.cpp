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
void set_workload(int *arriv, int *burst, _process *&pg, int *tickets) {
        for (int i = 0; i < MAX_PROCESSES; i++) {
                pg[i].pid = i;
                pg[i].arrival_time = arriv[i];
                pg[i].burst_time = burst[i];
                pg[i].remain_time = burst[i];
                pg[i].ticket = tickets[i];
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

void push_readyQ_v2(int curTime, queue<_process *> *Q, _process *pg) {
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
 * @ret - Denotes which process is running at that time.
 * E.g. ret[0] = 1,
 * means process '1' is running at the '0' momment of timeflow.
 *
 * @pg - (processGroup) Array of _process.
 *
 * @curTime - Denotes timeflow, 0 to MAX_TIME
 *
 * @Q - Ready queue.
 */
void FIFO(int *ret, _process *pg) {
        int curTime = 0;
        queue<_process *> Q;

        while (curTime < MAX_TIME) {
                push_readyQ_v2(curTime, &Q, pg);
                /*
                 * Execute all processes in the queue.
                 */
                while (!Q.empty()) {
                        _process *sched = Q.front();
                        Q.pop();
                        for (int i = 0; i < sched->burst_time; i++) {
                                ret[i + curTime] = sched->pid;
                                push_readyQ_v2(curTime + i, &Q, pg);
                        }
                        sched->isDone = true;
                        curTime += sched->burst_time;
                }
        }
};

/*
 * RoundRobin
 */
void RoundRobin(int *ret, _process *pg, int quantum) {
        /*
         * @quantum - Size of timeslice.
         *
         * @Q - Ready queue.
         */
        int curTime = 0;
        queue<_process *> Q;
        _process *sched;

        while (curTime < MAX_TIME) {
                /*
                 * Execute all processes in the queue.
                 */
                push_readyQ_v2(curTime, &Q, pg);
                if (!Q.empty()) {
                        sched = Q.front();
                        Q.pop();
                        for (int i = 0; i < quantum; i++) {
                                sched->remain_time--;
                                ret[curTime] = sched->pid;
                                curTime++;
                                push_readyQ_v2(curTime, &Q, pg);
                                if (sched->remain_time == 0) {
                                        sched->isDone = true;
                                        break;
                                }
                        }
                }

                /*
                 * If process has some task remaining, push to queue again.
                 */
                if (!sched->isDone)
                        Q.push(sched);
        }
}

/*
 * MLFQ
 *
 * Time quantum of Q_i is calculated by quantum^i
 */
void MLFQ(int *ret, _process *pg, int quantum) {
        /*
         * Priority: Most highest queue is Q0.
         * Q0 > Q1 > Q2
         */
        int curTime = 0;
        queue<_process *> Q0;
        queue<_process *> Q1;
        queue<_process *> Q2;
        queue<_process *> *qList[] = {&Q0, &Q1, &Q2};
        int Q_timeQuantum[3];
        int cur_Q_index = 0;
        _process *sched;

        for (int i = 0; i < 3; i++) {
                Q_timeQuantum[i] = (int)pow(quantum, i);
        }

        while (curTime < MAX_TIME) {
                push_readyQ_v2(curTime, qList[0], pg);
                /*
                 * TODO:
                 *      Is it possible to optimize under WHILE loop?
                 */
                while (1) {
                        if (!(*qList[cur_Q_index]).empty() || cur_Q_index == 2)
                                break;
                        cur_Q_index++;
                }

                /*
                 * Execute all processes in the current queue.
                 *
                 * TODO:
                 *      Replace under logics into RoundRobin.
                 */
                // RoundRobin(ret, pg, quantum, curTime, qList[cur_Q_index],
                // &Q0);
                if (!(*qList[cur_Q_index]).empty()) {
                        sched = (*qList[cur_Q_index]).front();
                        (*qList[cur_Q_index]).pop();
                        for (int i = 0; i < Q_timeQuantum[cur_Q_index]; i++) {
                                sched->remain_time--;
                                ret[curTime] = sched->pid;
                                curTime++;
                                push_readyQ_v2(curTime, qList[0], pg);
                                if (sched->remain_time == 0) {
                                        sched->isDone = true;
                                        break;
                                }
                        }
                }

                /*
                 * TODO:
                 *      Is it possible to optimize 3-indented code?
                 */
                if (!sched->isDone) {
                        if (Q0.empty() && Q1.empty() && Q2.empty()) {
                                (*qList[cur_Q_index]).push(sched);
                        } else {
                                if (cur_Q_index != 2)
                                        cur_Q_index++;
                                (*qList[cur_Q_index]).push(sched);
                        }
                }

                /*
                 * Move to topmost queue.
                 */
                cur_Q_index = 0;
        }
}

/*
 * function for the greatest common denominator
 */
int gcd(int a, int b) {
        int c;
        while (b != 0) {
                c = a % b;
                a = b;
                b = c;
        }
        return a;
}

/*
 * function for the least common multiple
 */
int lcm(int a, int b) { return a * b / gcd(a, b); }

/*
 * Stride Scheduler
 *
 * @s_pass - Smallest Pass Value
 */
void Stride(int *ret, _process *pg) {
        int curTime = 0;
        int LCM = pg[0].ticket;
        int index, s_pass;

        /*
         * step to make proper stride
         */
        for (int i = 1; i < MAX_PROCESSES; i++) {
                LCM = lcm(LCM, pg[i].ticket);
        }
        for (int i = 0; i < MAX_PROCESSES; i++) {
                pg[i].stride = LCM / pg[i].ticket;
        }

        printf("stride : %d  %d  %d  %d  %d\n", pg[0].stride, pg[1].stride,
               pg[2].stride, pg[3].stride, pg[4].stride);

        while (curTime < MAX_TIME) {
                s_pass = 0;
                for (index = 1; index < MAX_PROCESSES; index++) {
                        if (pg[s_pass].pass_value > pg[index].pass_value) {
                                s_pass = index;
                        }
                }

                ret[curTime] = pg[s_pass].pid;
                curTime++;

                pg[s_pass].pass_value += pg[s_pass].stride;
        }
};