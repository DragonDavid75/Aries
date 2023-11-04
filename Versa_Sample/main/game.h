//******************************************************************************
// DEFINES & TYPEDEFS

#define GUIDE_LINE_LENGTH 20
#define BALL_RADIUS 8
#define BALL_SPEED 5

#define BOARD_HORIZONTAL_SPACES SCREEN_WIDTH/(2*BALL_RADIUS)-1
#define BOARD_VERTICAL_SPACES   SCREEN_HEIGHT/(2*BALL_RADIUS)

#define BALL_X_ORIGIN           SCREEN_WIDTH/2
#define BALL_Y_ORIGIN           SCREEN_HEIGHT-BALL_RADIUS-1
#define BALL_LAYER_DISTANCE     BALL_RADIUS*0.9

#define COLOR_NUM 6

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
void updateBallLocation(void);
void throwBall(float dir, uint16_t col);
void stopBall(void);

//void setBallParam(uint16_t xStartPos, uint16_t yStartPos, uint16_t r, uint16_t movSpeed, uint16_t color);