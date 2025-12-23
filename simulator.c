#include <stdio.h>
#include <unistd.h>

int readVehicle(char *, int *);
int readPriority(int *);

int qA=0, qB=0, qC=0, qD=0, qP=0;

void serve(char road, int *q) {