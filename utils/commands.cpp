#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
void drive_funtion(unsigned char fun[4])
{
  for (int i = 0; i < 4; i++)
  {
    serialPutchar(port_num, fun[i]);
    printf("%c", fun[i]);
  }
}
int main()
{
  int port_num;
  printf("Connection...");
  if ((port_num = serialOpen("/dev/ttyS0", 9600)) < 0)
  {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    return 1;
  }
  printf("Connected Successful\n");
  unsigned char left[] = "LFT!";
  unsigned char right[] = "RFT!";
  unsigned char stop[] = "STP!";
  unsigned char go_on[] = "SFT!";
  while (1)
  {
    drive_funtion(left);
    usleep(1000);
    drive_funtion(right);
    usleep(1000);
    drive_funtion(go_on);
    usleep(1000);
    drive_funtion(stop);
    usleep(1000);
  }

  /*while (1)
  {
    printf("Sent: ");
    while (1)
    {
      unsigned char tx;
      scanf("%c", &tx);
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
  }*/
}