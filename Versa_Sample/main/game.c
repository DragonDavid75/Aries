//******************************************************************************
// INCLUDES

#include <stdint.h>
#include "global.h"
#include "tftdriver.h"
#include "game.h"
#include <math.h>
#include "esp_random.h"
#include "hwplatform.h"
#include <sprites.h>
#include "scr_main.h"
#include <string.h>

//------------------------------------------------------------------------------
// Local variables

static uint8_t xReverse = FALSE;
static uint8_t yReverse = FALSE;

static float xDisplacement;
static float yDisplacement;

static uint16_t ballColor;

float xPosition;
float yPosition;

float throwDirection = PI/2;

uint8_t moving = FALSE;

struct ball gameBoard[BOARD_VERTICAL_SPACES][BOARD_HORIZONTAL_SPACES];
uint8_t ballsOnBoard = 0;

uint32_t gameScore = 0;
uint8_t currentLevel = 1;

uint16_t timerTime = 0; //in seconds

tMenuButton currentButton = NONE;

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
  
  for(uint16_t i = 0; i<BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<BOARD_HORIZONTAL_SPACES-i%2;j++){
      if(ballsPlaced<BALLS_PER_LEVEL*currentLevel){
        gameBoard[i][j].color = randomColor();
        if(gameBoard[i][j].color != GAME_BACKGROUND_COLOR) ballsPlaced++;
      }
      else{
        gameBoard[i][j].color = GAME_BACKGROUND_COLOR;
      }
      gameBoard[i][j].visited = FALSE;
    }
  }
  ballsOnBoard = ballsPlaced;

  if(checkUnconectedBall()) generateBoard();
}

//******************************************************************************
void displayBoard(void)
//******************************************************************************
// Description: Displays the playing board
// Parameters: none - Returns: none
//******************************************************************************
{
  for(uint16_t i = 0; i<BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<BOARD_HORIZONTAL_SPACES-i%2;j++){
      if(gameBoard[i][j].color != GAME_BACKGROUND_COLOR) LCD_DrawCircle((j*2+1+i%2)*BALL_RADIUS, (i*2+1)*BALL_LAYER_DISTANCE, BALL_RADIUS, gameBoard[i][j].color);
    }
  }
}

//******************************************************************************
void initGame(void)
//******************************************************************************
// Description: Starts game
// Parameters: none - Returns: none
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR); //Clear screen
  LCD_DrawRectangle(SCORE_MENU_X_ORIGIN,SCORE_MENU_Y_ORIGIN,SCORE_MENU_WIDTH,SCORE_MENU_HEIGHT,SCORE_MENU_BACKGROUND_COLOR); //Draw Score menu 

  if(!ballsOnBoard) generateBoard();
  displayBoard();

  do{
    setBall(randomColor());
  }while(ballColor == GAME_BACKGROUND_COLOR);
  
  drawTimer(TIMER_X_ORIGIN, TIMER_Y_ORIGIN, SCORE_MENU_TEXT_COLOR, SCORE_MENU_TEXT_SIZE);
  drawScore(SCORE_X_ORIGIN, SCORE_Y_ORIGIN, SCORE_MENU_TEXT_COLOR, SCORE_MENU_TEXT_SIZE);
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
    if(yPosition<=BALL_RADIUS){
      collition();
      //yReverse = !yReverse;
      //yPosition += 2*(BALL_RADIUS - yPosition);
    }
    /*
    if(yPosition>=SCREEN_HEIGHT-(BALL_RADIUS)){
      yReverse = !yReverse;
      yPosition -= 2*(yPosition-(SCREEN_HEIGHT - BALL_RADIUS));
    }
    */

    //Ball collision
    for(uint16_t i = 0; i<BOARD_VERTICAL_SPACES;i++){
      for(uint16_t j = 0; j<BOARD_HORIZONTAL_SPACES-i%2;j++){
        if(gameBoard[i][j].color != GAME_BACKGROUND_COLOR){
          if(sqrt(pow((j*2+1+i%2)*BALL_RADIUS - xPosition, 2)+pow((i*2+1)*BALL_LAYER_DISTANCE - yPosition, 2)) < 2*BALL_RADIUS) collition();
        }
      }
    }
  }

  if(!ballsOnBoard) return;
  if(yPosition>=GAME_OVER_LINE_HEIGHT-2*BALL_RADIUS) placeGameOverLine();
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
void setBallDirection(tUIEvent button)
//******************************************************************************
// Description: Set ball direction in radians
// Parameters: button - button pressed
// Returns: none
//******************************************************************************
{
  if(moving) return;

  LCD_DrawDashedPolarLine(BALL_X_ORIGIN, BALL_Y_ORIGIN, GUIDE_LINE_LENGTH, throwDirection, GUIDE_LINE_DASH_LENGTH, GUIDE_LINE_GAP_LENGTH, GAME_BACKGROUND_COLOR, BALL_RADIUS);
  repairBoard();

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
  
  LCD_DrawDashedPolarLine(BALL_X_ORIGIN, BALL_Y_ORIGIN, GUIDE_LINE_LENGTH, throwDirection, GUIDE_LINE_DASH_LENGTH, GUIDE_LINE_GAP_LENGTH, GUIDE_LINE_COLOR, BALL_RADIUS);
}

