#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>

int main ()
{
  int port_num;
  printf("Connection...");
  if ((port_num = serialOpen ("/dev/ttyS0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }
  printf("Successful\n");

  while (1)
  {
    printf("Sent: ");
    while(1)
	{
      unsigned char tx;
      scanf ("%c",&tx);
      if (tx == '\n')
	  {
        tx = '!';
		serialPutchar(port_num, tx);
		fflush(stdout);
		break;
      }
      if (tx != '\0')
	  {
        serialPutchar(port_num, tx);
      }
      fflush(stdout);
    }
  }
}

