//lcd.h

#ifndef LCD_H_
#define LCD_H_

void init_GPIO_LCD();
void init_LCD();
void clear_LCD();
void putchar_LCD(char c);
void putcmd_LCD(unsigned char c, unsigned int i);
void puts_LCD(char *c);
void setpos_LCD(unsigned int m, unsigned int n);

#endif /* LCD_H_ */