//******************************************************************************
void throwBall(void)
//******************************************************************************
// Description: sets the ball on motion with the set direction
// Parameters: Ball direction - Returns: none
//******************************************************************************
{
  if(moving) return;
  xDisplacement = BALL_SPEED*cos(throwDirection);
  yDisplacement = -BALL_SPEED*sin(throwDirection);
  moving = TRUE;
  xReverse = FALSE;
  yReverse = FALSE;
}

//******************************************************************************
void collition(void)
//******************************************************************************
// Description: Stops ball movement and sets the ball location to the nearest 
//              point on the grid
// Parameters: none - Returns: none
//******************************************************************************
{ 
  if(!moving) return;

  moving = FALSE;
  ballsOnBoard++;

  //LCD_DrawCircle(xPosition, yPosition, BALL_RADIUS, GAME_BACKGROUND_COLOR);
  uint8_t ballLayer = round(((float)yPosition-BALL_LAYER_DISTANCE)/(2*BALL_LAYER_DISTANCE));
  yPosition = (2*ballLayer+1)*BALL_LAYER_DISTANCE;
  uint8_t ballInLayerPos = round(((float)xPosition-(1+ballLayer%2)*BALL_RADIUS)/(2*BALL_RADIUS));
  xPosition = (2*ballInLayerPos + 1+ballLayer%2)*BALL_RADIUS;
  gameBoard[ballLayer][ballInLayerPos].color = ballColor;


  uint16_t deletedBalls = checkCluster(ballLayer, ballInLayerPos) + checkUnconectedBall();
  if(deletedBalls){
    ballsOnBoard -= deletedBalls;
    drawScore(SCORE_X_ORIGIN, SCORE_Y_ORIGIN, SCORE_MENU_BACKGROUND_COLOR, SCORE_MENU_TEXT_SIZE);
    gameScore += currentLevel*deletedBalls;
    drawScore(SCORE_X_ORIGIN, SCORE_Y_ORIGIN, SCORE_MENU_TEXT_COLOR, SCORE_MENU_TEXT_SIZE);
  }

  if(ballLayer>=BOARD_VERTICAL_SPACES-1 && deletedBalls<CLUSTER_MIN_SIZE) setCurrentMenu(GAME_OVER_MENU);//Check if ball on game over line
  if(!ballsOnBoard) setCurrentMenu(GAME_FINISHED_MENU);

  displayBoard();

  do{
    setBall(randomColor());
  }while(ballColor == GAME_BACKGROUND_COLOR);
}

//******************************************************************************
uint16_t checkUnconectedBall()
//******************************************************************************
// Description: Check if there asre any unconected balls
// Parameters: None - Returns: number of balls in the cluster
//******************************************************************************
{
  uint16_t clusterSize = 0;
  for(uint16_t i = 0; i<BOARD_HORIZONTAL_SPACES; i++){
    if(gameBoard[0][i].visited==FALSE && gameBoard[0][i].color != GAME_BACKGROUND_COLOR) recursiveUnconectedBallCheck(0, i);
  }

  for(uint16_t i = 0; i<BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<BOARD_HORIZONTAL_SPACES-i%2;j++){
      if(gameBoard[i][j].visited == FALSE && gameBoard[i][j].color != GAME_BACKGROUND_COLOR){
        clusterSize++;
        gameBoard[i][j].color = GAME_BACKGROUND_COLOR;
        LCD_DrawCircle((j*2+1+i%2)*BALL_RADIUS, (i*2+1)*BALL_LAYER_DISTANCE, BALL_RADIUS, GAME_BACKGROUND_COLOR);
      } 
      gameBoard[i][j].visited = FALSE;
    }
  }

  return clusterSize;
}

