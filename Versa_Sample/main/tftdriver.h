//******************************************************************************
// File: tftdriver.h
// Description: driver for the ILI9341 240x320 screen
// Written by: Juan Ramon Vadillo VERSA DESIGN S.L.
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

#ifndef TFTDRIVER_H_INCLUDED
#define TFTDRIVER_H_INCLUDED

//******************************************************************************
// INCLUDES

#include <stdint.h>
#include "pcbdefs.h"

//******************************************************************************
// DEFINES & TYPEDEFS

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

#define GET_RED_FROM_RGB565(c)  ((c >> 8) & 0xF8)
#define GET_GREEN_FROM_RGB565(c) ((c >> 3) & 0xFC)
#define GET_BLUE_FROM_RGB565(c) ((c << 3) & 0xFC)


#define RGB565(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | \
                         (((b) & 0xF8) >> 3))

#define ILI9341_CMD_NOP                             0x00
#define ILI9341_CMD_SOFTWARE_RESET                  0x01
#define ILI9341_CMD_READ_DISP_ID                    0x04
#define ILI9341_CMD_READ_DISP_STATUS                0x09
#define ILI9341_CMD_READ_DISP_MADCTRL               0x0B
#define ILI9341_CMD_READ_DISP_PIXEL_FORMAT          0x0C
#define ILI9341_CMD_READ_DISP_IMAGE_FORMAT          0x0D
#define ILI9341_CMD_READ_DISP_SIGNAL_MODE           0x0E
#define ILI9341_CMD_READ_DISP_SELF_DIAGNOSTIC       0x0F
#define ILI9341_CMD_ENTER_SLEEP_MODE                0x10
#define ILI9341_CMD_SLEEP_OUT                       0x11
#define ILI9341_CMD_PARTIAL_MODE_ON                 0x12
#define ILI9341_CMD_NORMAL_DISP_MODE_ON             0x13
#define ILI9341_CMD_DISP_INVERSION_OFF              0x20
#define ILI9341_CMD_DISP_INVERSION_ON               0x21
#define ILI9341_CMD_GAMMA_SET                       0x26
#define ILI9341_CMD_DISPLAY_OFF                     0x28
#define ILI9341_CMD_DISPLAY_ON                      0x29
#define ILI9341_CMD_COLUMN_ADDRESS_SET              0x2A
#define ILI9341_CMD_PAGE_ADDRESS_SET                0x2B
#define ILI9341_CMD_MEMORY_WRITE                    0x2C
#define ILI9341_CMD_COLOR_SET                       0x2D
#define ILI9341_CMD_MEMORY_READ                     0x2E
#define ILI9341_CMD_PARTIAL_AREA                    0x30
#define ILI9341_CMD_VERT_SCROLL_DEFINITION          0x33
#define ILI9341_CMD_TEARING_EFFECT_LINE_OFF         0x34
#define ILI9341_CMD_TEARING_EFFECT_LINE_ON          0x35
#define ILI9341_CMD_MEMORY_ACCESS_CONTROL           0x36
#define ILI9341_CMD_VERT_SCROLL_START_ADDRESS       0x37
#define ILI9341_CMD_IDLE_MODE_OFF                   0x38
#define ILI9341_CMD_IDLE_MODE_ON                    0x39
#define ILI9341_CMD_COLMOD_PIXEL_FORMAT_SET         0x3A
#define ILI9341_CMD_WRITE_MEMORY_CONTINUE           0x3C
#define ILI9341_CMD_READ_MEMORY_CONTINUE            0x3E
#define ILI9341_CMD_SET_TEAR_SCANLINE               0x44
#define ILI9341_CMD_GET_SCANLINE                    0x45
#define ILI9341_CMD_WRITE_DISPLAY_BRIGHTNESS        0x51
#define ILI9341_CMD_READ_DISPLAY_BRIGHTNESS         0x52
#define ILI9341_CMD_WRITE_CTRL_DISPLAY              0x53
#define ILI9341_CMD_READ_CTRL_DISPLAY               0x54
#define ILI9341_CMD_WRITE_CONTENT_ADAPT_BRIGHTNESS  0x55
#define ILI9341_CMD_READ_CONTENT_ADAPT_BRIGHTNESS   0x56
#define ILI9341_CMD_WRITE_MIN_CAB_LEVEL             0x5E
#define ILI9341_CMD_READ_MIN_CAB_LEVEL              0x5F
#define ILI9341_CMD_READ_ID1                        0xDA
#define ILI9341_CMD_READ_ID2                        0xDB
#define ILI9341_CMD_READ_ID3                        0xDC

