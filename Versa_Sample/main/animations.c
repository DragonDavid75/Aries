//******************************************************************************
// INCLUDES

#include <stdint.h>
#include "global.h"
#include "tftdriver.h"
#include "animations.h"
#include <math.h>

//------------------------------------------------------------------------------
// Local variables

static uint8_t moving = FALSE;
static uint8_t xReverse = FALSE;
static uint8_t yReverse = FALSE;

//------------------------------------------------------------------------------
// FUNCTIONS

//******************************************************************************
void setBallParam(uint16_t xStartPos, uint16_t yStartPos, uint16_t r, uint16_t movSpeed, uint16_t color)
//******************************************************************************
// Description: sets the ball parameters
// Parameters: uint16_t xStartPos, uint16_t yStartPos, uint16_t r, uint16_t movSpeed
// Returns: none
//******************************************************************************
{   
  ballRadius = r;

  start_xPosition = xStartPos;
  start_yPosition = yStartPos;

  speed = movSpeed;
  ballColor = color;
}

//******************************************************************************
void updateBallLocation(void)
//******************************************************************************
// Description: updates the current location of the ball
// Parameters: none - Returns: none
//******************************************************************************
{ 
  if(!moving) return;
  LCD_DrawCircle(xPosition, yPosition, ballRadius, BLACK_COLOR);

  xPosition += (xReverse)?-speed*cos(direction):+speed*cos(direction);
  yPosition += (yReverse)?+speed*sin(direction):-speed*sin(direction);

  //Horizontal collision
  if(xPosition>=SCREEN_WIDTH-(ballRadius)){
    xReverse = !xReverse;
    xPosition -= 2*(xPosition-(SCREEN_WIDTH - ballRadius));
  }
  if(xPosition<=ballRadius){
    xReverse = !xReverse;
    xPosition += 2*(ballRadius - xPosition);
  }

  //Vertical collision
  if(yPosition>=SCREEN_HEIGHT-(ballRadius)){
    yReverse = !yReverse;
    yPosition -= 2*(yPosition-(SCREEN_HEIGHT - ballRadius));
  }
  if(yPosition<=ballRadius){
    yReverse = !yReverse;
    yPosition += 2*(ballRadius - yPosition);
  }

  LCD_DrawCircle(xPosition, yPosition, ballRadius, ballColor);
}

//******************************************************************************
void throwBall(float dir)
//******************************************************************************
// Description: sets the ball on motion with the set direction
// Parameters: Ball direction - Returns: none
//******************************************************************************
{ 
  xPosition = start_xPosition;
  yPosition = start_yPosition;
  direction = dir;
  moving = TRUE;
  xReverse = FALSE;
  yReverse = FALSE;
}


//******************************************************************************
void stopBall(void)
//******************************************************************************
// Description: Stops ball movement
// Parameters: none - Returns: none
//******************************************************************************
{ 
  moving = FALSE;
  xPosition = start_xPosition;
  yPosition = start_yPosition;
}
