#ifndef ILI9341_H_
#define ILI9341_H_
// Includes



// Defines
#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define ORANGE      0xFC00
#define WHITE       0xFFFF
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F


// Variables
extern volatile uint16_t LcdVertPixel;
extern volatile uint16_t LcdHoriPixel;


// Structures



// Functions
extern void Ili9341_Reset(void);										// Initialize ILI9341
extern void Ili9341_Preset(void);										// Configure ILI9341
//extern void Ili9341_Write(uint8_t a_deCommand, uint8_t a_deData);
extern void Ili9341_Display(uint8_t a_Byte);
extern void Ili9341_Rotate(uint16_t a_Angle);
extern void Ili9341_Background(uint16_t a_Color);

extern void Ili9341_Ascii(int8_t* a_String, uint32_t a_FontSize, uint16_t a_TextColor, uint16_t a_BackgroundColor,
		uint16_t a_CoordX, uint16_t a_CoordY, uint16_t a_OffsetX, uint16_t a_OffsetY);
extern void Ili9341_Point(uint16_t a_Color, uint16_t a_Size, int16_t a_OriginX, int16_t a_OriginY);
extern void Ili9341_LineHori(uint16_t a_color, int16_t a_Length, int16_t a_OriginX, int16_t a_OriginY);
extern void Ili9341_LineVert(uint16_t a_color, int16_t a_Length, int16_t a_OriginX, int16_t a_OriginY);

extern void Ili9341_LineDiag(uint16_t a_color,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);
extern void Ili9341_LineEarc(uint16_t a_Color, int16_t a_Angle,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);

void Ili9341_HallowItri(uint16_t a_Color, int16_t a_Angle,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);
void Ili9341_HallowAtri(uint16_t a_Color, int16_t a_Angle,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);
void Ili9341_HallowRhom(uint16_t a_Color,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);
void Ili9341_HallowRect(uint16_t a_Color,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);
void Ili9341_HallowElli(uint16_t a_Color,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);

extern void Ili9341_FilledRect(uint16_t a_Color,
		int16_t a_OriginX, int16_t a_OriginY, int16_t a_OffsetX, int16_t a_OffsetY);

extern void Ili9341_Block_insert(	uint16_t a_Color,
		uint16_t u2t_column_sc, 		// Start Column
		uint16_t u2t_page_sp, 			// Start Page
		uint16_t u2t_column_ec, 		// End Column
		uint16_t u2t_page_ep,			// End Page
		uint16_t u2t_offset);


#endif /* ILI9341_H_ */
