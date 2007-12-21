/*=========================================================================

  Program: GDCM (Grass Root DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2007 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmConfigure.h" // for GDCM_DATA_ROOT
#include "vtkGDCMReader.h"

#include "vtkPNGWriter.h"
#include "vtkImageData.h"
#include <vtksys/SystemTools.hxx>

#include "gdcmDataImages.h"

int TestvtkGDCMRead(const char *filename)
{
  vtkGDCMReader *reader = vtkGDCMReader::New();
  //reader->CanReadFile( filename );
  std::cerr << "Reading : " << filename << std::endl;
  reader->SetFileName( filename );
  reader->Update();

  reader->GetOutput()->Print( cout );

  vtkPNGWriter *writer = vtkPNGWriter::New();
  writer->SetInputConnection( reader->GetOutputPort() );
  std::string pngfile = vtksys::SystemTools::GetFilenamePath( filename );
  pngfile = "/tmp/png";
  pngfile += '/';
  pngfile += vtksys::SystemTools::GetFilenameWithoutExtension( filename );
  pngfile += ".png";
  writer->SetFileName( pngfile.c_str() );
  writer->Write();

  reader->Delete();
  writer->Delete();
  return 0; 
}

int TestvtkGDCMReader(int argc, char *argv[])
{
  if( argc == 2 )
    {
    const char *filename = argv[1];
    return TestvtkGDCMRead(filename);
    }

  // else
  int r = 0, i = 0;
  const char *filename;
  while( (filename = gdcmDataImages[i]) )
    {
    r += TestvtkGDCMRead( filename );
    ++i;
    }

  return r;
}