//******************************************************************************
void recursiveUnconectedBallCheck(int8_t ballLayer, int8_t ballInLayerPos)
//******************************************************************************
// Description: Check the size of the cluster in a recursive manner
// Parameters: Start position on the game board - Returns: None
//******************************************************************************
{
  if(ballLayer < 0 || ballLayer >= BOARD_VERTICAL_SPACES) return;
  if(ballInLayerPos < 0 || ballInLayerPos >= BOARD_HORIZONTAL_SPACES) return;

  gameBoard[ballLayer][ballInLayerPos].visited = TRUE;

  for(int8_t i = -1; i <= 1; i++){
    for(int8_t j = 0; j <= 1; j++){
      int8_t ballInLayerToCheck = ballInLayerPos + j*(2-abs(i)) + abs(i)*(ballLayer%2) - 1;
      if(gameBoard[ballLayer+i][ballInLayerToCheck].visited==FALSE && gameBoard[ballLayer+i][ballInLayerToCheck].color != GAME_BACKGROUND_COLOR) recursiveUnconectedBallCheck(ballLayer+i, ballInLayerToCheck);
    }
  }
}

//******************************************************************************
uint16_t checkCluster(uint8_t ballLayer, uint8_t ballInLayerPos)
//******************************************************************************
// Description: Check the size of the cluster
// Parameters: Start position on the game board - Returns: number of balls deleted
//******************************************************************************
{
  uint16_t clusterSize = 0;
  recursiveClusterCheck(ballLayer, ballInLayerPos, &clusterSize);

  for(uint16_t i = 0; i<BOARD_VERTICAL_SPACES;i++){
    for(uint16_t j = 0; j<BOARD_HORIZONTAL_SPACES-i%2;j++){
      if(clusterSize >= CLUSTER_MIN_SIZE && gameBoard[i][j].visited == TRUE){
        gameBoard[i][j].color = GAME_BACKGROUND_COLOR;
        LCD_DrawCircle((j*2+1+i%2)*BALL_RADIUS, (i*2+1)*BALL_LAYER_DISTANCE, BALL_RADIUS, GAME_BACKGROUND_COLOR);
      }
      gameBoard[i][j].visited = FALSE;
    }
  }

  if(clusterSize < CLUSTER_MIN_SIZE) clusterSize = 0;
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
void drawScore(uint16_t x, uint16_t y, uint16_t col, uint8_t size)
//******************************************************************************
// Description: Shows or erases score from display
// Parameters: None
//******************************************************************************
{
  LCD_DrawNumber(x, y, gameScore, SCORE_LEN, col, size);
}

//******************************************************************************
void drawTimer(uint16_t x, uint16_t y, uint16_t col, uint8_t size)
//******************************************************************************
// Description: Shows or erases timer from display
// Parameters: None
//******************************************************************************
{
  LCD_DrawNumber(x, y, timerTime/60, 2, col, size);                                 //Minutes
  LCD_DrawCharacter(x+2*(NUMBER_SPRITE_WIDTH+1)*size, y, 58, col, size);            //:
  LCD_DrawNumber(x+3*(NUMBER_SPRITE_WIDTH+1)*size, y, timerTime%60, 2, col, size);  //Seconds
}

//******************************************************************************
void updateTimer(void)
//******************************************************************************
// Description: Update timer from display
// Parameters: None
//******************************************************************************
{
  drawTimer(TIMER_X_ORIGIN, TIMER_Y_ORIGIN, SCORE_MENU_BACKGROUND_COLOR, SCORE_MENU_TEXT_SIZE);
  timerTime++;
  drawTimer(TIMER_X_ORIGIN, TIMER_Y_ORIGIN, SCORE_MENU_TEXT_COLOR, SCORE_MENU_TEXT_SIZE);
}

//******************************************************************************
void resetTimer(void)
//******************************************************************************
// Description: Reset timer time and show on display
// Parameters: Timer start time in seconds
//******************************************************************************
{
  timerTime = 0;
}

//******************************************************************************
void repairBoard(void)
//******************************************************************************
// Description: Redraws last rows from the board 
// Parameters: None
//******************************************************************************
{
  for(uint16_t i = BOARD_VERTICAL_SPACES - GUIDE_LINE_LENGTH/(2*BALL_LAYER_DISTANCE); i<BOARD_VERTICAL_SPACES-1; i++){
    for(uint16_t j = 0; j<BOARD_HORIZONTAL_SPACES-i%2; j++){
      if(gameBoard[i][j].color != GAME_BACKGROUND_COLOR) LCD_DrawCircle((j*2+1+i%2)*BALL_RADIUS, (i*2+1)*BALL_LAYER_DISTANCE, BALL_RADIUS, gameBoard[i][j].color);
    }
  }
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

//******************************************************************************
void mainMenuInit(void)
//******************************************************************************
// Description: Level finished
// Parameters: None
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR); //Clear screen
  LCD_DrawRectangle(SCREEN_WIDTH/10, SCREEN_HEIGHT/20, 8*SCREEN_WIDTH/10, 18*SCREEN_HEIGHT/20, MENU_COLOR);

  LCD_DrawRectangle(SCREEN_WIDTH/20, SCREEN_HEIGHT/20, 18*SCREEN_WIDTH/20, 3*SCREEN_HEIGHT/20, GREY_COLOR);

  LCD_DrawString(SCREEN_WIDTH/2-6*3*7/2, SCREEN_HEIGHT/8+3*8/2, "POP IT", randomColor(), 3);

  drawButton(PLAY);
  drawButton(SETTINGS);
  drawButton(QUIT);

  setCurrentButton(PLAY);
}

