#include "include/lab1_sched_types.h"
#include <bits/stdc++.h>
using namespace std;

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
                printf(" %02d", i);
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

void reset_job_state(_process *&pg) {
        for (int p = 0; p < MAX_PROCESSES; p++) {
                pg[p].isDone = false;
                pg[p].remain_time = pg[p].burst_time;
        }
}

/*
 * FIFO(FCFS)
 *
 * It's very hard to handle pointer of poninter, with appropiate memory
 * reference. OTL
 */
void FIFO(int *&ret, _process *&pg) {
        /*
         * @ret - Indicates which process is running at that time.
         * E.g. ret[0] = 1,
         * means process '1' is running at the '0' momment of timeflow.
         *
         * @pg - (processGroup) Array of _process.
         *
         * @curTime - Indicates timeflow, 0 to MAX_TIME
         *
         * @Q - scheduling queue.
         */
        int curTime = 0;
        queue<_process *> Q;

        while (curTime < MAX_TIME) {
                /*
                 * Find a process which has not been completed.
                 * Push them to scheduling queue.
                 */
                for (int p = 0; p < MAX_PROCESSES; p++) {
                        _process *cp = &pg[p];
                        if ((!cp->isDone) && (cp->arrival_time <= curTime))
                                Q.push(cp);
                }

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
         * @Q - scheduling queue.
         * @isStarted - To avoid duplicate scheduling, put true into isStarted[]
         *              when certain process is started. Index means process ID.
         */
        int curTime = 0;
        queue<_process *> Q;
        vector<bool> isStarted(MAX_PROCESSES, false);

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
                                if (sched->remain_time == 0) {
                                        sched->isDone = true;
                                        break;
                                }
                        }
                }

                /*
                 * Find a process which already arrived.
                 * Push them to scheduling queue.
                 */
                for (int j = 0; j < MAX_PROCESSES; j++) {
                        _process *tmp = &pg[j];
                        if ((!tmp->isDone) && (tmp->arrival_time <= curTime) &&
                            (!isStarted[tmp->pid])) {
                                Q.push(tmp);
                                isStarted[tmp->pid] = true;
                        }
                }

                /*
                 * If process has some task remaining, push to queue again.
                 */
                if (sched != NULL)
                        if (!sched->isDone)
                                Q.push(sched);
        }
}