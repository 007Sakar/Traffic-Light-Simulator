#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>

/* ---------------- CONSTANTS ---------------- */
#define ROADS 4
#define LANES 3
#define MAX_QUEUE 100
#define WINDOW_W 800
#define WINDOW_H 800
#define VEHICLE_W 18
#define VEHICLE_H 10
#define LIGHT_W 20
#define LIGHT_H 20
#define GREEN_DURATION 5000
#define RED_DURATION 5000

/* ---------------- DATA STRUCTURES ---------------- */
typedef struct {
    int id;
} Vehicle;

typedef struct {
    Vehicle items[MAX_QUEUE];
    int front, rear, count;
} Queue;

typedef struct {
    int green;      // 1=green, 0=red
    Uint32 timer;   // last switch timestamp
    int forced;     // 1 if green forced by priority
} TrafficLight;

/* ---------------- GLOBALS ---------------- */
Queue junction[ROADS][LANES];
TrafficLight lights[ROADS];
int running = 1;

/* ---------------- QUEUE FUNCTIONS ---------------- */
void initQueue(Queue *q) { q->front = q->rear = q->count = 0; }
int isEmpty(Queue *q) { return q->count == 0; }
int isFull(Queue *q) { return q->count == MAX_QUEUE; }
void enqueue(Queue *q, Vehicle v) {
    if (isFull(q)) return;
    q->items[q->rear] = v;
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->count++;
}
Vehicle dequeue(Queue *q) {
    Vehicle v = {-1};
    if (isEmpty(q)) return v;
    v = q->items[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->count--;
    return v;
}

/* ---------------- FILE I/O ---------------- */
void loadVehiclesFromFile() {
    FILE *fp = fopen("vehicle.data", "r");
    if (!fp) return;

    int r, l, id;
    while (fscanf(fp, "%d %d %d", &r, &l, &id) == 3) {
        if (r < ROADS && l < LANES) {
            Vehicle v = {id};
            enqueue(&junction[r][l], v);
        }
    }
    fclose(fp);
}

void saveVehiclesToFile() {
    FILE *fp = fopen("vehicle.data", "w");
    if (!fp) return;

    for (int r = 0; r < ROADS; r++)
        for (int l = 0; l < LANES; l++) {
            Queue *q = &junction[r][l];
            int cnt = q->count;
            int idx = q->front;
            for (int i = 0; i < cnt; i++) {
                fprintf(fp, "%d %d %d\n", r, l, q->items[idx].id);
                idx = (idx + 1) % MAX_QUEUE;
            }
        }
    fclose(fp);
}

/* ---------------- TRAFFIC CONTROL ---------------- */
void serveTraffic() {
    // Priority lane handling
    for (int r = 0; r < ROADS; r++) {
        Queue *priorityLane = &junction[r][1]; // lane-2
        if (priorityLane->count > 10 && lights[r].green) {
            while (priorityLane->count > 5 && lights[r].green) {
                dequeue(priorityLane);
            }
            return;
        }
    }

    // Normal round-robin service
    for (int r = 0; r < ROADS; r++) {
        for (int l = 0; l < LANES; l++) {
            if (!isEmpty(&junction[r][l]) && lights[r].green) {
                dequeue(&junction[r][l]);
                return;
            }
        }
    }
}

/* ---------------- SDL DRAW ---------------- */
void drawRoads(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_FRect vertical = {350, 0, 100, 800};
    SDL_FRect horizontal = {0, 350, 800, 100};
    SDL_RenderFillRect(renderer, &vertical);
    SDL_RenderFillRect(renderer, &horizontal);
}

void drawVehicles(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int r = 0; r < ROADS; r++) {
        for (int l = 0; l < LANES; l++) {
            Queue *q = &junction[r][l];
            for (int i = 0; i < q->count && i < 10; i++) {
                SDL_FRect rect;
                if (r == 0) { rect.x = 380 + l * 20; rect.y = 100 - i*15; }
                else if (r == 1) { rect.x = 700 + i*15; rect.y = 380 + l*20; }
                else if (r == 2) { rect.x = 380 + l * 20; rect.y = 700 + i*15; }
                else { rect.x = 100 - i*15; rect.y = 380 + l*20; }
                rect.w = VEHICLE_W;
                rect.h = VEHICLE_H;
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

void drawTrafficLights(SDL_Renderer *renderer) {
    for (int r = 0; r < ROADS; r++) {
        SDL_FRect rect;
        rect.w = LIGHT_W;
        rect.h = LIGHT_H;
        if (r == 0) { rect.x = 390; rect.y = 320; }
        else if (r == 1) { rect.x = 480; rect.y = 390; }
        else if (r == 2) { rect.x = 390; rect.y = 460; }
        else { rect.x = 300; rect.y = 390; }

        if (lights[r].green) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_RenderFillRect(renderer, &rect);
    }
}

/* ---------------- LIGHT LOGIC ---------------- */
void updateTrafficLights() {
    Uint32 now = SDL_GetTicks();
    for (int r = 0; r < ROADS; r++) {
        Queue *priorityLane = &junction[r][1];

        // Adaptive priority: force green if lane-2 >10 vehicles
        if (priorityLane->count > 10) {
            lights[r].green = 1;
            lights[r].forced = 1;
            lights[r].timer = now; // reset timer
        } else if (lights[r].forced && priorityLane->count <= 5) {
            lights[r].forced = 0;
            lights[r].green = 0; // return to normal cycle
            lights[r].timer = now;
        }

        // Normal timed cycle only if not forced
        if (!lights[r].forced) {
            Uint32 duration = lights[r].green ? GREEN_DURATION : RED_DURATION;
            if (now - lights[r].timer > duration) {
                lights[r].green = !lights[r].green;
                lights[r].timer = now;
            }
        }
    }
}

/* ---------------- MAIN ---------------- */
int main(int argc, char *argv[]) {
    srand(time(NULL));

    // Initialize queues
    for (int r = 0; r < ROADS; r++) {
        for (int l = 0; l < LANES; l++) {
            initQueue(&junction[r][l]);
        }
    }

    loadVehiclesFromFile();

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Traffic Junction Simulator",
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize traffic lights AFTER SDL_Init
    for (int r = 0; r < ROADS; r++) {
        lights[r].green = (r % 2 == 0); // NS green first
        lights[r].timer = SDL_GetTicks();
        lights[r].forced = 0;
    }

    Uint32 lastServe = SDL_GetTicks();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) running = 0;
        }

        updateTrafficLights();

        if (SDL_GetTicks() - lastServe > 1000) {
            serveTraffic();
            lastServe = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        drawRoads(renderer);
        drawVehicles(renderer);
        drawTrafficLights(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    saveVehiclesToFile();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}