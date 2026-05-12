#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h> //file control
#include <errno.h> //error handling
#include <termios.h> //termios API for terminal control

struct termios tty; //tty as naming convention

int main(){
  //open serial port (in this case, /dev/ttyS0)
  int serial_port = open("/dev/ttyS0", O_RDWR);

  //check for errors
  if(tcgetattr(serial_port, &tty) != 0) {
    printf("ERROR: %i from tcgetattr: %s\n", errno, strerror(errno));
    return 1;
  }

  /* CONTROL MODE OPTIONS (C_CFLAG) */
  tty.c_cflag &= ~PARENB;
  tty.c_cflag |= PARENB;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag |= CSTOPB;
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS7; //7-bit data field
}
