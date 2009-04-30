/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2009 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#ifndef __gdcmImageToImageFilter_h
#define __gdcmImageToImageFilter_h

#include "gdcmPixmap.h"

namespace gdcm
{

/**
 * \brief ImageToImageFilter class
 * Super class for all filter taking an image and producing an output image
 */
class GDCM_EXPORT ImageToImageFilter
{
public:
  ImageToImageFilter():Input(new Pixmap),Output(new Pixmap) {}
  ~ImageToImageFilter() {}

  /// Set input image
  void SetInput(const Pixmap& image);

  /// Get Output image
  const Pixmap &GetOutput() const { return *Output; }

protected:
  SmartPointer<Pixmap> Input;
  SmartPointer<Pixmap> Output;
};

} // end namespace gdcm

#endif //__gdcmImageToImageFilter_h

