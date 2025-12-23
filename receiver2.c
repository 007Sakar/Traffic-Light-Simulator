#include <stdio.h>

int readPriority(int *count) {
    FILE *fp = fopen("vehicles.data", "r");
    if (!fp) return 0;

    int found = 0;
    char r;
    int c;

    FILE *temp = fopen("temp.data", "w");

    while (fscanf(fp, "%c %d\n", &r, &c) != EOF) {
        if (r == 'P' && !found) {
            *count = c;
            found = 1;
        } else {
            fprintf(temp, "%c %d\n", r, c);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("vehicles.data");
    rename("temp.data", "vehicles.data");

    return found;
}
