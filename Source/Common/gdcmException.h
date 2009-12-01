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
#ifndef __gdcmException_h
#define __gdcmException_h

#include <cassert>
#include <cstdlib> // NULL
#include <exception>
#include <sstream> // ostringstream
#include <stdexcept> // logic_error
#include <string>

namespace gdcm
{

/**
 * \brief Exception
 * \details Standard exception handling object.
 * \note Its copy-constructor and assignment operator 
 * are generated by the compiler.
 */
class Exception : public std::exception
{
  /// std::logic_error is used to internally hold a string.
  /// It has the nice property of having a copy-constructor
  /// that never fails. Thereby it provides the same guarantee to
  /// the copy-constructor of gdcm::Exception.
  typedef std::logic_error StringHolder;

  /// Creates the text to be returned by Exception:what().
  static StringHolder CreateWhat(const char* const desc,
                                 const char* const file,
                                 const unsigned int lineNumber,
                                 const char* const func)
  {
    assert(desc != NULL);
    assert(file != NULL);
    assert(func != NULL);
    std::ostringstream oswhat;
    oswhat << file << ":" << lineNumber << " (" << func << "):\n";
    oswhat << desc;
    return StringHolder( oswhat.str() );
  }

public:
  /// Explicit constructor, initializing the description and the
  /// text returned by what().
  /// \note The last parameter is ignored for the time being.
  /// It may be used to specify the function where the exception
  /// was thrown.
  explicit Exception(const char *desc = "None", 
            const char *file = __FILE__,
            unsigned int lineNumber = __LINE__,
            // FIXME:  __PRETTY_FUNCTION__ is the non-mangled version for __GNUC__ compiler
            const char *func = "" /*__FUNCTION__*/)
  :
  What( CreateWhat(desc, file, lineNumber, func) ),
  Description(desc)
  {
  }
  
  virtual ~Exception() throw() {}
         
  /// what implementation
  const char* what() const throw()
    { 
    return What.what();
    }

  /// Return the Description
  const char * GetDescription() const { return Description.what(); }

private:
  StringHolder  What;
  StringHolder  Description;
};

} // end namespace gdcm

#endif

