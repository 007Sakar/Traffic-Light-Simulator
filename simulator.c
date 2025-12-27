#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define CENTER_SIZE 100
#define ROAD_WIDTH 150
#define LANE_WIDTH 50
#define VEHICLE_SIZE 40
#define MAX_VEHICLES 200
#define TRAFFIC_LIGHT_SIZE 15
#define PRIORITY_THRESHOLD 10

typedef struct {
    int road;
    int lane;
    int id;
    float x, y;
    bool active;
    bool waiting;
} Vehicle;

typedef struct {
    bool green[4]; // One for each road
    int vehicle_count[4][3]; // Count per road and lane
} TrafficLight;

Vehicle vehicles[MAX_VEHICLES];
int vehicle_count = 0;
TrafficLight traffic_light;
int last_processed_id = 0;

void init_traffic_light() {
    for (int i = 0; i < 4; i++) {
        traffic_light.green[i] = false;
        for (int j = 0; j < 3; j++) {
            traffic_light.vehicle_count[i][j] = 0;
        }
    }
}

void count_vehicles_per_lane() {
    // Reset counts
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            traffic_light.vehicle_count[i][j] = 0;
        }
    }

    // Count waiting vehicles
    for (int i = 0; i < vehicle_count; i++) {
        if (vehicles[i].active && vehicles[i].waiting) {
            traffic_light.vehicle_count[vehicles[i].road][vehicles[i].lane]++;
        }
    }
}

void update_traffic_lights() {
    count_vehicles_per_lane();

    // Reset all lights
    for (int i = 0; i < 4; i++) {
        traffic_light.green[i] = false;
    }

    // Check lane 2 (index 2) priority for each road
    int priority_road = -1;
    int max_lane2_count = 0;

    for (int road = 0; road < 4; road++) {
        if (traffic_light.vehicle_count[road][2] >= PRIORITY_THRESHOLD) {
            if (traffic_light.vehicle_count[road][2] > max_lane2_count) {
                max_lane2_count = traffic_light.vehicle_count[road][2];
                priority_road = road;
            }
        }
    }

    // If a road has priority lane threshold, give it green
    if (priority_road >= 0) {
        traffic_light.green[priority_road] = true;
    } else {
        // Otherwise, find road with most waiting vehicles
        int max_total = 0;
        int busiest_road = -1;

        for (int road = 0; road < 4; road++) {
            int total = 0;
            for (int lane = 0; lane < 3; lane++) {
                total += traffic_light.vehicle_count[road][lane];
            }
            if (total > max_total) {
                max_total = total;
                busiest_road = road;
            }
        }

        if (busiest_road >= 0) {
            traffic_light.green[busiest_road] = true;
        }
    }
}

void load_vehicles() {
    FILE *fp = fopen("vehicle.data", "r");
    if (!fp) return;

    int road, lane, id;
    while (fscanf(fp, "%d %d %d", &road, &lane, &id) == 3) {
        // Only process new vehicles
        if (id <= last_processed_id) continue;
        
        // Check if vehicle already exists
        bool exists = false;
        for (int i = 0; i < vehicle_count; i++) {
            if (vehicles[i].id == id) {
                exists = true;
                break;
            }
        }

        if (!exists && vehicle_count < MAX_VEHICLES) {
            vehicles[vehicle_count].road = road;
            vehicles[vehicle_count].lane = lane;
            vehicles[vehicle_count].id = id;
            vehicles[vehicle_count].active = true;
            vehicles[vehicle_count].waiting = false;

            // Set initial position based on road and lane
            float center_x = WINDOW_WIDTH / 2;
            float center_y = WINDOW_HEIGHT / 2;

            switch (road) {
                case 0: // North (top)
                    vehicles[vehicle_count].x = center_x - ROAD_WIDTH/2 + LANE_WIDTH/2 + lane * LANE_WIDTH;
                    vehicles[vehicle_count].y = 0;
                    break;
                case 1: // East (right)
                    vehicles[vehicle_count].x = WINDOW_WIDTH;
                    vehicles[vehicle_count].y = center_y - ROAD_WIDTH/2 + LANE_WIDTH/2 + lane * LANE_WIDTH;
                    break;
                case 2: // South (bottom)
                    vehicles[vehicle_count].x = center_x + ROAD_WIDTH/2 - LANE_WIDTH/2 - lane * LANE_WIDTH;
                    vehicles[vehicle_count].y = WINDOW_HEIGHT;
                    break;
                case 3: // West (left)
                    vehicles[vehicle_count].x = 0;
                    vehicles[vehicle_count].y = center_y + ROAD_WIDTH/2 - LANE_WIDTH/2 - lane * LANE_WIDTH;
                    break;
            }
            
            last_processed_id = id;
            vehicle_count++;
        }
    }
    fclose(fp);

    // Remove inactive vehicles to free up space
    int write_index = 0;
    for (int read_index = 0; read_index < vehicle_count; read_index++) {
        if (vehicles[read_index].active) {
            if (write_index != read_index) {
                vehicles[write_index] = vehicles[read_index];
            }
            write_index++;
        }
    }
    vehicle_count = write_index;
}

