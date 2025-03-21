#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turnaround_time;
    int completed;
} Process;

Process processes[MAX];
int n = 0;

void read_processes(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char line[100];
    fgets(line, sizeof(line), file); // Skip header

    while (fscanf(file, "%d %d %d %d", 
                  &processes[n].pid, 
                  &processes[n].arrival_time, 
                  &processes[n].burst_time, 
                  &processes[n].priority) != EOF) {
        processes[n].completed = 0;
        n++;
    }
    fclose(file);
}

void priority_scheduling() {
    int time = 0, completed_count = 0;
    int gantt[MAX][3]; // pid, start, end
    int gc_index = 0;

    while (completed_count < n) {
        int idx = -1;
        int min_priority = 9999;

        for (int i = 0; i < n; i++) {
            if (!processes[i].completed && 
                processes[i].arrival_time <= time && 
                processes[i].priority < min_priority) {
                min_priority = processes[i].priority;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        Process *p = &processes[idx];
        p->waiting_time = time - p->arrival_time;
        p->turnaround_time = p->waiting_time + p->burst_time;
        p->completed = 1;

        gantt[gc_index][0] = p->pid;
        gantt[gc_index][1] = time;
        gantt[gc_index][2] = time + p->burst_time;
        gc_index++;

        time += p->burst_time;
        completed_count++;
    }

    printf("\nGantt Chart:\n");
    for (int i = 0; i < gc_index; i++) {
        printf("| P%d ", gantt[i][0]);
    }
    printf("|\n0");
    for (int i = 0; i < gc_index; i++) {
        printf(" %d", gantt[i][2]);
    }
    printf("\n");

    float total_wt = 0, total_tat = 0;
    printf("\nProcess Details:\nPID\tWT\tTAT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", 
               processes[i].pid, 
               processes[i].waiting_time, 
               processes[i].turnaround_time);
        total_wt += processes[i].waiting_time;
        total_tat += processes[i].turnaround_time;
    }

    printf("\nAverage WT: %.2f\n", total_wt / n);
    printf("Average TAT: %.2f\n", total_tat / n);
}

int main() {
    read_processes("processes.txt");
    priority_scheduling();
    return 0;
}
