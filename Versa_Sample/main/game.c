//******************************************************************************
// INCLUDES

#include <stdint.h>
#include "global.h"
#include "tftdriver.h"
#include "game.h"
#include <math.h>
#include "esp_random.h"

//------------------------------------------------------------------------------
// Local variables

static uint8_t xReverse = FALSE;
static uint8_t yReverse = FALSE;

static float xDisplacement;
static float yDisplacement;

static uint16_t ballColor;

float xPosition;
float yPosition;


uint8_t moving = FALSE;

struct ball gameBoard[BOARD_VERTICAL_SPACES][BOARD_HORIZONTAL_SPACES];

//------------------------------------------------------------------------------
// FUNCTIONS


//******************************************************************************
uint16_t randomColor(void)
//******************************************************************************
// Description: Generates a random color
// Parameters: none - Returns: none
//******************************************************************************
{
  switch(esp_random()%(COLOR_NUM+1)){
    case 0:
      return BLACK_COLOR;
    case 1:
      return RED_COLOR;
    case 2:
      return GREEN_COLOR;
    case 3:
      return BLUE_COLOR;
    case 4:
      return YELLOW_COLOR;
    case 5:
      return CYAN_COLOR;
    case 6:
      return VIOLET_COLOR;
    default:
      break;
  }
  return 0;
}


//******************************************************************************
void generateBoard(void)
//******************************************************************************
// Description: Generates the playing board
// Parameters: none - Returns: none
//******************************************************************************
{
  uint8_t ballsPlaced = 0;
  
  for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES;j++){
      if(ballsPlaced<=20){
        gameBoard[i][j].color = randomColor();
        if(gameBoard[i][j].color != BLACK_COLOR) ballsPlaced++;
      }
      else{
        gameBoard[i][j].color = BLACK_COLOR;
      }
      /**/
    }
  }
}

//******************************************************************************
void displayBoard(void)
//******************************************************************************
// Description: Displays the playing board
// Parameters: none - Returns: none
//******************************************************************************
{
  for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES-i%2;j++){
      LCD_DrawCircle((j*2+1+i%2)*BALL_RADIUS, (i*2+1)*BALL_LAYER_DISTANCE, BALL_RADIUS, gameBoard[i][j].color);
    }
  }
}

//******************************************************************************
void updateBallLocation(void)
//******************************************************************************
// Description: updates the current location of the ball
// Parameters: none - Returns: none
//******************************************************************************
{ 
  if(!moving) return;
  LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, BLACK_COLOR);

  xPosition += (xReverse)?-xDisplacement:+xDisplacement;
  yPosition += (yReverse)?-yDisplacement:yDisplacement;

  //Horizontal collision
  if(xPosition>=SCREEN_WIDTH-(BALL_RADIUS)){
    xReverse = !xReverse;
    xPosition -= 2*(xPosition-(SCREEN_WIDTH - BALL_RADIUS));
  }
  if(xPosition<=BALL_RADIUS){
    xReverse = !xReverse;
    xPosition += 2*(BALL_RADIUS - xPosition);
  }

  //Vertical collision
  if(yPosition>=SCREEN_HEIGHT-(BALL_RADIUS)){
    yReverse = !yReverse;
    yPosition -= 2*(yPosition-(SCREEN_HEIGHT - BALL_RADIUS));
  }
  if(yPosition<=BALL_RADIUS){
    yReverse = !yReverse;
    yPosition += 2*(BALL_RADIUS - yPosition);
  }

  //Ball collision
  for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES-i%2;j++){
      if(gameBoard[i][j].color != BLACK_COLOR){
        if(sqrt(pow((j*2+1+i%2)*BALL_RADIUS - xPosition, 2)+pow((i*2+1)*BALL_LAYER_DISTANCE - yPosition, 2)) < 2*BALL_RADIUS) stopBall();
      }
    }
  }
  /**/

  LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, ballColor);
}

//******************************************************************************
void throwBall(float dir,uint16_t col)
//******************************************************************************
// Description: sets the ball on motion with the set direction
// Parameters: Ball direction - Returns: none
//******************************************************************************
{ 
  ballColor = col;
  xPosition = BALL_X_ORIGIN;
  yPosition = BALL_Y_ORIGIN;
  xDisplacement = BALL_SPEED*cos(dir);
  yDisplacement = -BALL_SPEED*sin(dir);
  moving = TRUE;
  xReverse = FALSE;
  yReverse = FALSE;
}


//******************************************************************************
void stopBall(void)
//******************************************************************************
// Description: Stops ball movement and sets the ball location to the nearest 
//              point on the grid
// Parameters: none - Returns: none
//******************************************************************************
{ 
  if(!moving) return;

  moving = FALSE;

  LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, BLACK_COLOR);
  uint8_t ballLayer = round(((float)yPosition-BALL_LAYER_DISTANCE)/(2*BALL_LAYER_DISTANCE));
  yPosition = (2*ballLayer+1)*BALL_LAYER_DISTANCE;
  uint8_t ballInLayerPos = round(((float)xPosition-(1+ballLayer%2)*BALL_RADIUS)/(2*BALL_RADIUS));
  xPosition = (2*ballInLayerPos + 1+ballLayer%2)*BALL_RADIUS;
  gameBoard[ballLayer][ballInLayerPos].color = ballColor;

  displayBoard();

  xPosition = BALL_X_ORIGIN;
  yPosition = BALL_Y_ORIGIN;
}