//******************************************************************************
void settingsInit(void)
//******************************************************************************
// Description: Level finished
// Parameters: None
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR); //Clear screen
  LCD_DrawRectangle(SCREEN_WIDTH/10, SCREEN_HEIGHT/20, 8*SCREEN_WIDTH/10, 18*SCREEN_HEIGHT/20, MENU_COLOR);

  LCD_DrawRectangle(SCREEN_WIDTH/20, SCREEN_HEIGHT/20, 18*SCREEN_WIDTH/20, 3*SCREEN_HEIGHT/20, GREY_COLOR);

  LCD_DrawString(SCREEN_WIDTH/2-8*3*7/2, SCREEN_HEIGHT/8+3*8/2, "SETTINGS", WHITE_COLOR, 3);

  drawButton(SOUND_ENABLED);
  drawButton(MAIN_MENU);

  setCurrentButton(SOUND_ENABLED);
}

//******************************************************************************
void pauseInit(void)
//******************************************************************************
// Description: Level finished
// Parameters: None
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR); //Clear screen
  LCD_DrawRectangle(SCREEN_WIDTH/10, SCREEN_HEIGHT/20, 8*SCREEN_WIDTH/10, 18*SCREEN_HEIGHT/20, MENU_COLOR);

  LCD_DrawRectangle(SCREEN_WIDTH/20, SCREEN_HEIGHT/20, 18*SCREEN_WIDTH/20, 3*SCREEN_HEIGHT/20, GREY_COLOR);

  LCD_DrawString(SCREEN_WIDTH/2-5*3*7/2, SCREEN_HEIGHT/8+3*8/2, "PAUSE", WHITE_COLOR, 3);

  drawButton(RESUME);
  drawButton(MAIN_MENU);

  setCurrentButton(RESUME);
}

