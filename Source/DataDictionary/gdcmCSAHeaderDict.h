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
#ifndef __gdcmCSAHeaderDict_h
#define __gdcmCSAHeaderDict_h

#include "gdcmTypes.h"
#include "gdcmTag.h"
#include "gdcmCSAHeaderDictEntry.h"

#include <iostream>
#include <iomanip>
#include <set>

namespace gdcm
{
/**
 * \brief Class to represent a map of CSAHeaderDictEntry
 */
class GDCM_EXPORT CSAHeaderDict
{
public:
  typedef std::set<CSAHeaderDictEntry> MapCSAHeaderDictEntry;
  typedef MapCSAHeaderDictEntry::iterator Iterator;
  typedef MapCSAHeaderDictEntry::const_iterator ConstIterator;
  //static CSAHeaderDictEntry GroupLengthCSAHeaderDictEntry; // = CSAHeaderDictEntry("Group Length",VR::UL,VM::VM1);

  CSAHeaderDict():CSAHeaderDictInternal() {
    assert( CSAHeaderDictInternal.empty() );
  }

  friend std::ostream& operator<<(std::ostream& _os, const CSAHeaderDict &_val);

  ConstIterator Begin() const { return CSAHeaderDictInternal.begin(); }
  ConstIterator End() const { return CSAHeaderDictInternal.end(); }

  bool IsEmpty() const { return CSAHeaderDictInternal.empty(); }
  void AddCSAHeaderDictEntry(const CSAHeaderDictEntry &de)
    {
#ifndef NDEBUG
    MapCSAHeaderDictEntry::size_type s = CSAHeaderDictInternal.size();
#endif
    CSAHeaderDictInternal.insert( de );
    assert( s < CSAHeaderDictInternal.size() );
    }

  const CSAHeaderDictEntry &GetCSAHeaderDictEntry(const char *name) const
    {
    return *CSAHeaderDictInternal.begin();
    }

protected:
  friend class CSAHeaderDicts;
  void LoadDefault();

private:
  CSAHeaderDict &operator=(const CSAHeaderDict &_val); // purposely not implemented
  CSAHeaderDict(const CSAHeaderDict &_val); // purposely not implemented

  MapCSAHeaderDictEntry CSAHeaderDictInternal;
};
//-----------------------------------------------------------------------------
inline std::ostream& operator<<(std::ostream& os, const CSAHeaderDict &val)
{
  return os;
}


} // end namespace gdcm

#endif //__gdcmCSAHeaderDict_h
