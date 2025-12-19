#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    FILE *fp;
    srand(time(NULL));

    while (1) {
        fp = fopen("vehicles.data", "a");
        fprintf(fp, "A %d\n", rand() % 3 + 1);
        fprintf(fp, "B %d\n", rand() % 3 + 1);
        fclose(fp);
        sleep(2);
    }
}
