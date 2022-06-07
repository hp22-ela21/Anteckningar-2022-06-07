/***********************************************************************************
* C++ exempel 3 - GPIO-implementering med sysfs i C++. Filhantering sker via
* fstream-klasser ofstream samt ifstream. Notera att dessa inte behöver stängas
* efter användning, då de stängs automatiskt när de går ut scope (via deras
* respektive destruktor), men kan göras via medlemsfunktionen close.
*
* Eftersom biblioteket unistd.h inte ingår i standard C (det är ett Linuxbibliotek),
* så finns inte motsvarande C++-variant cunistd, utan unistd.h får inkluderas,
* precis som i C.
*
* Kompilera i Linux och skapa en körbar fil döpt cpp_example3 via följande kommando:
* g++ *cpp -o cpp_example3 -Wall
*
* Kör sedan programmet via följande kommando:
* ./cpp_example3
***********************************************************************************/

// Inkluderingsdirektiv:
#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>

// Konstanter:
static constexpr std::uint8_t LED1 = 17;
static constexpr std::uint8_t BUTTON1 = 22;
static constexpr std::uint8_t BUTTON2 = 27;

// Enumerationsklasser:
enum class direction { in, out };

/***********************************************************************************
* file_write: Skriver en rad med text till en fil.
***********************************************************************************/
void file_write(const std::string& filepath, const std::string& data)
{
   std::ofstream fstream(filepath, std::ios::app);
   if (!fstream)
   {
      std::cerr << "Could not open file at path " << filepath << "!\n\n";
   }
   else
   {
      fstream << data;
   }
   return;
}

/***********************************************************************************
* file_read: Läser en rad med text från en fil och lagrar i en sträng.
***********************************************************************************/
void file_read(const std::string& filepath, std::string& s)
{
   std::ifstream fstream(filepath, std::ios::in);
   if (!fstream)
   {
      std::cerr << "Could not open file at path " << filepath << "!\n\n";
   }
   else
   {
      std::getline(fstream, s);
   }
   return;
}

/***********************************************************************************
* setup: Reserverar PIN och sätter till in- eller utport.
***********************************************************************************/
static void setup(const std::uint8_t pin, const direction io_direction)
{
   file_write("/sys/class/gpio/export", std::to_string(pin));
   const auto filepath = "/sys/class/gpio/gpio" + std::to_string(pin) + "/direction";
   if (io_direction == direction::in)
   {
      file_write(filepath, "in");
   }
   else
   {
      file_write(filepath, "out");
   }
   return;
}

/***********************************************************************************
* read: Returnerar insignal från en given PIN.
***********************************************************************************/
static std::uint8_t read(const std::uint8_t pin)
{
   const auto filepath = "/sys/class/gpio/gpio" + std::to_string(pin) + "/value";
   std::string s;
   file_read(filepath, s);
   if (s == "1") return 1;
   else return 0;
}

/***********************************************************************************
* write: Sätter utsignal på en given PIN.
***********************************************************************************/
static void write(const std::uint8_t pin, const std::uint8_t value)
{
   const auto filepath = "/sys/class/gpio/gpio" + std::to_string(pin) + "/value";
   if (value) file_write(filepath, "1");
   else file_write(filepath, "0");
   return;
}

/***********************************************************************************
* release: Tar bort reservation från en given PIN. Om aktuell PIN är en utport
*          så sätts utsignalen till låg innan reservationen tas bort.
***********************************************************************************/
static inline void release(const std::uint8_t pin, const direction io_direction)
{
   if (io_direction == direction::out) write(pin, 0);
   file_write("/sys/class/gpio/unexport", std::to_string(pin));
   return;
}

/***********************************************************************************
* blink: Blinkar utsignalen på en given PIN med valbar fördröjningstid mätt i ms.
***********************************************************************************/
static void blink(const std::uint8_t pin, const std::size_t delay_time)
{
   write(pin, 1);
   usleep(delay_time * 1000);
   write(pin, 0);
   usleep(delay_time * 1000);
   return;
}

/***********************************************************************************
* main: Ansluter en lysdiod till PIN 17 samt två tryckknappar till PIN 22 och 27.
*       Ifall en av tryckknapparna är nedtryckt så blinkar lysdioden var 100:e ms.
*       Annars om båda tryckknappar är nedtryckt så hålls lysdioden tänd. Annars
*       hålls lysdioden släckt.
***********************************************************************************/
int main(void)
{
   setup(LED1, direction::out);
   setup(BUTTON1, direction::in);
   setup(BUTTON2, direction::in);

   while (true)
   {
      if (read(BUTTON1) && read(BUTTON2))
      {
         write(LED1, 1);
      }
      else if (read(BUTTON1) || read(BUTTON2))
      {
         blink(LED1, 100);
      }
      else
      {
         write(LED1, 0);
      }
   }

   return 0;
}