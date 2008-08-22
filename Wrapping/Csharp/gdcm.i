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
// See docs:
// http://www.swig.org/Doc1.3/Python.html
// http://www.swig.org/Doc1.3/SWIGPlus.html#SWIGPlus
// http://www.geocities.com/foetsch/python/extending_python.htm

%module(docstring="A DICOM library") gdcm
#pragma SWIG nowarn=504,510
%{
#include "gdcmTypes.h"
#include "gdcmSwapCode.h"
#include "gdcmObject.h"
#include "gdcmTag.h"
#include "gdcmVL.h"
#include "gdcmVR.h"
#include "gdcmDataElement.h"
#include "gdcmDataSet.h"
#include "gdcmPreamble.h"
#include "gdcmFile.h"
#include "gdcmReader.h"
#include "gdcmImageReader.h"
#include "gdcmStringFilter.h"
#include "gdcmGlobal.h"
#include "gdcmVR.h"
#include "gdcmVM.h"
#include "gdcmDicts.h"
#include "gdcmDict.h"
#include "gdcmDictEntry.h"
#include "gdcmDirectory.h"
#include "gdcmTesting.h"
#include "gdcmUIDGenerator.h"
#include "gdcmScanner.h"

using namespace gdcm;
%}

// swig need to know what are uint16_t, uint8_t...
%include "stdint.i"

// gdcm does not use std::string in its interface, but we do need it for the 
// %extend (see below)
%include "std_string.i"
//%include "std_set.i"
%include "std_vector.i"
%include "std_pair.i"
%include "std_map.i"

// operator= is not needed in python AFAIK
%ignore operator=;                      // Ignore = everywhere.
%ignore operator++;                     // Ignore

//%feature("autodoc", "1")
//%include "gdcmTypes.h" // define GDCM_EXPORT so need to be the first one...
#define GDCM_EXPORT
%include "gdcmSwapCode.h"
%include "gdcmPixelFormat.h"
//%include "gdcmMediaStorage.h"
%rename(__getitem__) gdcm::Tag::operator[];
%include "gdcmTag.h"
%extend gdcm::Tag
{
  const char *__str__() {
    static std::string buffer;
    std::ostringstream os;
    os << *self;
    buffer = os.str();
    return buffer.c_str();
  }
};
%include "gdcmDataElement.h"
%include "gdcmDataSet.h"
%include "gdcmPhotometricInterpretation.h"
%include "gdcmObject.h"
%include "gdcmLookupTable.h"
%include "gdcmOverlay.h"
%include "gdcmVL.h"
%include "gdcmValue.h"
%include "gdcmByteValue.h"
%include "gdcmPreamble.h"
%include "gdcmFileMetaInformation.h"
%include "gdcmFile.h"
%include "gdcmImage.h"
%include "gdcmGlobal.h"
%include "gdcmDictEntry.h"
%include "gdcmDict.h"
%include "gdcmDicts.h"
%include "gdcmReader.h"
%include "gdcmImageReader.h"
%template (PairString) std::pair<std::string,std::string>;
%include "gdcmStringFilter.h"
%template (FilenamesType) std::vector<std::string>;
%include "gdcmDirectory.h"
%include "gdcmTesting.h"
%include "gdcmUIDGenerator.h"
%include "gdcmScanner.h"

