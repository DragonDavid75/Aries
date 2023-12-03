//******************************************************************************
// INCLUDES

#include <stdint.h>
#include "global.h"
#include "tftdriver.h"
#include "game.h"
#include <math.h>
#include "esp_random.h"
#include "hwplatform.h"

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
uint8_t ballsOnBoard;

uint32_t gameScore = 0;
uint8_t currentLevel = 1;

uint16_t timerTime = 0; //in seconds
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
      return GAME_BACKGROUND_COLOR;
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
  
  for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES-1;i++){
    for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES-1;j++){
      if(ballsPlaced<=BALLS_ON_BOARD){
        gameBoard[i][j].color = randomColor();
        if(gameBoard[i][j].color != GAME_BACKGROUND_COLOR) ballsPlaced++;
      }
      else{
        gameBoard[i][j].color = GAME_BACKGROUND_COLOR;
      }
      gameBoard[i][j].visited = FALSE;
    }
  }
  ballsOnBoard = BALLS_ON_BOARD;
}

//******************************************************************************
void displayBoard(void)
//******************************************************************************
// Description: Displays the playing board
// Parameters: none - Returns: none
//******************************************************************************
{
  for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES-1;i++){
    for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES-1-i%2;j++){
      LCD_DrawCircle((j*2+1+i%2)*BALL_RADIUS, (i*2+1)*BALL_LAYER_DISTANCE, BALL_RADIUS, gameBoard[i][j].color);
    }
  }
  drawScore(SCORE_MENU_TEXT_COLOR);
}

//******************************************************************************
void initGame(void)
//******************************************************************************
// Description: Starts game
// Parameters: none - Returns: none
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR);
  LCD_DrawRectangle(0,SCREEN_HEIGHT-2*BALL_RADIUS,SCREEN_WIDTH,2*BALL_RADIUS,SCORE_MENU_BACKGROUND_COLOR);

  generateBoard();
  displayBoard();

  do{
    setBall(randomColor());
  }while(ballColor == GAME_BACKGROUND_COLOR);
  
  setTimer(5*60);
  placeGameOverLine();
}

//******************************************************************************
void updateBallLocation(void)
//******************************************************************************
// Description: updates the current location of the ball
// Parameters: none - Returns: none
//******************************************************************************
{ 
  if(moving){
    LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, GAME_BACKGROUND_COLOR);

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
    /*
    if(yPosition>=SCREEN_HEIGHT-(BALL_RADIUS)){
      yReverse = !yReverse;
      yPosition -= 2*(yPosition-(SCREEN_HEIGHT - BALL_RADIUS));
    }
    */
    if(yPosition<=BALL_RADIUS){
      stopBall();
      //yReverse = !yReverse;
      //yPosition += 2*(BALL_RADIUS - yPosition);
    }

    //Ball collision
    for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES-1;i++){
      for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES-1-i%2;j++){
        if(gameBoard[i][j].color != GAME_BACKGROUND_COLOR){
          if(sqrt(pow((j*2+1+i%2)*BALL_RADIUS - xPosition, 2)+pow((i*2+1)*BALL_LAYER_DISTANCE - yPosition, 2)) < 2*BALL_RADIUS) stopBall();
        }
      }
    }
  }
  placeGameOverLine();
  LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, ballColor);
}
//******************************************************************************
void setBall(uint16_t col)
//******************************************************************************
// Description: sets the ball characteristics
// Parameters: Ball color - Returns: none
//******************************************************************************
{ 
  ballColor = col;
  xPosition = BALL_X_ORIGIN;
  yPosition = BALL_Y_ORIGIN;
}

