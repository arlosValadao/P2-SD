#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <lcd.h>
#include <wiringPi.h>
#include <wiringSerial.h>


//USE WIRINGPI PIN NUMBERS
#define LCD_RS  13               //Register select pin
#define LCD_E   18               //Enable Pin
#define LCD_D4  21               //Data pin 4
#define LCD_D5  24               //Data pin 5
#define LCD_D6  26               //Data pin 6
#define LCD_D7  27               //Data pin 7
#define B_UP	23				//PA07
#define B_DOWN	25				//PA10
#define B_ENTER	19				//PA20


int main()
{
  char array_menu[3][100] = { {"SELECIONAR NODE 1"}, {"LIGAR LED BUILTIN"}, {"APAGAR LED BUILTIN"} };
  int array_command[3] = { 0x3, 0x1, 0x2 };
  int menu_counter = -1;
  int fd;
  int lcdfd;
      if ((fd = serialOpen ("/dev/ttyS3", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
	pinMode(B_DOWN, INPUT);
	pinMode(B_UP, INPUT);
	pinMode(B_ENTER, INPUT);
   lcdfd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
   printf("PROGRAMA RODANDO\n");
	lcdClear(lcdfd);
	lcdPuts(lcdfd, "BEM-VINDO");
   while(1) {
		if(!digitalRead(B_DOWN)) {
			//while(!digitalRead(B_DOWN)) {}
			delay(230);
			printf("PRESSIONOU B_DOWN\n");
			menu_counter++;
			if(menu_counter > 2) menu_counter = 0;
			lcdClear(lcdfd);
			lcdPrintf(lcdfd, "%s", array_menu[menu_counter]);
		}
	//	else if(!digitalRead(B_ENTER)) serialPutchar(fd, array_command[menu_counter - 1]);
		else if(!digitalRead(B_UP)) {
			delay(230); 
			printf("PRESSIONOU B_UP\n");
			menu_counter--;
			if(menu_counter < 0) menu_counter = 2;
			lcdClear(lcdfd);
			lcdPrintf(lcdfd, "%s", array_menu[menu_counter]);
		}
		
		else if(!digitalRead(B_ENTER)) {
			delay(230);
			printf("PRESSIONOU B_ENTER\n");
			lcdClear(lcdfd);
			lcdPuts(lcdfd, "ENVIANDO COMANDO...");
			serialPutchar(fd, array_command[menu_counter]);
			delay(500);
			lcdClear(lcdfd);
			lcdPuts(lcdfd, "COMANDO ENVIADO!");
			delay(500);
			lcdClear(lcdfd);
			lcdPrintf(lcdfd, "%s", array_menu[menu_counter]);	
		}
	}	 
	return 0;
}
