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
#include "gdcmUID.h"
#include "gdcmTrace.h"
#include "gdcmSystem.h"

#include "uuid/uuid.h"

namespace gdcm
{

const char UID::GDCM_UID[] = "1.2.826.0.1.3680043.2.1143";
std::string UID::Root = GetGDCMUID();
std::string UID::HardwareAddress = System::EncodeHardwareAddress();

const char *UID::GetGDCMUID()
{
  return GDCM_UID;
}

template <int N>
inline int getlastdigit(unsigned char *data)
{
  int extended, carry = 0;
  for(int i=0;i<N;i++)
    {
    extended = (carry << 8) + data[i];
    data[i] = extended / 10;
    carry = extended % 10;
    }
  return carry;
}

const char* UID::GenerateUniqueUID()
{
  Unique = GetRoot();
  Unique = GetGDCMUID(); // FIXME !!!
  if( Unique.empty() )
    {
    // Seriously...
    return NULL;
    }
//  if( HardwareAddress.empty() )
//    {
//    HardwareAddress = System::EncodeHardwareAddress(); // called once, too system intensive
//    if( HardwareAddress.empty() )
//      {
//      // ethernet cable is not plugged, do not allow creating UIDs
//      return NULL;
//      }
//    //srand( time(NULL) );
//    }
  assert( !HardwareAddress.empty() ); // programmer error
  Unique += ".";
  Unique += HardwareAddress;
  Unique += ".";
  std::string datetime = System::GetCurrentDateTime();
  if( datetime.empty() )
    {
    // Not sure how this is supposed to happen...
    return NULL;
    }
  Unique += datetime;
  // Also add a mini random number just in case:
  // FIXME: I choose 2 bytes for the random number, since GDCM Root UID is so long, and harware address can take up
  // to 15 bytes, case 255.255.255.255.255.255 <-> 281474976710655
  // But a better approach to dynamically calculate the max size for random bits...
  unsigned char tmp[2+1];
  //int r = (int) (100.0*rand()/RAND_MAX);
  get_random_bytes(tmp, 2);
  bool zero = false;
  int res;
  std::string sres;
  while(!zero)
    {
    res = getlastdigit<2>(tmp);
    sres.insert(sres.begin(), '0' + res);
    zero = (tmp[0] == 0) && (tmp[1] == 0) /*&& (addr[2] == 0) 
      && (addr[3] == 0) && (addr[4] == 0) && (addr[5] == 0)*/;
    }


  //int n = snprintf(tmp,3,"%02d", r);
//  if( n > 2 ) 
//    {
//    abort();
//    return "";
//    }
  Unique += ".";
  Unique += sres;
//  if( strcmp(tmp, "00") == 0 )
//    {
//    Unique += '0'; // special rule for '0'
//    }
//  else if( tmp[0] == '0' )
//    {
//    // Remove the starting '0'
//    Unique += tmp+1;
//    }
//  else // add all bytes
//    {
//    Unique += tmp;
//    }
  assert( IsUIDValid( Unique.c_str() ) );

  return Unique.c_str();
}

bool UID::IsUIDValid(const char *uid_)
{
  /*
  9.1 UID ENCODING RULES
  The DICOM UID encoding rules are defined as follows:
  - Each component of a UID is a number and shall consist of one or more digits. The first digit of
  each component shall not be zero unless the component is a single digit.
  Note: Registration authorities may distribute components with non-significant leading zeroes. The leading
  zeroes should be ignored when being encoded (ie. �00029� would be encoded �29�).
  - Each component numeric value shall be encoded using the characters 0-9 of the Basic G0 Set
  of the International Reference Version of ISO 646:1990 (the DICOM default character
  repertoire).
  - Components shall be separated by the character "." (2EH).
  - If ending on an odd byte boundary, except when used for network negotiation (See PS 3.8),
  one trailing NULL (00H), as a padding character, shall follow the last component in order to
  align the UID on an even byte boundary.
  - UID's, shall not exceed 64 total characters, including the digits of each component, separators
  between components, and the NULL (00H) padding character if needed.
  */

  /*
   * FIXME: This is not clear in the standard, but I believe a trailing '.' is not allowed since
   * this is considered as a separator for components
   */

  std::string uid = uid_;
  if( uid.size() > 64 )
    {
    return false;
    }
  if( uid[uid.size()-1] == '.' )
    {
    return false;
    }
  std::string::size_type i = 0;
  for(; i < uid.size(); ++i)
    {
    if( uid[i] == '.' )
      {
      // check that next character is neither '0' (except single number) not '.'
      if( uid[i+1] == '.' )
        {
        return false;
        }
      else if( uid[i+1] == '0' ) // character is garantee to exist since '.' is not last char
        {
        // Need to check first if we are not at the end of string
        if( i+2 != uid.size() && uid[i+2] != '.' )
          {
          return false;
          }
        }
      }
    }
  // no error found !
  return true;
}


} // end namespace gdcm