//******************************************************************************
void gameOverInit(void)
//******************************************************************************
// Description: Level finished
// Parameters: None
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR); //Clear screen
  LCD_DrawRectangle(SCREEN_WIDTH/10, SCREEN_HEIGHT/20, 8*SCREEN_WIDTH/10, 18*SCREEN_HEIGHT/20, MENU_COLOR);
  
  drawScore(ENDSCORE_X_ORIGIN, ENDSCORE_Y_ORIGIN, FINAL_SCORE_COLOR, FINAL_SCORE_MENU_TEXT_SIZE);
  drawTimer(ENDTIME_X_ORIGIN,ENDTIME_Y_ORIGIN, FINAL_SCORE_COLOR, FINAL_SCORE_MENU_TEXT_SIZE);

  LCD_DrawRectangle(SCREEN_WIDTH/20, SCREEN_HEIGHT/20, 18*SCREEN_WIDTH/20, 3*SCREEN_HEIGHT/20, GREY_COLOR);

  LCD_DrawString(SCREEN_WIDTH/2-9*3*7/2, SCREEN_HEIGHT/8+3*8/2, "GAME OVER", WHITE_COLOR, 3);

  drawButton(MAIN_MENU);

  setCurrentButton(MAIN_MENU);
}

//******************************************************************************
void levelFinishedInit(void)
//******************************************************************************
// Description: Level finished
// Parameters: None
//******************************************************************************
{
  LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GAME_BACKGROUND_COLOR); //Clear screen
  LCD_DrawRectangle(SCREEN_WIDTH/10, SCREEN_HEIGHT/20, 8*SCREEN_WIDTH/10, 18*SCREEN_HEIGHT/20, MENU_COLOR);
  
  drawScore(ENDSCORE_X_ORIGIN, ENDSCORE_Y_ORIGIN, FINAL_SCORE_COLOR, FINAL_SCORE_MENU_TEXT_SIZE);
  drawTimer(ENDTIME_X_ORIGIN,ENDTIME_Y_ORIGIN, FINAL_SCORE_COLOR, FINAL_SCORE_MENU_TEXT_SIZE);

  LCD_DrawRectangle(SCREEN_WIDTH/20, SCREEN_HEIGHT/20, 18*SCREEN_WIDTH/20, 3*SCREEN_HEIGHT/20, GREY_COLOR);

  LCD_DrawString(SCREEN_WIDTH/2-7*3*7/2, SCREEN_HEIGHT/8+3*8/2, "YOU WIN", WHITE_COLOR, 3);

  drawButton(NEXT_LEVEL);
  drawButton(MAIN_MENU);

  setCurrentButton(NEXT_LEVEL);
}

//******************************************************************************
void drawButton(tMenuButton button)
//******************************************************************************
// Description: Draws a line around the button currently selected
// Parameters: None
//******************************************************************************
{
  uint8_t buttonPos;
  uint8_t textLen;
  char text[11];

  switch(button){
    case PLAY:
      strcpy(text, "PLAY");
      textLen = 4;
      buttonPos=0;
      break;
    case SOUND_ENABLED:
      strcpy(text, (getSoundEnabled())?"SOUND ON":"SOUND OFF");
      textLen = (getSoundEnabled())?8:9;
      buttonPos=0;
      break;
    case SETTINGS:
      strcpy(text, "SETTINGS");
      textLen = 8;
      buttonPos=1;
      break;
    case RESUME:
      strcpy(text, "RESUME");
      textLen = 6;
      buttonPos=2;
      break;
    case NEXT_LEVEL:
      strcpy(text, "NEXT LEVEL");
      textLen = 10;
      buttonPos=2;
      break;
    case QUIT:
      strcpy(text, "QUIT");
      textLen = 4;
      buttonPos=3;
      break;
    case MAIN_MENU:
      strcpy(text, "MAIN MENU");
      textLen = 9;
      buttonPos=3;
      break;
    default:
      return;
  }

  LCD_DrawRectangle(BUTTONS_X_ORIGIN, BUTTONS_Y_ORIGIN+buttonPos*BUTTON_HEIGHT+buttonPos*BUTTON_DISTANCE, BUTTON_WIDTH, BUTTON_HEIGHT, BUTTON_COLOR);

  LCD_DrawString(
    BUTTONS_X_ORIGIN+BUTTON_WIDTH/2-7*BUTTON_TEXT_SIZE*textLen/2, 
    BUTTONS_Y_ORIGIN+(2*buttonPos+1)*BUTTON_HEIGHT/2+buttonPos*BUTTON_DISTANCE+4*BUTTON_TEXT_SIZE, 
    text, 
    BUTTON_TEXT_COLOR, 
    BUTTON_TEXT_SIZE
  );
}

