//******************************************************************************
// File: global.h
// Description: Global definitions
// Author:
//  Francisco Jose Fabra Collado (VERSA DESIGN S.L.)
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

#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

//******************************************************************************
// INCLUDES

#include <stdint.h>

//******************************************************************************
// VARIABLES & DEFINES

#define FW_VERSION_HIGH   1 // v1.01
#define FW_VERSION_LOW    1

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef PI
#define PI 3.1415
#endif

#define BLACK_COLOR         RGB565(0, 0, 0)
#define RED_COLOR           RGB565(255, 0, 0)
#define GREEN_COLOR         RGB565(0, 255, 0)
#define BLUE_COLOR          RGB565(0, 0, 255)
#define YELLOW_COLOR        RGB565(255, 255, 0)
#define CYAN_COLOR          RGB565(0, 255, 255)
#define VIOLET_COLOR        RGB565(255, 0, 255)
#define LIGHT_GREY_COLOR    RGB565(191, 191, 191)
#define GREY_COLOR          RGB565(127, 127, 127)
#define DARK_GREY_COLOR     RGB565(63, 63, 63)
#define WHITE_COLOR         RGB565(255, 255, 255)

#define NUMBER_SPRITE_HEIGHT 8
#define NUMBER_SPRITE_WIDTH 6

typedef enum
{
  RES_OK = 0,
  RES_ERROR = 1,
  RES_ERROR_TIMEOUT = 2,
  RES_ERROR_PARAM =  3,
  RES_ERROR_MUTEX = 4,
  RES_ERROR_LENGTH = 5,
  RES_ERROR_WRITE = 6,
  RES_ERROR_CHECK = 7,
  RES_ERROR_ACK = 8,
  RES_ERROR_NO_MEMO = 9,
  RES_RETRY = 10
} tRetCode;

// Definition for an invalid number in UINT32
#ifndef U32_INVALID
#define U32_INVALID (0xFFFFFFFFL)
#endif

//******************************************************************************
// FUNCTIONS

#endif
