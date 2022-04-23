#include "lcd1602.h"
//############################################################
#define _LCD_COLS         16
#define _LCD_ROWS         2
//############################################################
/*****  GPIOB3  H:data;L:command  ******/
#define  LCD1602_RS_DATA	   		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_SET)		
#define  LCD1602_RS_CMD					HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,GPIO_PIN_RESET)
/*****  GPIOB4  H:read;L:write  ******/
#define  LCD1602_RW_READ        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_SET)
#define  LCD1602_RW_WRITE       HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,GPIO_PIN_RESET)
/*****  GPIOB5    ******/
#define  LCD1602_E_HIGH         HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET)
#define  LCD1602_E_LOW          HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET)

#define  LCD1602_DATA(x)				GPIOB->ODR=(GPIOB->ODR & 0x00ff) | (x<<8)
//############################################################
/* Commands*/
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80
/* Flags for display entry mode */
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
/* Flags for display on/off control */
#define LCD_DISPLAYON           0x04
#define LCD_CURSORON            0x02
#define LCD_BLINKON             0x01
/* Flags for display/cursor shift */
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00
/* Flags for function set */
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00
//############################################################
//############################################################
typedef struct {
uint8_t DisplayControl;
uint8_t DisplayFunction;
uint8_t DisplayMode;
uint8_t currentX;
uint8_t currentY;
} LCD_Options_t;
//############################################################
/* Private functions */
static void LCD_Cmd(uint8_t cmd);
static void LCD_Data(uint8_t data);
static void LCD_CursorSet(uint8_t col, uint8_t row);
static void LCD_EnBlink(void);
//############################################################
/* Private variable */
static LCD_Options_t LCD_Opts;
//us级Delay
void  LCD_Delay_us(uint16_t  us)
{
    uint32_t  Div = (SysTick->LOAD+1)/1000;
    uint32_t  StartMicros = HAL_GetTick()*1000 + (1000- SysTick->VAL/Div);
    while((HAL_GetTick()*1000 + (1000-SysTick->VAL/Div)-StartMicros < us));
}
//ms级Delay
void  LCD_Delay_ms(uint8_t  ms)
{
    HAL_Delay(ms);
}
//LCD初始化
void LCD_Init(void)
{
  /* Set cursor pointer to beginning for LCD */
    LCD_Opts.currentX = 0;
    LCD_Opts.currentY = 0;
    LCD_Opts.DisplayFunction = LCD_8BITMODE | LCD_5x8DOTS | LCD_1LINE;
    if (_LCD_ROWS > 1)
    LCD_Opts.DisplayFunction |= LCD_2LINE;
  /* Set # lines, font size,8BITMODE,etc. */
    LCD_Cmd(LCD_FUNCTIONSET | LCD_Opts.DisplayFunction);
  /* Turn the display on with no cursor or blinking default */
    LCD_Opts.DisplayControl = LCD_DISPLAYON;
    LCD_DisplayOn();
    /* Default font directions 文字不动，地址自动+1*/
    LCD_Opts.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    LCD_Cmd(LCD_ENTRYMODESET | LCD_Opts.DisplayMode);
    LCD_Clear();
    LCD_Delay_ms(5);
}
//清屏
void LCD_Clear(void)
{
    LCD_Cmd(LCD_CLEARDISPLAY);
    LCD_Delay_ms(5);
}
//显示一个string,支持换行
void LCD_Puts(uint8_t x, uint8_t y, char* str)
{
    LCD_CursorSet(x, y);
    while (*str) {
        if (LCD_Opts.currentX >= _LCD_COLS)/*移动到下一行*/ {
            LCD_Opts.currentX = 0;
            LCD_Opts.currentY++;
            LCD_CursorSet(LCD_Opts.currentX, LCD_Opts.currentY);
        }
        if (*str == '\n') /*换行，列对其*/{
            LCD_Opts.currentY++;
            LCD_CursorSet(LCD_Opts.currentX, LCD_Opts.currentY);
        } else if (*str == '\r') /*回车，换行回行首*/{
            LCD_CursorSet(0, LCD_Opts.currentY);
        } else {
            LCD_Data(*str);
            LCD_Opts.currentX++;
        }
        str++;
    }
}
//############################################################
void LCD_DisplayOn(void)
{
    LCD_Opts.DisplayControl |= LCD_DISPLAYON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}
//############################################################
void LCD_DisplayOff(void)
{
    LCD_Opts.DisplayControl &= ~LCD_DISPLAYON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}
//############################################################
void LCD_BlinkOn(void)
{
    LCD_Opts.DisplayControl |= LCD_BLINKON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}
//############################################################
void LCD_BlinkOff(void)
{
    LCD_Opts.DisplayControl &= ~LCD_BLINKON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}
//############################################################
void LCD_CursorOn(void)
{
    LCD_Opts.DisplayControl |= LCD_CURSORON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}
//############################################################
void LCD_CursorOff(void)
{
    LCD_Opts.DisplayControl &= ~LCD_CURSORON;
    LCD_Cmd(LCD_DISPLAYCONTROL | LCD_Opts.DisplayControl);
}
//############################################################
void LCD_ScrollLeft(void)
{
    LCD_Cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
//############################################################
void LCD_ScrollRight(void)
{
    LCD_Cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
//############################################################
void LCD_CreateChar(uint8_t location, uint8_t *data)
{
    uint8_t i;
  /* We have 8 locations available for custom characters */
    location &= 0x07;
    LCD_Cmd(LCD_SETCGRAMADDR | (location << 3));

    for (i = 0; i < 8; i++) {
        LCD_Data(data[i]);
    }
}
//############################################################
void LCD_PutCustom(uint8_t x, uint8_t y, uint8_t location)
{
    LCD_CursorSet(x, y);
    LCD_Data(location);
}
//############################################################
static void LCD_EnBlink(void)
{
    LCD1602_E_HIGH;
    LCD_Delay_us(50);
    LCD1602_E_LOW;
    LCD_Delay_us(50);
}
//############################################################
static void LCD_Cmd(uint8_t cmd)
{
    LCD1602_RS_CMD;
    LCD1602_RW_WRITE;
    LCD1602_DATA(cmd);
    LCD_EnBlink();
}
//############################################################
static void LCD_Data(uint8_t data)
{
    LCD1602_RS_DATA;
    LCD1602_RW_WRITE;
    LCD1602_DATA(data);
    LCD_EnBlink();
}
//############################################################
static void LCD_CursorSet(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (row >= _LCD_ROWS)
        row = 0;
    LCD_Opts.currentX = col;
    LCD_Opts.currentY = row;
    LCD_Cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
//############################################################
void LCD_Put(uint8_t Data)
{
    LCD_Data(Data);
}
//############################################################
/********************************End of File************************************/
