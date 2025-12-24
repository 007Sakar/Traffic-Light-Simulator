#include <stdio.h>
#include <unistd.h>

int readVehicle(char *, int *);
int readPriority(int *);

int qA=0, qB=0, qC=0, qD=0, qP=0;

void serve(char road, int *q) {
    void serveRoad(char name, Queue* q) {
    printf("\nGREEN LIGHT → Road %c\n", name);
    int v = dequeue(q);
    if (v > 0) {
        printf("Serving %d vehicles (Time = %d sec)\n", v, v * T);
        sleep(v * T);
    }
}