#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int readVehicle(char road, int *count);
int readPriority(int *count);

#define WINDOW_W 800
#define WINDOW_H 800

int qA = 0, qB = 0, qC = 0, qD = 0, qP = 0;
char currentGreen = 'A';

/* Draw cross roads */
void drawRoads(SDL_Renderer *r) {
    SDL_SetRenderDrawColor(r, 60, 60, 60, 255);

    SDL_FRect vertical = {360, 0, 80, 800};
    SDL_FRect horizontal = {0, 360, 800, 80};

    SDL_RenderFillRect(r, &vertical);
    SDL_RenderFillRect(r, &horizontal);
}

/* Draw traffic light */
void drawLight(SDL_Renderer *r, char road, float x, float y) {
    if (currentGreen == road)
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);   // GREEN
    else
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);   // RED

    SDL_FRect light = {x, y, 20, 20};
    SDL_RenderFillRect(r, &light);
}

/* Draw vehicles as queued rectangles */
void drawVehicles(SDL_Renderer *r, int count, float x, float y, float dx, float dy) {
    SDL_SetRenderDrawColor(r, 0, 0, 255, 255);

    for (int i = 0; i < count && i < 10; i++) {
        SDL_FRect car = {x + dx * i, y + dy * i, 18, 28};
        SDL_RenderFillRect(r, &car);
    }
}

int main(void) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(
            "Traffic Light Simulator (SDL3)",
            WINDOW_W, WINDOW_H,
            0, &window, &renderer) < 0) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event e;
    int v;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                running = false;
        }

        /* Read generated vehicles */
        if (readVehicle('A', &v)) qA += v;
        if (readVehicle('B', &v)) qB += v;
        if (readVehicle('C', &v)) qC += v;
        if (readVehicle('D', &v)) qD += v;
        if (readPriority(&v)) qP += v;

        /* Priority logic */
        if (qP > 10) {
            currentGreen = 'A';   // Priority lane override
        } else {
            currentGreen =
                (currentGreen == 'A') ? 'B' :
                (currentGreen == 'B') ? 'C' :
                (currentGreen == 'C') ? 'D' : 'A';
        }

        /* Clear screen */
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        drawRoads(renderer);

        /* Traffic lights */
        drawLight(renderer, 'A', 390, 300);
        drawLight(renderer, 'B', 390, 480);
        drawLight(renderer, 'C', 300, 390);
        drawLight(renderer, 'D', 480, 390);

        /* Vehicles */
        drawVehicles(renderer, qA, 380, 100, 0, 30);
        drawVehicles(renderer, qB, 405, 600, 0, -30);
        drawVehicles(renderer, qC, 100, 380, 30, 0);
        drawVehicles(renderer, qD, 600, 405, -30, 0);

        SDL_RenderPresent(renderer);
        SDL_Delay(1000);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
