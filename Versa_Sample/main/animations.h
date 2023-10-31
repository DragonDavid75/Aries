//------------------------------------------------------------------------------
// Local variables

//Ball variables
static uint16_t ballRadius;
static uint16_t ballColor;

uint16_t start_xPosition;
uint16_t start_yPosition;
uint16_t xPosition;
uint16_t yPosition;

static uint16_t speed;
static float direction;

//******************************************************************************
// FUNCTIONS

void setBallParam(uint16_t xStartPos, uint16_t yStartPos, uint16_t r, uint16_t movSpeed, uint16_t color);
void updateBallLocation(void);
void throwBall(float dir);
void stopBall(void);