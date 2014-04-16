#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "EasyBMP/EasyBMP.cpp"

using namespace std;

int main(int argc, char** argv) {
  if (argc < 4) {
    cout << "Not enough arguments." << endl;
    return 1;
  }

  string message(argv[3]);
  message = message + (char)10;
  int len = message.length();

  // Use EasyBMP to get all file stats
  // It's just easier this way.
  BMP sheldon;
  sheldon.ReadFromFile(argv[1]);
  int offset = sheldon.TellOffset();
  int height = sheldon.TellHeight();
  int width = sheldon.TellWidth();
  cout << "Pixels start at: " << offset << endl;
  cout << "Width of image: " << width << endl;
  cout << "Height of image: " << height << endl;

  // Open the BMP file for manipulation
  FILE * input_file = fopen(argv[1], "rb");
  long size;
  size_t result;
  unsigned char* memblock;

  // Only proceed if we have a file
  if (input_file != NULL) {
    // get file size
    fseek(input_file, 0, SEEK_END);
    size = ftell(input_file);
    rewind(input_file);

    // Allocate memory for file read
    // and read it into our memblock
    // then close the file.
    memblock = new unsigned char[size];
    result = fread(memblock, sizeof(unsigned char), size, input_file);
    fclose(input_file);
    // only proceed if we have data.
    if (result == size) {
      // two vars, one for string position, the other for
      // keeping track of pixel depth
      int k = 0, depth = 0;
      // iterate over all pixels
      // i = memblock counter
      // j = current char bit counter
      // depth = pixel counter
      for (int i = 0; i < size; i++) {
        // only start manipulation if we are past the
        // offset value.
        if (i >= offset) {
          // only add chars if we are in the bounds of
          // our string
          if (k < len) {
            // get the character to manipulate
            char curr = message[k];
            // iterate over all it's bits
            for (int j = 0; j < 8; j++) {
              // if we are at the fourth byte, skip it
              if (depth == 3) {
                // decrement our bit counter
                // reset the depth
                // increment our byte counter
                depth = 0; i++; j--;
              // else, proceed with adding a bit to the end of a pixel.
              } else {
                // get the LSB for the current position in the string
                unsigned char temp = (curr >> j) & 1;
                // add it to the end of the current memory block
                memblock[i] = ((memblock[i] >> 1) << 1) | temp;
                // increment the depth and memblock counter.
                i++; depth++;
              }
            }
            // afterwards we increment our string counter and
            // rewind back once in the memblock counter.
            k++; i--;
          } else {
            break;
          }
        }
      }
    } else {
      cout << "Did not read from file" << endl;
      return 1;
    }
  } else {
    return 1;
  }

  // write to our output file
  FILE* output_file = fopen(argv[2], "wb");
  if (output_file != NULL) {
    result = fwrite(memblock, sizeof(unsigned char), size, output_file);
    if (result != size) {
      cout << "Did not write to file" << endl;
      fclose(output_file);
      delete[] memblock;
      return 1;
    }
    fclose(output_file);
  } else {
    cout << "Could not open output file" << endl;
    delete[] memblock;
    return 1;
  }
  // delete the memory block
  delete[] memblock;

  return 0;
}
