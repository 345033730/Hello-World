#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

// Define a structure to hold process data
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int TAT;
    int start_time; 
    int end_time;   
} Process;

// Bubble sort for FCFS Sort processes by arrival time 
void compareAT(Process processes[], int n) {
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
// Bubble sort for SJF Sort processes by arrival time 
void compareBT(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Function to calculate waiting time, turnaround time, and end_time 
void calculateTimes(Process processes[], int n, int isFCFS) {
    int current_time = 0;

    if (isFCFS) {
        // Sort processes by arrival time for FCFS using Bubble Sort
        compareAT(processes, n);
    } else {
        // Sort processes by arrival time for SJF using Bubble Sort
        compareBT(processes, n);
    }

    // Initialize waiting time and turnaround time for each process
    for (int i = 0; i < n; i++) {
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        if (processes[i].waiting_time < 0) {
            processes[i].waiting_time = 0;//aviod negative waiting time
            current_time = processes[i].arrival_time;
        }
        processes[i].start_time = current_time;
        processes[i].end_time = current_time + processes[i].burst_time;
        processes[i].TAT = processes[i].end_time - processes[i].arrival_time;
        current_time = processes[i].end_time;
    }
}

// Print Gantt Chart
void printGanttChart(Process processes[], int n) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; i++) {
        printf("| P%d ", processes[i].pid);
    }
    printf("|\n");

    // Print timeline
    for (int i = 0; i < n; i++) {
        printf("%d    ", processes[i].start_time);
    }
    printf("%d\n", processes[n - 1].end_time);
}

void printResults(Process processes[], int n) {
    float average_waiting = 0, average_TAT = 0;

    // Print process details
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].burst_time,
               processes[i].waiting_time,
               processes[i].TAT);
        average_waiting += processes[i].waiting_time;
        average_TAT += processes[i].TAT;
    }

    // Calculate and print averages waiting and turnaround time
    average_waiting /= n;
    average_TAT /= n;
    printf("\nAverage Waiting Time: %.2f\n", average_waiting);
    printf("Average Turnaround Time: %.2f\n", average_TAT);
}

int main(void) {
    FILE *file;
    file = fopen("processes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Process processes[MAX_PROCESSES];
    int read = 0;
    int records = 0;

    // Skip the header line
    char header[100];
    if (fgets(header, sizeof(header), file) == NULL) {
        printf("Error reading header line.\n");
        fclose(file);
        return 1;
    }

    // Read the file line by line
    do {
        read = fscanf(file, "%d %d %d %d",
                      &processes[records].pid,
                      &processes[records].arrival_time,
                      &processes[records].burst_time,
                      &processes[records].priority);

        if (read == 4) {
            records++;
        } else if (read != 4 && !feof(file)) {
            printf("File format incorrect.\n");
            fclose(file);
            return 1;
        }

        if (ferror(file)) {
            printf("Error reading file.\n");
            fclose(file);
            return 1;
        }

        // Check if the maximum number of processes is reached
        if (records >= MAX_PROCESSES) {
            printf("Maximum number of processes reached.\n");
            break;
        }
    } while (!feof(file));

    fclose(file);

    // Print the number of processes read
    printf("\n%d processes read.\n\n", records);

    // FCFS Scheduling
    printf("FCFS Scheduling:\n");
    calculateTimes(processes, records, 0); // 0 for FCFS
    printGanttChart(processes, records);
    printResults(processes, records);

    // SJF Scheduling
    printf("\nSJF Scheduling:\n");
    calculateTimes(processes, records, 1); // 1 for SJF
    printGanttChart(processes, records);
    printResults(processes, records);

    return 0;
}