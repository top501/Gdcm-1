/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2008 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "gdcmImage.h"
#include "gdcmImageReader.h"

int TestImage(int, char *[])
{
  gdcm::Image img;

  gdcm::ImageReader reader;
  const gdcm::Image &img2 = reader.GetImage();

{
  gdcm::Image img3 = reader.GetImage();
}

  return 0;
}
