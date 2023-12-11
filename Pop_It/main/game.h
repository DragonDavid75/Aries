#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "task_ui.h"
//******************************************************************************
// DEFINES & TYPEDEFS

#define MAX_LEVEL                   (BOARD_HORIZONTAL_SPACES*BOARD_VERTICAL_SPACES-BALLS_AT_LV_1)/(3*BALLS_PER_LEVEL/2)

#define GUIDE_LINE_LENGTH           51
#define GUIDE_LINE_COLOR            WHITE_COLOR
#define GUIDE_LINE_DASH_LENGTH      1
#define GUIDE_LINE_GAP_LENGTH       4

#define BALL_RADIUS                 8
#define BALL_SPEED                  5

#define BOARD_HORIZONTAL_SPACES     SCREEN_WIDTH/(2*BALL_RADIUS)
#define BOARD_VERTICAL_SPACES       SCREEN_HEIGHT/(2*BALL_RADIUS)

#define BALL_X_ORIGIN               SCREEN_WIDTH/2
#define BALL_Y_ORIGIN               SCREEN_HEIGHT-3*BALL_RADIUS-1
#define BALL_LAYER_DISTANCE         BALL_RADIUS*0.9

#define COLOR_NUM                   6

#define BALLS_AT_LV_1               20
#define BALLS_PER_LEVEL             10
#define CLUSTER_MIN_SIZE            3

#define SCORE_LEN                   10
#define SCORE_X_ORIGIN              SCREEN_WIDTH*3/4-(NUMBER_SPRITE_WIDTH+1)*SCORE_LEN/2
#define SCORE_Y_ORIGIN              SCREEN_HEIGHT-BALL_RADIUS+NUMBER_SPRITE_HEIGHT/2
#define ENDSCORE_X_ORIGIN           SCREEN_WIDTH/2-(NUMBER_SPRITE_WIDTH+1)*SCORE_LEN*FINAL_SCORE_MENU_TEXT_SIZE/2
#define ENDSCORE_Y_ORIGIN           SCREEN_HEIGHT*4/10
#define ENDTIME_X_ORIGIN            SCREEN_WIDTH/2 - 7*FINAL_SCORE_MENU_TEXT_SIZE*TIMER_LEN/2
#define ENDTIME_Y_ORIGIN            SCREEN_HEIGHT/2

#define SCORE_MENU_X_ORIGIN         0
#define SCORE_MENU_Y_ORIGIN         SCREEN_HEIGHT-2*BALL_RADIUS
#define SCORE_MENU_HEIGHT           2*BALL_RADIUS
#define SCORE_MENU_WIDTH            SCREEN_WIDTH

#define TIMER_LEN                   5
#define TIMER_X_ORIGIN              SCREEN_WIDTH/4-(NUMBER_SPRITE_WIDTH+1)*TIMER_LEN/2
#define TIMER_Y_ORIGIN              SCREEN_HEIGHT-BALL_RADIUS+NUMBER_SPRITE_HEIGHT/2

#define GAME_OVER_LINE_HEIGHT       BALL_RADIUS + BALL_LAYER_DISTANCE*2*(BOARD_VERTICAL_SPACES-1)
#define GAME_OVER_LINE_DASH_LENGTH  4
#define GAME_OVER_LINE_GAP_LENGTH   8

#define GAME_OVER_LINE_COLOR        DARK_GREY_COLOR
#define GAME_BACKGROUND_COLOR       BLACK_COLOR
#define SCORE_MENU_BACKGROUND_COLOR LIGHT_GREY_COLOR
#define SCORE_MENU_TEXT_COLOR       BLACK_COLOR
#define MENU_COLOR                  LIGHT_GREY_COLOR
#define FINAL_SCORE_COLOR           BLACK_COLOR
#define BUTTON_COLOR                GREY_COLOR
#define BUTTON_TEXT_COLOR           WHITE_COLOR
#define BUTTON_HIGHLIGHT_COLOR      WHITE_COLOR
#define BACK_WALL_COLOR             DARK_GREY_COLOR

#define SCORE_MENU_TEXT_SIZE        1
#define FINAL_SCORE_MENU_TEXT_SIZE  2

#define BUTTONS_X_ORIGIN            2*SCREEN_WIDTH/10
#define BUTTONS_Y_ORIGIN            6*SCREEN_HEIGHT/20
#define BUTTON_HEIGHT               SCREEN_HEIGHT/10
#define BUTTON_WIDTH                6*SCREEN_WIDTH/10
#define BUTTON_DISTANCE             SCREEN_HEIGHT/20
#define BUTTON_HIGHLIGHT_WIDTH      2
#define BUTTON_TEXT_SIZE            2

struct ball{
  uint16_t color;
  bool visited;
};

typedef enum
{
  START_MENU,
  SETTINGS_MENU,
  PAUSE_MENU,
  GAME_MENU,
  GAME_OVER_MENU,
  GAME_FINISHED_MENU
} tMenu;

typedef enum
{
  PLAY,
  SETTINGS, 
  QUIT, 
  SOUND_ENABLED, 
  RESUME, 
  NEXT_LEVEL, 
  MAIN_MENU, 
  NONE
} tMenuButton;

bool moving;

//******************************************************************************
// FUNCTIONS

void mainMenuInit(void);
void settingsInit(void);
void pauseInit(void);
void gameInit(void);
void gameOverInit(void);
void levelFinishedInit(void);
uint16_t randomColor(void);
void generateBoard(void);
void displayBoard(void);
void drawBackWall(void);
void moveBoardDown(void);
void updateBallLocation(void);
void setBall(uint16_t col);
void setBallDirection(tUIEvent button);
void throwBall(void);
void collition(void);
uint16_t checkUnconectedBall();
void recursiveUnconectedBallCheck(int8_t ballLayer, int8_t ballInLayerPos);
uint16_t checkCluster(uint8_t ballLayer, uint8_t ballInLayerPos);
void recursiveClusterCheck(int8_t ballLayer, int8_t ballInLayerPos, uint16_t* clusterSize);
void drawScore(uint16_t x, uint16_t y, uint16_t col, uint8_t size);
void drawTimer(uint16_t x, uint16_t y, uint16_t col, uint8_t size);
void updateTimer(void);
void resetTimer(void);
void repairBoard(void);
void placeGameOverLine(void);
void drawButton(tMenuButton button);
void drawButtonHighlight(uint16_t col);
void selectButton(void);
uint16_t getBallsOnBoard(void);
uint8_t getMoving(void);
tMenuButton getCurrentButton(void);
void setCurrentButton(tMenuButton button);

#endif
