#include <stdio.h> 
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;              // Process ID
    int arrival_time;     // Arrival time
    int burst_time;       // Burst time
    int priority;         // Priority
    int waiting_time;     // Waiting Time
    int turnaround_time;  // Turnaround Time
} Process;

// Function to sort processes by arrival time
void sortByArrival(Process *processes, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void firstComeFirstServed(Process *p, int n) {
    sortByArrival(p, n);
    int currentTime = 0;
    int startTime[n], endTime[n];

    for (int i = 0; i < n; i++) {
        if(currentTime < p[i].arrival_time) {
            currentTime = p[i].arrival_time;
        }
        startTime[i] = currentTime;
        p[i].waiting_time = currentTime - p[i].arrival_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
        currentTime += p[i].burst_time;
        endTime[i] = currentTime;
    }

    printf("\nFCFS Scheduling Results:\n");
    printf("PID\tArrival\tBurst\tPriority\tWT\tTAT\n");
    int total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].waiting_time, p[i].turnaround_time);
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);

    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = startTime[i]; j < endTime[i]; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        int executionTime = endTime[i] - startTime[i];
        printf(" P%d ", p[i].pid);
        for (int j = 0; j < executionTime - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = startTime[i]; j < endTime[i]; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n%d", startTime[0]);
    for (int i = 0; i < n; i++) {
        int middleSpacing = endTime[i] - startTime[i];
        for (int j = 0; j < middleSpacing; j++) {
            printf("  ");
        }
        printf("%d", endTime[i]);
    }
    printf("\n");
}

void priorityScheduling(Process *p, int n) {
    int currentTime = 0, completed = 0;
    int startTime[n], endTime[n], visited[n];
    for (int i = 0; i < n; i++) visited[i] = 0;

    while (completed < n) {
        int idx = -1;
        int highest_priority = 9999;
        for (int i = 0; i < n; i++) {
            if (!visited[i] && p[i].arrival_time <= currentTime && p[i].priority < highest_priority) {
                highest_priority = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) {
            currentTime++;
            continue;
        }
        startTime[completed] = currentTime;
        currentTime += p[idx].burst_time;
        endTime[completed] = currentTime;

        p[idx].waiting_time = startTime[completed] - p[idx].arrival_time;
        p[idx].turnaround_time = p[idx].waiting_time + p[idx].burst_time;

        Process temp = p[completed];
        p[completed] = p[idx];
        p[idx] = temp;
        visited[completed] = 1;
        completed++;
    }

    printf("\nPriority Scheduling Results:\n");
    printf("PID\tArrival\tBurst\tPriority\tWT\tTAT\n");
    int total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid, p[i].arrival_time, p[i].burst_time, p[i].priority,
               p[i].waiting_time, p[i].turnaround_time);
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);

    printf("\nGantt Chart:\n");
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = startTime[i]; j < endTime[i]; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");
    for (int i = 0; i < n; i++) {
        int executionTime = endTime[i] - startTime[i];
        printf(" P%d ", p[i].pid);
        for (int j = 0; j < executionTime - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    printf("\n ");
    for (int i = 0; i < n; i++) {
        for (int j = startTime[i]; j < endTime[i]; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n%d", startTime[0]);
    for (int i = 0; i < n; i++) {
        int middleSpacing = endTime[i] - startTime[i];
        for (int j = 0; j < middleSpacing; j++) {
            printf("  ");
        }
        printf("%d", endTime[i]);
    }
    printf("\n");
}

int main() {
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL) {
        perror("Failed to open file.");
        return EXIT_FAILURE;
    }

    Process *processes = malloc(sizeof(Process) * MAX_PROCESSES);
    int count = 0;
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (sscanf(buffer, "%d %d %d %d",
                   &processes[count].pid,
                   &processes[count].arrival_time,
                   &processes[count].burst_time,
                   &processes[count].priority) == 4) {
            processes[count].waiting_time = 0;
            processes[count].turnaround_time = 0;
            count++;
        }
    }
    fclose(file);

    int choice;
    printf("Select Scheduling Algorithm:\n");
    printf("1. First-Come, First-Served (FCFS)\n");
    printf("2. Priority Scheduling\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        firstComeFirstServed(processes, count);
    } else if (choice == 2) {
        priorityScheduling(processes, count);
    } else {
        printf("Invalid choice. Exiting.\n");
    }

    free(processes);
    return 0;
}
