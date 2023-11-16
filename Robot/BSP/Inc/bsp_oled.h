#ifndef __OLED_H
#define __OLED_H
#include "sys.h"
#include "stdlib.h"
// OLEDģʽ����
// 0:4�ߴ���ģʽ
// 1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

//-----------------OLED�˿ڶ���----------------
#define OLED_SCLK_Clr() PDout(7) = 0 // SCL
#define OLED_SCLK_Set() PDout(7) = 1 // SCL

#define OLED_SDIN_Clr() PDout(6) = 0 // SDA
#define OLED_SDIN_Set() PDout(6) = 1 // SDA

#define OLED_RST_Clr() PDout(5) = 0
#define OLED_RST_Set() PDout(5) = 1

#define OLED_DC_Clr() PDout(4) = 0 // DC
#define OLED_DC_Set() PDout(4) = 1 // DC

#define OLED_CMD 0  // д����
#define OLED_DATA 1 // д����

// OLED�����ú���
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *p);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
#endif
