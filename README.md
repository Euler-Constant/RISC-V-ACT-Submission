# SUBMISSION FOR THE 'RISC-V ACT Framework Enablement and M-Mode Firmware Validation on Hardware Board' MENTORSHIP 

This is a program that configures a Linux serial with the following configurations and capabilities:

## Serial Port Opening
   - Accepts a serial device path as a CLI argument (e.g., /dev/ttyUSB0)
   - Opens the device in read-write mode (O_RDWR)
   - Error handling with perror on failure

## Terminal Configuration (8N1, Raw Mode)
   - Uses cfmakeraw() to disable canonical processing
   - Control flags (c_cflag): No parity, 1 stop bit, 8-bit data (CS8), enables receiver (CREAD), ignores modem control lines (CLOCAL)
   - Local flags (c_lflag): Disables canonical mode, echo, echo erase, newline echo, and signal characters
   - Input flags (c_iflag): Disables software flow control (XON/XOFF) and special byte handling
   - Output flags (c_oflag): Disables output post-processing and newline-to-CR/LF translation

## Baud Rate Configuration
   - Sets both input and output baud rates to 9600 (B9600)

## Read Timeout Configuration
   - VTIME = 10 → 1 second read timeout (units of 0.1s)
   - VMIN = 0 → return immediately if no data available

## Write Capability
   - Sends the message "Hello\r" (6 bytes) to the serial device on startup

## Blocking Read with select()
   - Uses select() with a 60-second timeout to poll for incoming data
   - Handles three outcomes: timeout, error, or data available
   - Reads up to 256 bytes into a buffer when data is available
   - Prints the number of bytes read and the content

## Secondary Direct Read
   - Performs an additional read() call after the select() block (lines 109-115), though this appears redundant/dead code since the buffer is reused

## Error Handling
   - Checks return values of open, tcgetattr, tcsetattr, select, and read
   - Uses errno and strerror() for descriptive error messages

## Resource Cleanup
   - Closes the serial port file descriptor before exit.


## BUILD AND RUN INSTRUCTIONS

To build, run the following command:
- gcc uart-config.c -o uart-config

> Note: To be able to gain permissions to run the file, we would need to add the user to either the 'dialout' group (Debian) or uucp group (Arch). This test was conducted on Arch Linux.

To run the file, we would need three terminals:

- In the first, initialise the connection with 'socat -d -d pty,raw,echo=0 pty,raw,echo=0'. This opens a pair of pseudo-terminals that could vary per device or session.
- In the second, run the command './uart-config /dev/pts/x', depending on what files it initialises.
- In the third, run the command 'echo "hallo" > /dev/pts/x', again, depending on what file it initialises.

Images in repo. Tested on Arch Linux.
