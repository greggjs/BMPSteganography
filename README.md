This program takes a 32 bit BMP file and encodes a message using the Least Significant Bit (LSB) of each R,G, and B pixel, and skips the 4th pixel involved. To execute, you need 3 parameters:
1. input file
2. output file
3. string message
This code usese EasyBMP for getting file size and offset, but I modified it such that it will work to get size and such, since it didn't. Use my copy of EasyBMP for it to work properly.
