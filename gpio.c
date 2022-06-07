/* Inkluderingsdirektiv: */
#include "gpio.h"

/* Statiska funktioner: */
static void file_write(const char* filepath, const char* s);
static void file_read(const char* filepath, char* s, const size_t size);
static void gpio_export(const uint8_t pin);
static void gpio_unexport(const uint8_t pin);
static void gpio_set_direction(const uint8_t pin, const enum gpio_direction direction);

/***************************************************************************
* gpio_new: Reserverar aktuell PIN och v�ljer datariktning.
***************************************************************************/
void gpio_new(const uint8_t pin, const enum gpio_direction direction)
{
   gpio_export(pin);
   gpio_set_direction(pin, direction);
   return;
}

/***************************************************************************
* gpio_delete: Tar bort reservation fr�n aktuell PIN.
***************************************************************************/
void gpio_delete(const uint8_t pin)
{
   gpio_unexport(pin);
   return;
}

/***************************************************************************
* gpio_write: S�tter utsignal p� aktuell PIN. Fils�kv�gen s�tts samman
*             med PIN-numret via anrop av funktionen sprintf.
***************************************************************************/
void gpio_write(const uint8_t pin, const uint8_t value)
{
   char filepath[50];
   filepath[0] = '\0';
   sprintf(filepath, "/sys/class/gpio/gpio%u/value", pin);
   if (value)
   {
      file_write(filepath, "1");
   }
   else
   {
      file_write(filepath, "0");
   }
   return;
}

/***************************************************************************
* gpio_read: Returnerar insignal fr�n en given PIN.
***************************************************************************/
uint8_t gpio_read(const uint8_t pin)
{
   char s[10];
   char filepath[50];
   s[0] = '\0';
   filepath[0] = '\0';

   sprintf(filepath, "/sys/class/gpio/gpio%u/value", pin);
   file_read(filepath, s, sizeof(s));

   if (s[0] == '1')
   {
      return 1;
   }
   else
   {
      return 0;
   }
}

/***************************************************************************
* gpio_delay: Genererar f�rdr�jning m�tt i millisekunder.
***************************************************************************/
void gpio_delay(const size_t delay_time)
{
}

/***************************************************************************
* gpio_blink: Blinkar utsignalen p� aktuell PIN.
***************************************************************************/
void gpio_blink(const uint8_t pin, const size_t delay_time)
{
}

/***************************************************************************
* gpio_event_detected: Indikerar ifall ett specifikt event har �gt rum
*                      p� aktuell PIN och uppdaterar senast avl�sta
*                      insignal via ing�ende argument last_value. 
***************************************************************************/
bool gpio_event_detected(const uint8_t pin, const enum gpio_event event, uint8_t* last_value)
{
   return false;
}

/***************************************************************************
* file_write: Skriver ett textstycke till en fil.
***************************************************************************/
static void file_write(const char* filepath, const char* s)
{
}

/***************************************************************************
* file_read: L�ser en rad fr�n en fil och lagrar i en str�ng.
***************************************************************************/
static void file_read(const char* filepath, char* s, const size_t size)
{
}

/***************************************************************************
* gpio_export: Reserverar aktuell PIN f�r anv�ndning.
***************************************************************************/
static void gpio_export(const uint8_t pin)
{
}

/***************************************************************************
* gpio_unexport: Tar bort reservation av PIN efter anv�ndning.
***************************************************************************/
static void gpio_unexport(const uint8_t pin)
{
}

/***************************************************************************
* gpio_set_direction: S�tter datariktning p� aktuell PIN.
***************************************************************************/
static void gpio_set_direction(const uint8_t pin, const enum gpio_direction direction)
{
}
