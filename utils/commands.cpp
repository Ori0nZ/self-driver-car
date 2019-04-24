#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <stdlib.h>
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
	unsigned char arraycomd[4] = {"FWD", "STP", "LFT", "RFT"};
    printf("Do command: ");
    
      unsigned char tx;
      tx = arraycomd[0];
	serialPutchar(port_num, tx);
	sleep(2000);
	tx = arraycomd[3];
	  serialPutchar(port_num, tx);
	  sleep(2000);
	  tx = arraycomd[2];
	  serialPutchar(port_num, tx);
	  sleep(2000);
	  tx = arraycomd[1];
	  serialPutchar(port_num, tx);
	  sleep(2000);
    }
  }
}

