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
#include "gdcmImageApplyLookupTable.h"
#include "gdcmTesting.h"
#include "gdcmSystem.h"
#include "gdcmImageReader.h"
#include "gdcmImageWriter.h"

int TestImageApplyLookupTableFunc(const char *filename, bool verbose = false)
{
  gdcm::ImageReader reader;
  reader.SetFileName( filename );
  if ( !reader.Read() )
    {
    return 1;
    }
  const gdcm::Image &image = reader.GetImage();

  const gdcm::PhotometricInterpretation &pi = image.GetPhotometricInterpretation();
  if( pi != PhotometricInterpretation::PALETTE_COLOR )
    {
    // yeah well not much I can do here...
    return 0;
    }

  gdcm::ImageApplyLookupTable lutfilt;
  lutfilt.SetInput( image );
  bool b = lutfilt.Apply();
  if( !b )
    {
    std::cerr << "Could not apply lut: " << filename << std::endl;
    return 1;
    }

  // Create directory first:
  const char subdir[] = "TestImageApplyLookupTable";
  std::string tmpdir = gdcm::Testing::GetTempDirectory( subdir );
  if( !gdcm::System::FileIsDirectory( tmpdir.c_str() ) )
    {
    gdcm::System::MakeDirectory( tmpdir.c_str() );
    //return 1;
    }
  std::string outfilename = gdcm::Testing::GetTempFilename( filename, subdir );

  gdcm::ImageWriter writer;
  writer.SetFileName( outfilename.c_str() );
  //writer.SetFile( reader.GetFile() ); // increase test goal
  writer.SetImage( lutfilt.GetOutput() );
  if( !writer.Write() )
    {
    std::cerr << "Failed to write: " << outfilename << std::endl;
    return 1;
    }

  return 0;
}

int TestImageApplyLookupTable(int argc, char *argv[])
{
  if( argc == 2 )
    {
    const char *filename = argv[1];
    return TestImageApplyLookupTableFunc(filename, true);
    }

  // else
  gdcm::Trace::DebugOff();
  gdcm::Trace::WarningOff();
  int r = 0, i = 0;
  const char *filename;
  const char * const *filenames = gdcm::Testing::GetFileNames();
  while( (filename = filenames[i]) )
    {
    r += TestImageApplyLookupTableFunc( filename );
    ++i;
    }

  return r;
}