void update_vehicles(float delta_time) {
    float speed = 100.0f * delta_time;
    float center_x = WINDOW_WIDTH / 2;
    float center_y = WINDOW_HEIGHT / 2;
    float stop_distance = 180.0f; // Distance from center to stop

    for (int i = 0; i < vehicle_count; i++) {
        if (!vehicles[i].active) continue;

        float dx = vehicles[i].x - center_x;
        float dy = vehicles[i].y - center_y;
        float distance_to_center = sqrtf(dx * dx + dy * dy);

        // Check if should stop at traffic light
        bool should_stop = !traffic_light.green[vehicles[i].road] && distance_to_center > stop_distance;

        if (should_stop) {
            vehicles[i].waiting = true;
            continue;
        } else {
            vehicles[i].waiting = false;
        }

        switch (vehicles[i].road) {
            case 0: // North - move down
                vehicles[i].y += speed;
                if (vehicles[i].y > center_y - CENTER_SIZE/2) vehicles[i].active = false;
                break;
            case 1: // East - move left
                vehicles[i].x -= speed;
                if (vehicles[i].x < center_x + CENTER_SIZE/2) vehicles[i].active = false;
                break;
            case 2: // South - move up
                vehicles[i].y -= speed;
                if (vehicles[i].y < center_y + CENTER_SIZE/2) vehicles[i].active = false;
                break;
            case 3: // West - move right
                vehicles[i].x += speed;
                if (vehicles[i].x > center_x - CENTER_SIZE/2) vehicles[i].active = false;
                break;
        }
    }
}

void draw_roads(SDL_Renderer *renderer) {
    float center_x = WINDOW_WIDTH / 2;
    float center_y = WINDOW_HEIGHT / 2;

    // Draw roads (gray)
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);

    // North-South road
    SDL_FRect ns_road = {center_x - ROAD_WIDTH/2, 0, ROAD_WIDTH, WINDOW_HEIGHT};
    SDL_RenderFillRect(renderer, &ns_road);

    // East-West road
    SDL_FRect ew_road = {0, center_y - ROAD_WIDTH/2, WINDOW_WIDTH, ROAD_WIDTH};
    SDL_RenderFillRect(renderer, &ew_road);

    // Draw lane markings (white dashed lines)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // North-South lanes
    for (int i = 1; i < 3; i++) {
        float x = center_x - ROAD_WIDTH/2 + i * LANE_WIDTH;
        for (int y = 0; y < WINDOW_HEIGHT; y += 20) {
            SDL_FRect dash = {x - 1, y, 2, 10};
            SDL_RenderFillRect(renderer, &dash);
        }
    }

    // East-West lanes
    for (int i = 1; i < 3; i++) {
        float y = center_y - ROAD_WIDTH/2 + i * LANE_WIDTH;
        for (int x = 0; x < WINDOW_WIDTH; x += 20) {
            SDL_FRect dash = {x, y - 1, 10, 2};
            SDL_RenderFillRect(renderer, &dash);
        }
    }

    // Draw center intersection (darker)
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_FRect center = {center_x - CENTER_SIZE/2, center_y - CENTER_SIZE/2, CENTER_SIZE, CENTER_SIZE};
    SDL_RenderFillRect(renderer, &center);
}