/* Level 2 Commands (from the display Datasheet) */
#define ILI9341_CMD_RGB_SIGNAL_CONTROL              0xB0
#define ILI9341_CMD_FRAME_RATE_CONTROL_NORMAL       0xB1
#define ILI9341_CMD_FRAME_RATE_CONTROL_IDLE_8COLOR  0xB2
#define ILI9341_CMD_FRAME_RATE_CONTROL_PARTIAL      0xB3
#define ILI9341_CMD_DISPLAY_INVERSION_CONTROL       0xB4
#define ILI9341_CMD_BLANKING_PORCH_CONTROL          0xB5
#define ILI9341_CMD_DISPLAY_FUNCTION_CONTROL        0xB6
#define ILI9341_CMD_ENTRY_MODE_SET                  0xB7
#define ILI9341_CMD_BACKLIGHT_CONTROL_1             0xB8
#define ILI9341_CMD_BACKLIGHT_CONTROL_2             0xB9
#define ILI9341_CMD_BACKLIGHT_CONTROL_3             0xBA
#define ILI9341_CMD_BACKLIGHT_CONTROL_4             0xBB
#define ILI9341_CMD_BACKLIGHT_CONTROL_5             0xBC
#define ILI9341_CMD_BACKLIGHT_CONTROL_6             0xBD
#define ILI9341_CMD_BACKLIGHT_CONTROL_7             0xBE
#define ILI9341_CMD_BACKLIGHT_CONTROL_8             0xBF
#define ILI9341_CMD_POWER_CONTROL_1                 0xC0
#define ILI9341_CMD_POWER_CONTROL_2                 0xC1
#define ILI9341_CMD_VCOM_CONTROL_1                  0xC5
#define ILI9341_CMD_VCOM_CONTROL_2                  0xC7
#define ILI9341_CMD_POWER_ON_SEQ_CONTROL            0xCB
#define ILI9341_CMD_POWER_CONTROL_A                 0xCD
#define ILI9341_CMD_POWER_CONTROL_B                 0xCF
#define ILI9341_CMD_NVMEM_WRITE                     0xD0
#define ILI9341_CMD_NVMEM_PROTECTION_KEY            0xD1
#define ILI9341_CMD_NVMEM_STATUS_READ               0xD2
#define ILI9341_CMD_READ_ID4                        0xD3
#define ILI9341_CMD_POSITIVE_GAMMA_CORRECTION       0xE0
#define ILI9341_CMD_NEGATIVE_GAMMA_CORRECTION       0xE1
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_1         0xE2
#define ILI9341_CMD_DIGITAL_GAMMA_CONTROL_2         0xE3
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_A         0xE8
#define ILI9341_CMD_DRIVER_TIMING_CONTROL_B         0xEA
#define ILI9341_CMD_ENABLE_3_GAMMA_CONTROL          0xF2
#define ILI9341_CMD_INTERFACE_CONTROL               0xF6
#define ILI9341_CMD_PUMP_RATIO_CONTROL              0xF7


#define TFT_WR_HI()        (GPIO.out_w1ts = (1 << GPIO_LCD_WR))
#define TFT_WR_LO()        (GPIO.out_w1tc = (1 << GPIO_LCD_WR))
#define TFT_RD_HI()        (GPIO.out_w1ts = (1 << GPIO_LCD_RD))
#define TFT_RD_LO()        (GPIO.out_w1tc = (1 << GPIO_LCD_RD))
#define TFT_RS_LO()        (GPIO.out_w1tc = (1 << GPIO_LCD_DC))
#define TFT_RS_HI()        (GPIO.out_w1ts = (1 << GPIO_LCD_DC))
#define TFT_CS_LO()        (GPIO.out_w1tc = (1 << GPIO_LCD_CS))
#define TFT_CS_HI()        (GPIO.out_w1ts = (1 << GPIO_LCD_CS))
#define TFT_RESET_LO()    (GPIO.out_w1tc = (1 << GPIO_LCD_RESET))
#define TFT_RESET_HI()    (GPIO.out_w1ts = (1 << GPIO_LCD_RESET))

//******************************************************************************
// FUNCTIONS

void TFT_Init(void);

void PutPixelHiCol(uint16_t x, uint16_t y, uint16_t col);
void LCD_DrawRectangle(uint16_t x, uint16_t y, uint16_t sx, uint16_t sy, uint16_t col);
void LCD_DrawLine(uint16_t x, uint16_t y, uint16_t sx, uint16_t sy, uint16_t col);
void LCD_DrawDashedLine(uint16_t x1,  uint16_t y1, uint16_t x2, uint16_t y2, uint8_t dashLen, uint8_t gapLen, uint16_t col);
void LCD_DrawPolarLine(uint16_t x,  uint16_t y, uint16_t r,  float phi, uint16_t col);
void LCD_DrawDashedPolarLine(uint16_t x,  uint16_t y, uint16_t r,  float phi, uint8_t dashLen, uint8_t gapLen, uint16_t col, uint16_t originOffset);
void LCD_DrawCircle(uint16_t x,  uint16_t y, uint16_t r, uint16_t col);
void LCD_DrawCharacter(uint16_t x,  uint16_t y, uint8_t num, uint16_t col, uint8_t size);
void LCD_DrawNumber(uint16_t x,  uint16_t y, uint32_t num, uint8_t numLen, uint16_t col, uint8_t size);
void LCD_DrawString(uint16_t x,  uint16_t y, char *string, uint16_t col, uint8_t size);

// Backlight
void TFT_BacklightManage(void);
void TFT_SetBacklight(uint16_t pct);

#endif
