#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h> //file control
#include <errno.h> //error handling
#include <termios.h> //termios API for terminal control

struct termios tty; //tty as naming convention

int main(){
  int serial_port = open("/dev/ttyS0", O_RDWR);

  // Check for errors
  if(serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }

  if(tcgetattr(serial_port, &tty) != 0) {
    printf("ERROR: %i from tcgetattr: %s\n", errno, strerror(errno));
  }
}
