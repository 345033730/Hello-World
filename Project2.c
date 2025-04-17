#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
} Process;


void* executeProcess(void* arg) {
    Process* p = (Process*)arg;
    printf("Process %d started (Burst Time: %d)\n", p->pid, p->burst_time);
    sleep(p->burst_time); 
    printf("Process %d finished\n", p->pid);
    return NULL;
}

int main(void) {
    FILE *file = fopen("processes.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Process processes[MAX_PROCESSES];
    int records = 0;
    char header[100];
    if (fgets(header, sizeof(header), file) == NULL) {
        printf("Error reading header line.\n");
        fclose(file);
        return 1;
    }

    // Skip header and read processes
    fgets(header, sizeof(header), file);
    while (fscanf(file, "%d %d %d %d", 
                 &processes[records].pid, 
                 &processes[records].arrival_time, 
                 &processes[records].burst_time, 
                 &processes[records].priority) == 4) {
        records++;
        if (records >= MAX_PROCESSES) break;
    }while (!feof(file));

    fclose(file);

    printf("%d processes read.\n", records);

    pthread_t threads[MAX_PROCESSES];
    for (int i = 0; i < records; i++) {
        pthread_create(&threads[i], NULL, executeProcess, &processes[i]);
    }

    for (int i = 0; i < records; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}