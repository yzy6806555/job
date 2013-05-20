
#include "stdafx.h"
extern "C"
{
	#include "SDL.h"
	#include "SDL_mixer.h"
};


/* Mix_Chunk is like Mix_Music, only it's for ordinary sounds. */
static Mix_Chunk *phaser = NULL;

/* Every sound that gets played is assigned to a channel.  Note that
   this is different from the number of channels you request when you
   open the audio device; 
		a channel in SDL_mixer holds information about a sound sample that is playing, 
	while the number of channels you request when opening the device is dependent 
	on what sort of sound you want (1 channel = mono, 2 = stereo, etc) */
int phaserChannel = -1;

void handleKey2(SDL_KeyboardEvent key);

int main1(int arvc , char * argv[]) {

  SDL_Surface *screen;
  SDL_Event event;
  int done = 0;

  /* Same setup as before */
  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16; 
  int audio_channels = 2;
  int audio_buffers = 4096;

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
    printf("Unable to open audio!\n");
    exit(1);
  }

  /* We're going to pre-load the sound effects that we need right here
   */
  phaser = Mix_LoadWAV("phaser.wav");

  screen = SDL_SetVideoMode(320, 240, 0, 0);

  while(!done) {
    while(SDL_PollEvent(&event)) {

      switch(event.type)
	  {
      case SDL_QUIT:
			done = 1;
			break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
			handleKey2(event.key);
			break;
      }
    }
    SDL_Delay(50);

  }

  Mix_CloseAudio();
  SDL_Quit();

}

void handleKey2(SDL_KeyboardEvent key) {
  switch(key.keysym.sym)
  {
  case SDLK_p:

    /* We're going to have the phaser continually fire as long as
       the user is holding the button down */
    if(key.type == SDL_KEYDOWN) {

      if(phaserChannel < 0) {

	/* Mix_PlayChannel takes, as its arguments, the channel that
	   the given sound should be played on, the sound itself, and
	   the number of times it should be looped.  If you don't care
	   what channel the sound plays on, just pass in -1.  Looping
	   works like Mix_PlayMusic. This function returns the channel
	   that the sound was assigned to, which you'll need later. */
	   phaserChannel = Mix_PlayChannel(-1, phaser, -1);
	
      }
    } else {
      /* Mix_HaltChannel stops a certain channel from playing - this
	 is one of the reasons we kept track of which channel the
	 phaser had been assigned to */
      Mix_HaltChannel(phaserChannel);
      
      phaserChannel = -1;
    }
    break;
  }
}