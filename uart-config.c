#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h> //file control
#include <errno.h> //error handling
#include <termios.h> //termios API for terminal control
#include <sys/select.h> //select() syscall for non-blocking I/O

int main(int argc, char *argv[]){
  if(argc < 2) {
    printf("Usage: %s <serial device>\n", argv[0]);
    return 1;
  }
  //open serial port (file is configurable per device)
  int serial_port = open(argv[1], O_RDWR);

  if(serial_port < 0) {
    perror("open");
    return 1;
  }

  struct termios tty;

  //check for errors
  if(tcgetattr(serial_port, &tty) != 0) {
    printf("ERROR: %i from tcgetattr: %s\n", errno, strerror(errno));
    return 1;
  }
  
  //clean up config values
  cfmakeraw(&tty); 

  /* CONTROL MODE OPTIONS (C_CFLAG) */
  tty.c_cflag &= ~PARENB; //parity bit disabled
  tty.c_cflag &= ~CSTOPB; //stop bit disabled
  tty.c_cflag &= ~CSIZE; //
  tty.c_cflag |= CS8; //8-bit data field
  tty.c_cflag |= CREAD | CLOCAL;
  //Dataframe cOnfiguration for the serial: 8N1
  
  /* LOCAL MODE OPTIONS (C_LFLAG) */
  tty.c_lflag &= ~ICANON; //canonical mode
  tty.c_lflag &= ~ECHO; // echo
  tty.c_lflag &= ~ECHOE; // erasure
  tty.c_lflag &= ~ECHONL; // new-line echo
  tty.c_lflag &= ~ISIG; //signal characters
  
  /* INPUT MODE OPTIONS (C_IFLAG) */
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); //software-flow control
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); //special handling of bytes 
  
  /* OUTPUT MODE OPTIONS (C_OFLAG)*/
  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;
  
  /* SPECIAL CHARACTER OPTIONS (C_CC [NCCS])*/
  tty.c_cc[VTIME] = 10;
  tty.c_cc[VMIN] = 0;
  
  /* BAUD RATES (UNIX-BASED)*/
  cfsetispeed(&tty, B9600);
  cfsetospeed(&tty, B9600);
  
  /* SAVING TTY AND ERROR CHECKS*/
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return 1;
  }
  
  unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
  write(serial_port, msg, sizeof(msg)); // writing terminal
  char read_buf [256];
  memset(&read_buf, '\0', sizeof(read_buf));

  struct timeval timeout;
  fd_set readfds;

  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  FD_ZERO(&readfds);
  FD_SET(serial_port, &readfds);
  int rv = select(serial_port + 1, &readfds, NULL, NULL, &timeout);
  
  if (rv == 0) {
    printf("Timeout: no data received.\n");

  } else if (rv < 0) {
      perror("select");
      close(serial_port);
      return 1;

  } else {
    if (FD_ISSET(serial_port, &readfds)) {
        int num_bytes = read(serial_port, read_buf, sizeof(read_buf));

        if (num_bytes < 0) {
            perror("read");
            close(serial_port);
            return 1;
        }

        printf("Read %d bytes: %s\n", num_bytes, read_buf);
    }
}
  int num_bytes = read(serial_port, &read_buf, sizeof(read_buf)); //reading terminal
  //printf("Read %d bytes: %s\n", num_bytes, read_buf); //value printed to reading terminal

  if(num_bytes < 0){
    printf("Error reading: %s", strerror(errno));
    return 1;
  }

  close(serial_port);
  return 0;
}
