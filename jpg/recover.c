/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t BYTE;

int main(void)
{
    // Open the memory card file
    FILE* fp = fopen("card.raw", "r");
    
    // Check file permission
    if (fp == NULL)
    {
        printf("Could not recover JPEGS from the card.\n");
        return 1;
    }
    
    // Open output file 
    FILE* fout = NULL;

    // Variables
    char sname[10];
    int jpegc = 0;
    
    // Buffer of 512 bytes 
    BYTE buffer[512];
    
    // Untill end of file is reached
    while (fread(buffer, sizeof(buffer), 1, fp) > 0)  
    {
        // Check the first 4 bytes for JPEG signature
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) & (buffer[2] == 0xff) && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
          // If already exist, then close the file
          if (fout != NULL)
          {
              fclose(fout);
          }
         
          // Name the files with three digits
          sprintf(sname, "%03d.jpg", jpegc);
          
          // Open the file
          fout = fopen(sname, "w");
          
          jpegc++;
          
          // Write into the file
          fwrite(buffer, sizeof(buffer), 1, fout);
         
        } 
        
        else if (jpegc > 0)
        {
          // Put the JPEG into fout
          fwrite(buffer, sizeof(buffer), 1, fout);
        }
        
    } 
    
    // Close the file
    fclose(fp);
   
    // That's all folks
    return 0;
}
