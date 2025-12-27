#include <stdio.h>

int main() {
    FILE *fp = fopen("vehicle.data", "r");
    if (!fp) {
        printf("vehicle.data not found\n");
        return 1;
    }

    printf("Current Vehicles in File:\n");
    int road, lane, id;
    while (fscanf(fp, "%d %d %d", &road, &lane, &id) == 3) {
        printf("Road %d, Lane %d -> Vehicle %d\n", road, lane, id);
    }

    fclose(fp);
    return 0;
}