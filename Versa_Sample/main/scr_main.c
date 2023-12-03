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
#include "game.h"

//******************************************************************************
// DEFINES & TYPEDEFS

float throwDirection = PI/2;

static uint16_t UI_timer_1000ms = 0;

//******************************************************************************
// FUNCTIONS

//******************************************************************************
static void setBallDirection(tUIEvent button)
//******************************************************************************
// Description: Set ball direction in radians
// Parameters: button - button pressed
// Returns: none
//******************************************************************************
{
  LCD_DrawDashedPolarLine(BALL_X_ORIGIN, BALL_Y_ORIGIN, GUIDE_LINE_LENGTH, throwDirection, GUIDE_LINE_DASH_LENGTH, GUIDE_LINE_GAP_LENGTH, GAME_BACKGROUND_COLOR);

  switch (button)
  {
    case EV_KEY_L_TRIGGER_PRESS:
    case EV_KEY_LEFT_PRESS:
        throwDirection+=0.05;
      break;
    case EV_KEY_R_TRIGGER_PRESS:
    case EV_KEY_RIGHT_PRESS:
        throwDirection-=0.05;
      break;

    default:
      break;
  }
  
  if(throwDirection < 0.14) throwDirection = 0.14;
  if(throwDirection > 3) throwDirection = 3;
  
  LCD_DrawDashedPolarLine(BALL_X_ORIGIN, BALL_Y_ORIGIN, GUIDE_LINE_LENGTH, throwDirection, GUIDE_LINE_DASH_LENGTH, GUIDE_LINE_GAP_LENGTH, GUIDE_LINE_COLOR);
}
//******************************************************************************
static void setThrowInstruction(tUIEvent button)
//******************************************************************************
// Description: setThrowInstruction
// Parameters: button - button pressed
// Returns: none
//******************************************************************************
{
  switch (button)
  {
  case EV_FULL_REDRAW:
  case EV_PARTIAL_REDRAW:
    break;
  case EV_KEY_UP_PRESS:
    //throwBall(throwDirection);
    break;
  case EV_KEY_DOWN_PRESS:
    break;
  case EV_KEY_LEFT_PRESS:
    break;
  case EV_KEY_RIGHT_PRESS:
    break;
  case EV_KEY_OK_PRESS:
    break;
  case EV_KEY_RETURN_PRESS:
    break;
  case EV_KEY_L_TRIGGER_PRESS:
    break;
  case EV_KEY_R_TRIGGER_PRESS:
    break;
  default:
    break;
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
    switch (event)
    {
    case EV_INIT: break;// Initialization event
    case EV_FULL_REDRAW:
    case EV_PARTIAL_REDRAW:
      //Start game
      initGame();

      /*
      for(uint16_t i=0; i<=SCREEN_WIDTH/5;i++){
        LCD_DrawRectangle(i*5,0,1,SCREEN_HEIGHT,WHITE_COLOR);
      }
      for(uint16_t i=0; i<=SCREEN_HEIGHT/5;i++){
        LCD_DrawRectangle(0,i*5,SCREEN_WIDTH,1,WHITE_COLOR);
      }
      */
      break;
    case EV_TIMER_20MS:
      updateBallLocation();
      if(++UI_timer_1000ms >= 50){
        UI_timer_1000ms = 0;
        updateTimer();
      }
      break;
    case EV_KEY_UP_PRESS:
      if(!moving) throwBall(throwDirection);
      break;
    case EV_KEY_DOWN_PRESS:
      //setBallDirection(event);
      break;
    case EV_KEY_LEFT_PRESS:
      setBallDirection(event);
      break;
    case EV_KEY_RIGHT_PRESS:
      setBallDirection(event);
      break;
    case EV_KEY_OK_PRESS:
      setBallDirection(event);
      break;
    case EV_KEY_RETURN_PRESS:
      stopBall();
      break;
    case EV_KEY_L_TRIGGER_PRESS:
      setBallDirection(event);
      break;
    case EV_KEY_R_TRIGGER_PRESS:
      setBallDirection(event);
      break;
    case EV_KEY_START_PRESS:
      // Apagar
      LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0);
      DisablePower();
      SysSleep(500);
      break;
    case EV_KEY_SELECT_PRESS:
      // Apagar
      LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0);
      DisablePower();
      SysSleep(500);
      break;
    default: 
      UIIdle();
    }
  }
}

