/**
 * whodunit.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * This file is a solution to the whodunnit-Problem of pset4
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./whodunit clue verdict\n");
        return 1;
    }

    // remember filenames
    char* clue = argv[1];
    

    // open clue file 
    FILE* clue_ptr = fopen(clue , "r");
    if (clue_ptr == NULL)
    {
        printf("Could not open %s.\n", clue);
        return 2;
    }

    // open verdict file
    FILE* verdict_ptr = fopen("verdict.bmp", "w");
    if (verdict_ptr == NULL)
    {
        fclose(clue_ptr);
        fprintf(stderr, "Could not create %s.\n", "verdict.bmp");
        return 3;
    }

    // read clue file's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, clue_ptr);

    // read clue file's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, clue_ptr);

    // ensure clue file is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(verdict_ptr);
        fclose(clue_ptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // write verdict file's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, verdict_ptr);

    // write verdict file's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, verdict_ptr);

    // determine padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over clue file's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from clue file
            fread(&triple, sizeof(RGBTRIPLE), 1, clue_ptr);
            
            if (triple.rgbtRed == 0xff)
              triple.rgbtRed = 0x00;
            
            if (triple.rgbtRed == 0x00 && triple.rgbtBlue == 0x00 && triple.rgbtGreen == 0x00)
            {
              triple.rgbtRed = 0xff;
              triple.rgbtGreen = 0xff;
              triple.rgbtBlue = 0xff;
            } 
            
            if (triple.rgbtRed > 225 && triple.rgbtRed < 250)
            {
              triple.rgbtRed = 240;
              triple.rgbtGreen = 240;
              triple.rgbtBlue = 240;
            }
            
           else if (triple.rgbtRed > 200 && triple.rgbtRed < 225)
            {
              triple.rgbtRed = 225;
              triple.rgbtGreen = 225;
              triple.rgbtBlue = 225;
            }
            
           else if (triple.rgbtRed > 175 && triple.rgbtRed < 200)
            {
              triple.rgbtRed = 125;
              triple.rgbtGreen = 125;
              triple.rgbtBlue = 125;
            }
            
           else if (triple.rgbtRed > 150 && triple.rgbtRed < 175)
            {
              triple.rgbtRed = 60;
              triple.rgbtGreen = 60;
              triple.rgbtBlue = 60;
            }
            
            else if (triple.rgbtRed > 125 && triple.rgbtRed < 150)
            {
              triple.rgbtRed = 50;
              triple.rgbtGreen = 50;
              triple.rgbtBlue = 50;
            }
            
            else if (triple.rgbtRed > 100 && triple.rgbtRed < 125)
            {
              triple.rgbtRed = 50;
              triple.rgbtGreen = 50;
              triple.rgbtBlue = 50;
            }
            
            else if (triple.rgbtRed > 50 && triple.rgbtRed < 100)
            {
              triple.rgbtRed = 15;
              triple.rgbtGreen = 15;
              triple.rgbtBlue = 15;
            }
            
            else if (triple.rgbtRed > 0 && triple.rgbtRed < 50)
            {
              triple.rgbtRed = 0;
              triple.rgbtGreen = 0;
              triple.rgbtBlue = 0;
            } 
            
            // write RGB triple to  verdict file
            fwrite(&triple, sizeof(RGBTRIPLE), 1, verdict_ptr);
        }

        // skip over padding, if any
        fseek(clue_ptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, verdict_ptr);
        }
    }

    // close clue file
    fclose(clue_ptr);

    // close verdict file
    fclose(verdict_ptr);

    // that's all folks
    return 0;
}
