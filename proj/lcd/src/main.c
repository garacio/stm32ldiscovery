#include "stm32l1xx.h"
#include "lcd_gpio_init.h"
#include "stm32l_discovery_lcd_new.h"

int main()
{
  gpio_init();  //Настройка портов ввода/вывода под работу с LCD
  lcd_init();   //Здесь конфигурируем LCD контроллер
  LCD_GLASS_WriteChar("8", POINT_ON, COLUMN_OFF, 1); //Следующие 6 строк выводят символы поочередно в позиции 1-6 индикатора
  LCD_GLASS_WriteChar("0", POINT_OFF, COLUMN_OFF, 2);
  LCD_GLASS_WriteChar(" ", POINT_OFF, COLUMN_OFF, 3);
  LCD_GLASS_WriteChar("M", POINT_OFF, COLUMN_OFF, 4);
  LCD_GLASS_WriteChar("H", POINT_OFF, COLUMN_OFF, 5);
  LCD_GLASS_WriteChar("Z", POINT_OFF, COLUMN_OFF, 6);
  // LCD_GLASS_DisplayString(""); //Эта функция выводит сразу всю строку (если ее раскомментировать, тогда необходимо закомментировать предыдущие 6 строк посимвольного вывода)
  // LCD_GLASS_Clear(); //Очистка дисплея
while(1);
}

//Здесь обрабатывается неудачный результат функций assert_param() из стандартных библиотек
#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  while (1)
  {
  }
}
#endif