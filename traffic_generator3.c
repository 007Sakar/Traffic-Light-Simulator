#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int main() {
    srand(time(NULL));
    int vehicle_id = 1;

    printf("Generator started. Creating vehicles...\n");

    while (1) {
        FILE *fp = fopen("vehicle.data", "a");
        if (!fp) {
            printf("Error: Cannot open vehicle.data\n");
            return 1;
        }

        int road = rand() % 4;
        int lane = rand() % 3;

        fprintf(fp, "%d %d %d\n", road, lane, vehicle_id);
        fclose(fp);

        printf("Created: Vehicle %d on Road %d, Lane %d\n", vehicle_id, road, lane);
        vehicle_id++;

        Sleep(500); // 0.5 seconds
    }
    return 0;
}