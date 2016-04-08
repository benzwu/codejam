#include "controller.h"

#include <SDL/SDL.h>

#include "note.h"
#include "components/component.h"
#include "sh101.h"
#include "view.h"

Controller::Controller(SH101 * sh101, const View * view) : sh101(sh101), view(view)
{
  this->running = true;
  this->selectedComponent = -1;
}
  
void Controller::processEvents()
{
  int mouseEvent = NULL;
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      this->running = false;
      return;
    }

    switch (event.type) {
      case SDL_MOUSEBUTTONUP:
        mouseEvent = SDL_MOUSEBUTTONUP;
        if (this->selectedComponent != -1) {
          this->view->components.at(this->selectedComponent)->setValue(mouseX, mouseY, mouseEvent);
          this->selectedComponent = -1;
        }
        break;
      case SDL_MOUSEBUTTONDOWN:
        mouseEvent = SDL_MOUSEBUTTONDOWN;
        this->selectedComponent = this->view->pick(mouseX, mouseY);
        break;
      case SDL_MOUSEMOTION:
        mouseEvent = SDL_MOUSEMOTION;
        break;
    }

    int note = -1;
    switch (event.key.keysym.sym) {
      case SDLK_z: note = 7; break;
      case SDLK_s: note = 8; break;
      case SDLK_x: note = 9; break;
      case SDLK_d: note = 10; break;
      case SDLK_c: note = 11; break;
      case SDLK_v: note = 12; break;
      case SDLK_g: note = 13; break;
      case SDLK_b: note = 14; break;
      case SDLK_h: note = 15; break;
      case SDLK_n: note = 16; break;
      case SDLK_j: note = 17; break;
      case SDLK_m: note = 18; break;
      case SDLK_q: note = 19; break;
      case SDLK_2: note = 20; break;
      case SDLK_w: note = 21; break;
      case SDLK_3: note = 22; break;
      case SDLK_e: note = 23; break;
      case SDLK_r: note = 24; break;
      case SDLK_5: note = 25; break;
      case SDLK_t: note = 26; break;
      case SDLK_6: note = 27; break;
      case SDLK_y: note = 28; break;
      case SDLK_7: note = 29; break;
      case SDLK_u: note = 30; break;
      case SDLK_i: note = 31; break;
    }
    if (note != -1) {
      this->sh101->setNoteOn(note, event.type != SDL_KEYUP);
    }
  }

  if (this->selectedComponent != -1) {
    this->view->components.at(this->selectedComponent)->setValue(mouseX, mouseY, mouseEvent);
  }
}
