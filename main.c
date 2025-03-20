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
// Used for First Come First Served
void sortByArrival(Process *processes, int n) {
        
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                // Swap processes
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

    // Array to store the execution start and end times for Gantt Chart
    int startTime[n], endTime[n];

    for (int i = 0; i < n; i++) {
        // check if cpu is idle, if so jump to arrival time
        if(currentTime < p[i].arrival_time) {
            currentTime = p[i].arrival_time;
        }

        // Store the start time for Gantt chart
        startTime[i] = currentTime;

        //calculate waiting time
        p[i].waiting_time = currentTime - p[i].arrival_time;

        //calculate turnaround time
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;

        //move current time forward by burst time
        currentTime += p[i].burst_time;

        // Store the end time for Gantt chart
        endTime[i] = currentTime;

    }

    // Display results
    printf("\nFCFS Scheduling Results:\n");
    printf("PID\tArrival\tBurst\tPriority\tWT\tTAT\n");

    int total_wt = 0, total_tat = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].priority,
               p[i].waiting_time,
               p[i].turnaround_time);

        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    // Display averages
    printf("\nAverage Waiting Time: %.2f\n", (float)total_wt / n);
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);

    // Gantt Chart Display
    printf("\nGantt Chart:\n");
    
    // Print the top border of the Gantt chart
    printf(" ");
    for (int i = 0; i < n; i++) {
        for (int j = startTime[i]; j < endTime[i]; j++) {
            printf("--");
        }
        printf(" ");
    }
    printf("\n|");

    // Print the process blocks
    for (int i = 0; i < n; i++) {
        int executionTime = endTime[i] - startTime[i];
        int middleSpacing = executionTime;  

        printf(" P%d ", p[i].pid);

        for (int j = 0; j < executionTime - 1; j++) {
            printf(" ");
        }
        printf("|");
    }
    
    printf("\n ");

    // Print the bottom border of the Gantt chart
    for (int i = 0; i < n; i++) {
        for (int j = startTime[i]; j < endTime[i]; j++) {
            printf("--");
        }
        printf(" ");
    }
    
    // Print the timeline
    printf("\n");

    printf("%d", startTime[0]);
    for (int i = 0; i < n; i++) {
        int middleSpacing = endTime[i] - startTime[i];
        for (int j = 0; j < middleSpacing; j++) {
            printf("  ");
        }
        printf("%d", endTime[i]);
    }

    printf("\n");


}

void priorityScheduling(Process *p[]) {

}

int main() {
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL) {
        perror("Failed to open file.");
        return EXIT_FAILURE;
    }

    // array for all processes in file
    Process *processes = malloc(sizeof(Process) * MAX_PROCESSES);

    int count = 0;
    char buffer[256];  // Buffer for skipping the header line

    // Skip the header line
    fgets(buffer, sizeof(buffer), file);
    
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        // Use sscanf to extract integers from the line
        if (sscanf(buffer, "%d %d %d %d",
                   &processes[count].pid,
                   &processes[count].arrival_time,
                   &processes[count].burst_time,
                   &processes[count].priority) == 4) {
            
            // Initialize WT and TAT to 0
            processes[count].waiting_time = 0;
            processes[count].turnaround_time = 0;

            count++;
        }
    }
    


    fclose(file);

    //printf("%d\n", processes[1].priority);
    
    firstComeFirstServed(processes, count);
    
    free(processes);
    return 0;
}

