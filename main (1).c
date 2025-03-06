#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

// Define a structure to hold process data
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
    Process processes [100];
    int read =0;
    int records=0;
    do{
        read = fscanf(file,
                        "%d %d %d %d\n,"
                        &processes[records].pid,
                        &processes[records].arrival_time,
                        &processes[records].burst_time,
                        &processes[records].priority);
        if (read == 4) records++;
        if (read!== 4 && !feof(file))
        {
            printf("File format incorrect.\n");
            return 1;
        }
        
        if (ferror(file))
        {
            printf("Error reading file.\n");
            return 1;
        }
    
    }while (!feof(file));
    
    
    fclose(file);
    printf("\n%d pid read.\n\n", records);
    for (int i = 0；i<records; i++)
        printf("%d %d %d %d\n",
                processes[i].pid,
                processes[i].arrival_time,
                processes[i].burst_time,
                processes[i].priority);
    printf("\n");

   
    

    return 0;
}