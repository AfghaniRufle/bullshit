#include <iostream>
#include <cstdint>
#include <vector>
#include <sdl3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[]) {
    SDL_Window *window;                    // Declare a pointer
    // Renderer bertugas mengirim gambar ke window melalui backend SDL.
    SDL_Renderer *renderer;
    // Texture menerima data piksel dari CPU; pointer ini perlu dibuat dengan SDL_CreateTexture sebelum dipakai.
    SDL_Texture *framebufferTexture;
    // Ukuran gambar yang akan kita susun sendiri, satu nilai warna untuk setiap piksel.
    constexpr int width = 670;
    constexpr int height = 500;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);              // Initialize SDL3

    if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Gagal Init SDL: %s", SDL_GetError());
    return 1;
    } else if (!SDL_Init(SDL_INIT_AUDIO)) {
    SDL_Log("Gagal Init SDL: %s", SDL_GetError());
    return 1;
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL3 window",                  // window title
        670,                               // width, in pixels
        500,                               // height, in pixels
        SDL_WINDOW_FULLSCREEN         // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    std::vector<std::uint32_t> pixels(width * height);

    while (!done) {
        SDL_Event event;

        // Periksa event yang masuk, misalnya klik tombol X untuk menutup window.
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }

        // Isi framebuffer: loop luar memilih baris (y), loop dalam memilih kolom (x).
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Hitung tiap kanal warna berdasarkan posisi piksel agar terbentuk gradasi.
                const std::uint32_t red = (x * 255) / width;
                const std::uint32_t green = (y * 255) / height;
                const std::uint32_t blue = ((x + y) * 255) / (width + height);
                // Ubah koordinat (x, y) menjadi indeks 1D, lalu susun warna dalam format ARGB.
                pixels[y * width + x] =
                    0xFF000000u | (red << 16) | (green << 8) | blue;
            }
        }

        // Salin seluruh piksel CPU ke texture SDL; pitch adalah jumlah byte per satu baris.
        if (!SDL_UpdateTexture(framebufferTexture, NULL, pixels.data(), width * sizeof(std::uint32_t))) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not update framebuffer: %s\n", SDL_GetError());
            done = true;
            continue;
        }

        // Bersihkan frame sebelumnya, gambar texture ke window, lalu tampilkan frame baru.
        SDL_RenderClear(renderer);
        if (!SDL_RenderTexture(renderer, framebufferTexture, NULL, NULL)) {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not render framebuffer: %s\n", SDL_GetError());
            done = true;
            continue;
        }
        SDL_RenderPresent(renderer);
    }

    // Close and destroy the window
    SDL_DestroyTexture(framebufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
//eee
    // Clean up
    SDL_Quit();
    return 0;
}