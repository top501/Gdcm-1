/*=========================================================================

  Program: GDCM (Grass Root DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2008 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*
 * 1. Create a DICOM file from a 'raw' input:
 * 2. Create a blob (jpeg,pgm/pnm,j2k,rle) from input
 * - binary blob(s) (grayscale / RGB) input
 * - jpeg(s)
 * - j2k(s)
 *
 *   Mapping is:
 *
 *   DICOM RAW  <->  pnm/pgm
 *   DICOM jpg  <->  jpg
 *   DICOM jpgl <->  jpgl
 *   DICOM jpls <->  jpls
 *   DICOM j2k  <->  j2k
 *   DICOM rle  <->  Utah RLE ??
 *
 * ??:
 *   DICOM avi  <->  avi
 *   DICOM wav  <->  wav
 *   DICOM pdf  <->  pdf
 */
#include "gdcmReader.h"
#include "gdcmImageWriter.h"
#include "gdcmImageReader.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmDataSet.h"

#include <string>
#include <iostream>

#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <string.h>

int main (int argc, char *argv[])
{
  int c;
  //int digit_optind = 0;

  std::string filename;
  std::string outfilename;
  while (1) {
    //int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
        {"input", 1, 0, 0},
        {"output", 1, 0, 0},
        {0, 0, 0, 0}
    };

    // i -> input file
    // I -> input directory
    // o -> output file
    // O -> output directory
    c = getopt_long (argc, argv, "i:o:I:O:",
      long_options, &option_index);
    if (c == -1)
      {
      break;
      }

    switch (c)
      {
    case 0:
        {
        const char *s = long_options[option_index].name;
        printf ("option %s", s);
        if (optarg)
          {
          if( option_index == 0 ) /* input */
            {
            assert( strcmp(s, "input") == 0 );
            assert( filename.empty() );
            filename = optarg;
            }
          printf (" with arg %s", optarg);
          }
        printf ("\n");
        }
      break;

    case 'i':
      printf ("option i with value '%s'\n", optarg);
      assert( filename.empty() );
      filename = optarg;
      break;

    case 'o':
      printf ("option o with value '%s'\n", optarg);
      assert( outfilename.empty() );
      outfilename = optarg;
      break;

    case '?':
      break;

    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
      }
  }

  // For now only support one input / one output
  if (optind < argc)
    {
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
      {
      printf ("%s ", argv[optind++]);
      }
    printf ("\n");
    return 1;
    }

  if( filename.empty() )
    {
    std::cerr << "Need input file (-i)\n";
    return 1;
    }
  //if( outfilename.empty() )
  //  {
  //  std::cerr << "Need output file (-o)\n";
  //  return 1;
  //  }

/*
  gdcm::ImageWriter writer;
  writer.SetFileName( outfilename.c_str() );
  if( !writer.Write() )
    {
    std::cerr << "Failed to write: " << outfilename << std::endl;
    return 1;
    }
*/
  gdcm::ImageReader reader;
  reader.SetFileName( filename.c_str() );
  if( !reader.Read() )
    {
    std::cerr << "Failed to read: " << filename << std::endl;
    return 1;
    }

  unsigned int n = reader.GetNumberOfOverlays();
  for(unsigned int i = 0; i < n; ++i )
    {
    const gdcm::Overlay& o = reader.GetOverlay(i);
    o.Print( std::cout );
    }

  return 0;
}

