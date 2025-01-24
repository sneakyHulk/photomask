#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define ROWS 100
#define COLS 50
#define RADIUS 2
#define HORIZONTAL_SPACING 10
#define VERTICAL_SPACING 10
#define HORIZONTAL_SPACING_GRADIENT 1.03
#define VERTICAL_SPACING_GRADIENT 0.98
#define RADIUS_GRADIENT 0.99

int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    x += WINDOW_WIDTH / 2;
    y += HORIZONTAL_SPACING + 2 * RADIUS;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}


int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    x += WINDOW_WIDTH / 2;
    y += HORIZONTAL_SPACING + 2 * RADIUS;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

// Function to initialize SDL
int initSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    *window = SDL_CreateWindow("50x50 Grid of Points", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return -1;
    }

    return 0;
}

// Function to draw a grid of points
void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    double x_center = 0;
    double y_center = 0;
    double x_offset = 0;

    for (int row = 0; row < ROWS; ++row) {
        if (row % 2) x_offset = 0;
        else
            x_offset = HORIZONTAL_SPACING * pow(HORIZONTAL_SPACING_GRADIENT, row) / 2 + RADIUS;

        x_center = x_offset;
        for (int col = 0; col < COLS / 2; ++col) {
            SDL_RenderFillCircle(renderer, x_center, y_center, RADIUS * pow(RADIUS_GRADIENT, row));

            x_center += HORIZONTAL_SPACING * pow(HORIZONTAL_SPACING_GRADIENT, row) + 2 * RADIUS;
        }

        x_center = x_offset;
        for (int col = 0; col < COLS / 2; ++col) {
            SDL_RenderFillCircle(renderer, x_center, y_center, RADIUS * pow(RADIUS_GRADIENT, row));

            x_center -= HORIZONTAL_SPACING * pow(HORIZONTAL_SPACING_GRADIENT, row) + 2 * RADIUS;
        }

        y_center += VERTICAL_SPACING * pow(VERTICAL_SPACING_GRADIENT, row) + 2 * RADIUS;
    }
}

// Main function
int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Initialize SDL and create window & renderer
    if (initSDL(&window, &renderer) != 0) {
        return -1;
    }

    // Main loop flag and event handler
    int quit = 0;
    SDL_Event e;

    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the screen to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the grid of points
        drawGrid(renderer);

        // Present the rendered content
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}