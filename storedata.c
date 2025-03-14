#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

// Define a structure to hold data
typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
} Process;

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

    printf("\n%d processes read.\n\n", records);

    // Print the data
    for (int i = 0; i < records; i++) {
        printf("PID: %d, Arrival Time: %d, Burst Time: %d, Priority: %d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority);
    }
    printf("\n");

    return 0;
}