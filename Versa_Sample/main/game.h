#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

//******************************************************************************
// DEFINES & TYPEDEFS


#define GUIDE_LINE_LENGTH           26
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

#define BALLS_ON_BOARD              40
#define CLUSTER_MIN_SIZE            3

#define SCORE_LEN                   10
#define SCORE_X_ORIGIN              SCREEN_WIDTH*3/4-(NUMBER_SPRITE_WIDTH+1)*SCORE_LEN/2
#define SCORE_Y_ORIGIN              SCREEN_HEIGHT-BALL_RADIUS+NUMBER_SPRITE_HEIGHT/2

#define TIMER_LEN 5
#define TIMER_X_ORIGIN              SCREEN_WIDTH/4-(NUMBER_SPRITE_WIDTH+1)*TIMER_LEN/2
#define TIMER_Y_ORIGIN              SCREEN_HEIGHT-BALL_RADIUS+NUMBER_SPRITE_HEIGHT/2

#define GAME_OVER_LINE_HEIGHT       BALL_RADIUS + BALL_LAYER_DISTANCE*2*(BOARD_VERTICAL_SPACES-1)
#define GAME_OVER_LINE_DASH_LENGTH  4
#define GAME_OVER_LINE_GAP_LENGTH   8

#define GAME_OVER_LINE_COLOR        DARK_GREY_COLOR
#define GAME_BACKGROUND_COLOR       BLACK_COLOR
#define SCORE_MENU_BACKGROUND_COLOR LIGHT_GREY_COLOR
#define SCORE_MENU_TEXT_COLOR       BLACK_COLOR

struct ball{
  uint16_t color;
  uint8_t visited;
};

uint8_t moving;

//******************************************************************************
// FUNCTIONS

uint16_t randomColor(void);
void generateBoard(void);
void displayBoard(void);
void initGame(void);
void updateBallLocation(void);
void setBall(uint16_t col);
void throwBall(float dir);
void stopBall(void);
uint16_t checkCluster(uint8_t ballLayer, uint8_t ballInLayerPos);
void recursiveClusterCheck(int8_t ballLayer, int8_t ballInLayerPos, uint16_t* clusterSize);
void drawScore(uint16_t col);
void drawTimer(uint16_t col);
void updateTimer(void);
void setTimer(uint16_t t);
void placeGameOverLine(void);
void gameOver(void);

#endif
