#include <stdio.h>
#include <stdlib.h>

int byte_swap(int w)
{ unsigned int b0 = 0xFF & w;
  unsigned int b1 = 0xFF & (w >> 8);
  unsigned int b2 = 0xFF & (w >> 16);
  unsigned int b3 = 0xFF & (w >> 24);
  
  return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;  }

int main (int ac, char **av)
{ if(ac != 4)
    { printf("Usage: objcopy_convert <infile> <outfile> <number_of_words>\n");
      return 1; }
  
  FILE* ifile = fopen(av[1], "rb");
  FILE* ofile = fopen(av[2], "w");

  if(ifile == NULL)
    { printf("ERROR: Can't open file <%s>\n", av[1]);
      return 2; }

  if(ofile == NULL)
    { printf("ERROR: Can't open file <%s>\n", av[2]);
      return 2; }

  fseek(ifile, 0, SEEK_END);
  int ifile_size = ftell(ifile);
  rewind(ifile);

  if(ifile_size % 4)
    { printf("ERROR: Input file size (%d) is not a multiple of four!\n", ifile_size);
      fclose(ifile);
      fclose(ofile);
      return 3; }

  int words_in = ifile_size / 4;
  int words_out = atoi(av[3]);

  if(words_out == 0)
    { printf("ERROR: Requested output size <%s> is zero or cannot be parsed!\n", av[3]);
      fclose(ifile);
      fclose(ofile);
      return 4; }
  
  if(words_in > words_out)
    { printf("ERROR: Number of words (%d) in file <%s> is greater than requested! (%d)\n", 
	     words_in,
	     av[1],
	     words_out);
      fclose(ifile);
      fclose(ofile);
      return 5; }
  
  printf("\tFYI: %d out of %d words used for <%s>\n", words_in, words_out, av[1]);
    
  int * buf = malloc(4*words_out);
  
  for(int i = 0; i < words_out; ++i) buf[i] = 0;
  fread(buf, 4, words_in, ifile);

  for(int i = 0; i < words_out; ++i)
    fprintf(ofile, "%08X\n", byte_swap(buf[i]));

  free(buf);
  fclose(ifile);
  fclose(ofile);

  return 0; }
