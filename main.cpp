#include <iostream>
#include <cstdint>
#include <vector>
#include <SDL3/SDL.h>

int main(int argc, char* argv[]) {
    SDL_Window *window;                    // Declare a pointer
    SDL_Renderer *renderer;
    SDL_Texture *framebufferTexture;
    constexpr int width = 670;
    constexpr int height = 500;
    bool done = false;
    
     window = SDL_CreateWindow(
        "An SDL3 window",                  // window title
        width,                               // width, in pixels
        height,                               // height, in pixels
        0       // flags - see below
    );
    renderer = SDL_CreateRenderer(window, NULL);
    framebufferTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);              // Initialize SDL3

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    SDL_Log("Gagal Init SDL: %s", SDL_GetError());
    return 1;
    }

    // Create an application window with the following settings:
   

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    std::vector<std::uint32_t> pixels(width * height);

    while (!done) {
        SDL_Event event;

        const bool* keys = SDL_GetKeyboardState(nullptr);
        int babayaga = 1;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                if(event.key.key != 'b'){
                SDL_Log("Key pressed: %d",event.key.key);
                }
                if(event.key.key == 'a'){
                SDL_Log("NIGGA");
                }
            } 
            if(keys[SDL_SCANCODE_B]){
                babayaga = 0;
                SDL_Log("Key pss: %d",event.key.key);
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const std::uint32_t red = ((x * 255) / width)*babayaga;
                const std::uint32_t green = ((y * 255) / height)*babayaga;
                const std::uint32_t blue = (((x + y) * 255) / (width + height))*babayaga;
                pixels[y * width + x] =
                    0xFF000000u | (red << 16) | (green << 8) | blue;
            }
        }      
        SDL_UpdateTexture(framebufferTexture, NULL, pixels.data(), width * sizeof(std::uint32_t));
        SDL_RenderClear(renderer);
       SDL_RenderTexture(renderer, framebufferTexture, NULL, NULL); // Tempel texture ke layar
        SDL_RenderPresent(renderer); // Munculkan!
    }

    // Close and destroy the window
    SDL_DestroyTexture(framebufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();
    return 0;
}