//******************************************************************************
void throwBall(float dir)
//******************************************************************************
// Description: sets the ball on motion with the set direction
// Parameters: Ball direction - Returns: none
//******************************************************************************
{
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
  ballsOnBoard++;

  LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, GAME_BACKGROUND_COLOR);
  uint8_t ballLayer = round(((float)yPosition-BALL_LAYER_DISTANCE)/(2*BALL_LAYER_DISTANCE));
  yPosition = (2*ballLayer+1)*BALL_LAYER_DISTANCE;
  uint8_t ballInLayerPos = round(((float)xPosition-(1+ballLayer%2)*BALL_RADIUS)/(2*BALL_RADIUS));
  xPosition = (2*ballInLayerPos + 1+ballLayer%2)*BALL_RADIUS;
  gameBoard[ballLayer][ballInLayerPos].color = ballColor;


  uint16_t clusterSize = checkCluster(ballLayer, ballInLayerPos);
  if(clusterSize >= CLUSTER_MIN_SIZE){
    ballsOnBoard -= clusterSize;
    drawScore(SCORE_MENU_BACKGROUND_COLOR);
    gameScore += currentLevel*clusterSize;
    drawScore(SCORE_MENU_TEXT_COLOR);
  }

  if(ballLayer>=BOARD_VERTICAL_SPACES-1 && clusterSize<CLUSTER_MIN_SIZE) gameOver();//Check if ball on game over line

  displayBoard();
  placeGameOverLine();

  do{
    setBall(randomColor());
  }while(ballColor == GAME_BACKGROUND_COLOR);
}
//******************************************************************************
uint16_t checkCluster(uint8_t ballLayer, uint8_t ballInLayerPos)
//******************************************************************************
// Description: Check the size of the cluster
// Parameters: Start position on the game board - Returns: number of balls in the cluster
//******************************************************************************
{
  uint16_t clusterSize = 0;
  recursiveClusterCheck(ballLayer, ballInLayerPos, &clusterSize);

  for(uint16_t i = 0; i<=BOARD_VERTICAL_SPACES-1;i++){
    for(uint16_t j = 0; j<=BOARD_HORIZONTAL_SPACES-1;j++){
      if(clusterSize >= CLUSTER_MIN_SIZE && gameBoard[i][j].visited == TRUE) gameBoard[i][j].color = GAME_BACKGROUND_COLOR;
      gameBoard[i][j].visited = FALSE;
    }
  }

  return clusterSize;
}

//******************************************************************************
void recursiveClusterCheck(int8_t ballLayer, int8_t ballInLayerPos, uint16_t* clusterSize)
//******************************************************************************
// Description: Check the size of the cluster in a recursive manner
// Parameters: Start position on the game board - Returns: None
//******************************************************************************
{
  if(ballLayer < 0 || ballLayer >= BOARD_VERTICAL_SPACES) return;
  if(ballInLayerPos < 0 || ballInLayerPos >= BOARD_HORIZONTAL_SPACES) return;

  gameBoard[ballLayer][ballInLayerPos].visited = TRUE;
  *clusterSize += 1;

  for(int8_t i = -1; i <= 1; i++){
    for(int8_t j = 0; j <= 1; j++){
      int8_t ballInLayerToCheck = ballInLayerPos + j*(2-abs(i)) + abs(i)*(ballLayer%2) - 1;
      if(gameBoard[ballLayer+i][ballInLayerToCheck].visited==FALSE && gameBoard[ballLayer+i][ballInLayerToCheck].color == gameBoard[ballLayer][ballInLayerPos].color) recursiveClusterCheck(ballLayer+i, ballInLayerToCheck, clusterSize);
    }
  }
}


//******************************************************************************
void drawScore(uint16_t col)
//******************************************************************************
// Description: Shows or erases score from display
// Parameters: None
//******************************************************************************
{
  LCD_DrawNumber(SCORE_X_ORIGIN, SCORE_Y_ORIGIN, gameScore, SCORE_LEN, col);
}

//******************************************************************************
void drawTimer(uint16_t col)
//******************************************************************************
// Description: Shows or erases timer from display
// Parameters: None
//******************************************************************************
{
  LCD_DrawNumber(TIMER_X_ORIGIN, TIMER_Y_ORIGIN, timerTime/60, 2, col);                           //Minutes
  LCD_DrawCharacter(TIMER_X_ORIGIN+2*(NUMBER_SPRITE_WIDTH+1), TIMER_Y_ORIGIN, 10, col);           //:
  LCD_DrawNumber(TIMER_X_ORIGIN+3*(NUMBER_SPRITE_WIDTH+1), TIMER_Y_ORIGIN, timerTime%60, 2, col); //Seconds
}

//******************************************************************************
void updateTimer(void)
//******************************************************************************
// Description: Update timer from display
// Parameters: None
//******************************************************************************
{
  drawTimer(SCORE_MENU_BACKGROUND_COLOR);
  if((timerTime>0))timerTime--;
  else gameOver();
  drawTimer(SCORE_MENU_TEXT_COLOR);
}

//******************************************************************************
void setTimer(uint16_t t)
//******************************************************************************
// Description: Set timer time and show on display
// Parameters: Timer start time in seconds
//******************************************************************************
{
  timerTime = t;
  drawTimer(SCORE_MENU_TEXT_COLOR);
}

//******************************************************************************
void placeGameOverLine(void)
//******************************************************************************
// Description: Draws game over dashed line 
// Parameters: None
//******************************************************************************
{
  LCD_DrawDashedLine(0, GAME_OVER_LINE_HEIGHT, SCREEN_WIDTH, GAME_OVER_LINE_HEIGHT, GAME_OVER_LINE_DASH_LENGTH, GAME_OVER_LINE_GAP_LENGTH, GAME_OVER_LINE_COLOR);
}

//******************************************************************************
void gameOver(void)
//******************************************************************************
// Description: End of game
// Parameters: None
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0);
  DisablePower();
  SysSleep(500);
}
