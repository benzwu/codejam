#include <SDL2/SDL.h>

void music(Uint8* stream, int length)
{
    static int t = 0;
    for (int i = 0; i < length; i++) {
        stream[i] = (t * (t >> 5 | t >> 8)) >> (t >> 16); 
        t++;
    }
}

void audioCallback(void* data, Uint8* stream, int length)
{
    music(stream, length);
}

int main()
{
    SDL_Init(SDL_INIT_AUDIO);

    SDL_AudioSpec audioWant, audioHave;
    audioWant.freq = 8000;
    audioWant.format = AUDIO_S8;
    audioWant.channels = 1;
    audioWant.samples = 1024;
    audioWant.callback = audioCallback;
    SDL_OpenAudio(&audioWant, &audioHave);
    SDL_PauseAudio(0);

    while (1) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                exit(0);
            }
        }
        SDL_Delay(1000);
    }
}