//******************************************************************************
void drawButtonHighlight(uint16_t col)
//******************************************************************************
// Description: Draws a line around the button currently selected
// Parameters: None
//******************************************************************************
{
  uint8_t buttonPos;

  switch(currentButton){
    case PLAY:
    case SOUND_ENABLED:
      buttonPos=0;
      break;
    case SETTINGS:
      buttonPos=1;
      break;
    case RESUME:
    case NEXT_LEVEL:
      buttonPos=2;
      break;
    case QUIT:
    case MAIN_MENU:
      buttonPos=3;
      break;
    default:
      return;
  }
  LCD_DrawRectangle(BUTTONS_X_ORIGIN-BUTTON_HIGHLIGHT_WIDTH, BUTTONS_Y_ORIGIN+buttonPos*BUTTON_HEIGHT+buttonPos*BUTTON_DISTANCE-BUTTON_HIGHLIGHT_WIDTH, BUTTON_WIDTH+2*BUTTON_HIGHLIGHT_WIDTH, BUTTON_HIGHLIGHT_WIDTH                       , col);
  LCD_DrawRectangle(BUTTONS_X_ORIGIN-BUTTON_HIGHLIGHT_WIDTH, BUTTONS_Y_ORIGIN+buttonPos*BUTTON_HEIGHT+buttonPos*BUTTON_DISTANCE+BUTTON_HEIGHT         , BUTTON_WIDTH+2*BUTTON_HIGHLIGHT_WIDTH, BUTTON_HIGHLIGHT_WIDTH                       , col);
  LCD_DrawRectangle(BUTTONS_X_ORIGIN-BUTTON_HIGHLIGHT_WIDTH, BUTTONS_Y_ORIGIN+buttonPos*BUTTON_HEIGHT+buttonPos*BUTTON_DISTANCE-BUTTON_HIGHLIGHT_WIDTH, BUTTON_HIGHLIGHT_WIDTH               , BUTTON_HEIGHT+2*BUTTON_HIGHLIGHT_WIDTH       , col);
  LCD_DrawRectangle(BUTTONS_X_ORIGIN+BUTTON_WIDTH          , BUTTONS_Y_ORIGIN+buttonPos*BUTTON_HEIGHT+buttonPos*BUTTON_DISTANCE-BUTTON_HIGHLIGHT_WIDTH, BUTTON_HIGHLIGHT_WIDTH               , BUTTON_HEIGHT+2*BUTTON_HIGHLIGHT_WIDTH       , col);
}

//******************************************************************************
void selectButton(void)
//******************************************************************************
// Description: Set current button selected
// Parameters: None
//******************************************************************************
{
  switch(currentButton){
    case PLAY:
      setCurrentMenu(GAME_MENU);
      break;
    case SOUND_ENABLED:
      setSoundEnabled(!getSoundEnabled());
      drawButton(SOUND_ENABLED);
      break;
    case SETTINGS:
      setCurrentMenu(SETTINGS_MENU);
      break;
    case RESUME:
      setCurrentMenu(GAME_MENU);
    case NEXT_LEVEL:
      resetTimer();
      currentLevel++;
      setCurrentMenu(GAME_MENU);
      break;
    case QUIT:
      // Apagar
      LCD_DrawRectangle(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,0);
      DisablePower();
      SysSleep(500);
    case MAIN_MENU:
      resetTimer();
      ballsOnBoard = 0;
      gameScore = 0;
      setCurrentMenu(START_MENU);
      break;
    default:
      return;
  }
}

//******************************************************************************
uint16_t getBallsOnBoard(void)
//******************************************************************************
// Description: Returns the number of balls on the board
// Parameters: None
//******************************************************************************
{
  return ballsOnBoard;
}

//******************************************************************************
uint8_t getMoving(void)
//******************************************************************************
// Description: Returns the number of balls on the board
// Parameters: None
//******************************************************************************
{
  return moving;
}

//******************************************************************************
tMenuButton getCurrentButton(void)
//******************************************************************************
// Description: Returns the current button selected
// Parameters: None
//******************************************************************************
{
  return currentButton;
}

//******************************************************************************
void setCurrentButton(tMenuButton button)
//******************************************************************************
// Description: Set current button selected
// Parameters: None
//******************************************************************************
{
  drawButtonHighlight(MENU_COLOR);
  currentButton = button;
  drawButtonHighlight(BUTTON_HIGHLIGHT_COLOR);
}