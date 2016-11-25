/* Common Begin */
#ifndef uint8_t
#define uint8_t unsigned char
#endif
#ifndef uint16_t
#define uint16_t unsigned short
#endif
#ifndef uint32_t
#define uint32_t unsigned int
#endif

virtual void setup();
virtual void loop();

uint32_t _millis, _micros;
uint32_t millis();
uint32_t micros();
void delay(uint32_t time);
void delayMicroseconds(uint32_t time);
/* Common End */

/* Screen Begin */
void initScreen();
void openScreen();
void closeScreen();
void setBrgiht(char value);

uint8_t _backColor, _foreColor, _fontSize;
void backColor(uint8_t color);
void foreColor(uint8_t color);
void drawPixel(char x, char y, uint8_t color);
void drawLine(char x1, char y1, char x2, char y2);
void drawTri(char x1, char y1, char x2, char y2, char x3, char y3, uint8_t fillFlag);
void drawRect(char x1, char y1, char x2, char y2, uint8_t fillFlag);
void drawBitmap(char x, char y, char w, char h, uint8_t* data);
void drawColoredBitmap(char x, char y, char w, char h, uint8_t transColor, uint8_t* data);
void gBegin();  //CS` = 0;
void gEnd();    //CS` = 1;

void fontSize(char size);
void drawChar(char x, char y, char c);
void print(char x, char y, char* format, ...)
/* Screen End */

/* Keypad Begin */
#define LPAD_UP     0x01
#define LPAD_DOWN   0x02
#define LPAD_LEFT   0x04
#define LPAD_RIGHT  0x08
#define RPAD_UP     0x10
#define RPAD_DOWN   0x20
#define RPAD_LEFT   0x40
#define RPAD_RIGHT  0x80

uint8_t checkKeys();
uint8_t waitKeyUp(char key);
/* Keypad End */

/* Flash Begin */
void fBegin();  //CS` = 0;
void fEnd();    //CS` = 1;
uint8_t read(uint32_t addr);
void write(uint32_t addr, uint8_t data);
uint8_t* load(uint32_t addr, uint32_t length);
void save(uint32_t addr, uint32_t length, uint8_t* data);
/* Flash End */

/* EEPROM Begin */

/* EEPROM End */

/* Scripting Begin */

/* Scripting End */