void draw_traffic_lights(SDL_Renderer *renderer) {
    float center_x = WINDOW_WIDTH / 2;
    float center_y = WINDOW_HEIGHT / 2;
    float light_distance = 200.0f;

    // Draw traffic lights for each road
    for (int road = 0; road < 4; road++) {
        float x, y;
        
        switch (road) {
            case 0: // North
                x = center_x + ROAD_WIDTH/2 + 10;
                y = center_y - light_distance;
                break;
            case 1: // East
                x = center_x + light_distance;
                y = center_y + ROAD_WIDTH/2 + 10;
                break;
            case 2: // South
                x = center_x - ROAD_WIDTH/2 - 10 - TRAFFIC_LIGHT_SIZE;
                y = center_y + light_distance;
                break;
            case 3: // West
                x = center_x - light_distance;
                y = center_y - ROAD_WIDTH/2 - 10 - TRAFFIC_LIGHT_SIZE;
                break;
        }

        // Draw light background (black)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_FRect light_bg = {x, y, TRAFFIC_LIGHT_SIZE, TRAFFIC_LIGHT_SIZE * 2 + 5};
        SDL_RenderFillRect(renderer, &light_bg);

        // Draw red or green light
        if (traffic_light.green[road]) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
            SDL_FRect green_light = {x + 2, y + TRAFFIC_LIGHT_SIZE + 3, TRAFFIC_LIGHT_SIZE - 4, TRAFFIC_LIGHT_SIZE - 4};
            SDL_RenderFillRect(renderer, &green_light);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
            SDL_FRect red_light = {x + 2, y + 2, TRAFFIC_LIGHT_SIZE - 4, TRAFFIC_LIGHT_SIZE - 4};
            SDL_RenderFillRect(renderer, &red_light);
        }
    }
}

void draw_vehicles(SDL_Renderer *renderer) {
    for (int i = 0; i < vehicle_count; i++) {
        if (!vehicles[i].active) continue;

        // Color based on lane (lane 2 is special - blue)
        if (vehicles[i].lane == 2) {
            SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Blue for lane 2
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255); // Red for other lanes
        }

        SDL_FRect vehicle_rect = {
            vehicles[i].x - VEHICLE_SIZE/2,
            vehicles[i].y - VEHICLE_SIZE/2,
            VEHICLE_SIZE,
            VEHICLE_SIZE
        };
        SDL_RenderFillRect(renderer, &vehicle_rect);

        // Draw vehicle border
        if (vehicles[i].lane == 2) {
            SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
        }
        SDL_RenderRect(renderer, &vehicle_rect);
    }
}

void draw_info(SDL_Renderer *renderer) {
    float bar_x = 10;
    float bar_y = 10;
    float bar_width = 150;
    float bar_height = 20;

    // Road labels (N, E, S, W)
    const char *labels[] = {"North", "East", "South", "West"};

    for (int road = 0; road < 4; road++) {
        // Draw background
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 200);
        SDL_FRect bg = {bar_x, bar_y + road * 25, bar_width, bar_height};
        SDL_RenderFillRect(renderer, &bg);

        // Draw lane 2 vehicle count as a bar
        int count = traffic_light.vehicle_count[road][2];
        float fill_width = (count / (float)PRIORITY_THRESHOLD) * bar_width;
        if (fill_width > bar_width) fill_width = bar_width;

        if (count >= PRIORITY_THRESHOLD) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green when threshold reached
        } else {
            SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Blue otherwise
        }
        
        SDL_FRect fill = {bar_x, bar_y + road * 25, fill_width, bar_height};
        SDL_RenderFillRect(renderer, &fill);

        // Draw border
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &bg);
    }
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Traffic Simulation - Lane 2 Priority", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    init_traffic_light();

    bool running = true;
    SDL_Event event;
    Uint64 last_time = SDL_GetTicks();
    Uint64 last_load_time = SDL_GetTicks();
    Uint64 last_light_update = SDL_GetTicks();

    printf("Traffic Simulator Started\n");
    printf("Lane 2 Priority Threshold: %d vehicles\n", PRIORITY_THRESHOLD);
    printf("Blue vehicles = Lane 2 (priority lane)\n");
    printf("Red vehicles = Lanes 0 and 1\n\n");

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        Uint64 current_time = SDL_GetTicks();
        float delta_time = (current_time - last_time) / 1000.0f;
        last_time = current_time;

        // Load vehicles from file every 0.5 seconds
        if (current_time - last_load_time > 500) {
            load_vehicles();
            last_load_time = current_time;
        }

        // Update traffic lights every 2 seconds
        if (current_time - last_light_update > 2000) {
            update_traffic_lights();
            last_light_update = current_time;
        }

        update_vehicles(delta_time);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255); // Green background
        SDL_RenderClear(renderer);

        draw_roads(renderer);
        draw_traffic_lights(renderer);
        draw_vehicles(renderer);
        draw_info(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}