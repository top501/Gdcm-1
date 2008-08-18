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
#ifndef __gdcmJPEG16Codec_h
#define __gdcmJPEG16Codec_h

#include "gdcmJPEGCodec.h"

namespace gdcm
{
  
/**
 * \brief Class to do JPEG 16bits (lossless)
 * \note
 */
class JPEGInternals;
class ByteValue;
class JPEG16Codec : public JPEGCodec
{
public:
  JPEG16Codec();
  ~JPEG16Codec();

  bool Decode(std::istream &is, std::ostream &os);
  bool InternalCode(const char *input, unsigned long len, std::ostream &os);

  bool GetHeaderInfo(std::istream &is, TransferSyntax &ts);

private:
  JPEGInternals *Internals;
};

} // end namespace gdcm

#endif //__gdcmJPEG16Codec_h
