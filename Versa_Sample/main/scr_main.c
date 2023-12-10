//******************************************************************************
// File: scr_main.c
// Description: Main menu
// Author: Juan Ramon Vadillo (Versa Design S.L.)
// Licencia: Creative Commons No Comercial (CC BY-NC)
//
// Este archivo está sujeto a la Licencia Creative Commons No Comercial (CC BY-NC).
// Esto significa que puedes compartir, copiar, modificar y distribuir este archivo
// siempre y cuando sea para uso personal y no comercial, y siempre que se proporcione
// atribución al autor original. No se permite el uso comercial de este archivo ni la
// creación de obras derivadas sin el consentimiento expreso del autor.
//
// Para más detalles sobre la Licencia Creative Commons No Comercial (CC BY-NC), puedes
// consultar: [https://creativecommons.org/licenses/by-nc/4.0/]
//******************************************************************************

//******************************************************************************
// INCLUDES

#include <stdint.h>
#include <string.h>
#include "global.h"
#include "tftdriver.h"
#include "scr_main.h"
#include "hwplatform.h"
#include <stdio.h>
#include "task_ui.h"
#include <math.h>

//******************************************************************************
// DEFINES & TYPEDEFS

static uint16_t UI_timer_1000ms = 0;

tMenu currentMenu = START_MENU;

//******************************************************************************
// FUNCTIONS

//******************************************************************************
static void startMenuHandler(tUIEvent event)
//******************************************************************************
// Description: Start screen handler
//******************************************************************************
{
  switch (event)
  {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      mainMenuInit();
      break;
    case EV_KEY_OK_PRESS:
      selectButton();
      break;
    case EV_KEY_UP_PRESS:
      switch(getCurrentButton())
      {
        case QUIT:
          setCurrentButton(SETTINGS);
          break;
        case SETTINGS:
          setCurrentButton(PLAY);
          break;
        default:
          break;
      }
      break;
    case EV_KEY_DOWN_PRESS:
      switch(getCurrentButton())
      {
        case PLAY:
          setCurrentButton(SETTINGS);
          break;
        case SETTINGS:
          setCurrentButton(QUIT);
          break;
        default:
          break;
      }
      break;
    default: 
      UIIdle();
  }
}

//******************************************************************************
static void settingsHandler(tUIEvent event)
//******************************************************************************
// Description: Settings screen handler
//******************************************************************************
{
  switch (event)
  {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      settingsInit();
      break;    
    case EV_KEY_OK_PRESS:
      selectButton();
      break;
    case EV_KEY_UP_PRESS:
      switch(getCurrentButton())
      {
        case MAIN_MENU:
          setCurrentButton(SOUND_ENABLED);
          break;
        default:
          break;
      }
      break;
    case EV_KEY_DOWN_PRESS:
      switch(getCurrentButton())
      {
        case SOUND_ENABLED:
          setCurrentButton(MAIN_MENU);
          break;
        default:
          break;
      }
      break;
    default: 
      UIIdle();
  }
}

//******************************************************************************
static void pauseHandler(tUIEvent event)
//******************************************************************************
// Description: Pause screen handler
//******************************************************************************
{
  switch (event)
  {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      pauseInit();
      break;
    case EV_KEY_OK_PRESS:
      selectButton();
      break;
    case EV_KEY_UP_PRESS:
      switch(getCurrentButton())
      {
        case MAIN_MENU:
          setCurrentButton(RESUME);
          break;
        default:
          break;
      }
      break;
    case EV_KEY_DOWN_PRESS:
      switch(getCurrentButton())
      {
        case RESUME:
          setCurrentButton(MAIN_MENU);
          break;
        default:
          break;
      }
      break;
    case EV_KEY_START_PRESS:
    case EV_KEY_SELECT_PRESS:
      setCurrentButton(RESUME);
      selectButton();
    default: 
      UIIdle();
  }
}

//******************************************************************************
static void gameHandler(tUIEvent event)
//******************************************************************************
// Description: Game screen handler
//******************************************************************************
{
  switch (event)
  {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      //Start game
      initGame();
      break;
    case EV_TIMER_20MS:
      updateBallLocation();
      if(++UI_timer_1000ms >= 50){
        UI_timer_1000ms = 0;
        updateTimer();
      }
      break;
    case EV_KEY_OK_PRESS:
    case EV_KEY_UP_PRESS:
      throwBall();
      break;

    case EV_KEY_LEFT_PRESS:
    case EV_KEY_RIGHT_PRESS:
    case EV_KEY_L_TRIGGER_PRESS:
    case EV_KEY_R_TRIGGER_PRESS:
      setBallDirection(event);
      break;
    case EV_KEY_START_PRESS:
    case EV_KEY_SELECT_PRESS:
      setCurrentMenu(PAUSE_MENU);
      break;
    default: 
      UIIdle();
  }
}

//******************************************************************************
static void gameOverHandler(tUIEvent event)
//******************************************************************************
// Description: Game over screen handler
//******************************************************************************
{
  switch (event)
  {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      gameOverInit();
      break;   
    case EV_KEY_OK_PRESS:
      selectButton();
      break;
    case EV_KEY_START_PRESS:
    case EV_KEY_SELECT_PRESS:
      setCurrentButton(MAIN_MENU);
      selectButton();
    default: 
      UIIdle();
  }
}

//******************************************************************************
static void gameFinishedHandler(tUIEvent event)
//******************************************************************************
// Description: Game end screen handler
//******************************************************************************
{
  switch (event)
  {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      levelFinishedInit();
      break;
    case EV_KEY_OK_PRESS:
      selectButton();
      break;
    case EV_KEY_UP_PRESS:
      switch(getCurrentButton())
      {
        case MAIN_MENU:
          setCurrentButton(NEXT_LEVEL);
          break;
        default:
          break;
      }
      break;
    case EV_KEY_DOWN_PRESS:
      switch(getCurrentButton())
      {
        case NEXT_LEVEL:
          setCurrentButton(MAIN_MENU);
          break;
        default:
          break;
      }
      break;
    case EV_KEY_START_PRESS:
    case EV_KEY_SELECT_PRESS:
      setCurrentButton(NEXT_LEVEL);
      selectButton();
      break;
    default: 
      UIIdle();
  }
}

//******************************************************************************
void MainMenuScreenHandler(void)
//******************************************************************************
// Description: Main screen handler
//******************************************************************************
{
  tUIEvent event;

  UIForceFullRedraw(); // When we enter this menu, we have to force full redraw

  for (;;)
  {
    event = GetUserInterfaceEvent();

    switch (currentMenu)
    {
    case START_MENU:
      startMenuHandler(event);
      break;
    case SETTINGS_MENU:
      settingsHandler(event);
      break;
    case PAUSE_MENU:
      pauseHandler(event);
      break;
    case GAME_MENU:
      gameHandler(event);
      break;
    case GAME_OVER_MENU:
      gameOverHandler(event);
      break;
    case GAME_FINISHED_MENU:
      gameFinishedHandler(event);
      break;
    }
  }
}

//******************************************************************************
void setCurrentMenu(tMenu menu)
//******************************************************************************
// Description: Game end screen handler
//******************************************************************************
{
  currentMenu = menu;
  UIForceFullRedraw();
}