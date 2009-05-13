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
#include "gdcmSHA1.h"
#include "gdcmSystem.h"
#include "gdcm_polarssl.h"

#ifdef GDCM_USE_SYSTEM_OPENSSL
#include <openssl/sha.h>
#endif

#include <string.h> // memcmp
#include <stdlib.h> // malloc

/*
 */
namespace gdcm
{

class SHA1Internals
{
public:
#ifdef GDCM_USE_SYSTEM_OPENSSL
  SHA_CTX ctx;
#endif
};

SHA1::SHA1()
{
  Internals = new SHA1Internals;
}

SHA1::~SHA1()
{
  delete Internals;
}

bool SHA1::Compute(const char *buffer, unsigned long buf_len, char digest[])
{
#ifdef GDCM_USE_SYSTEM_OPENSSL
  if( !buffer || !buf_len )
    {
    return false;
    }

  unsigned char output[20];

  SHA_CTX ctx;
  SHA1_Init(&ctx);
  SHA1_Update(&ctx, buffer, buf_len);
  SHA1_Final(output, &ctx);

  unsigned char output2[20];
  sha1( (unsigned char*)buffer, buf_len, output2);
  assert( memcmp( output, output2, 20 ) == 0 );

  for (int di = 0; di < 20; ++di)
    {
    sprintf(digest+2*di, "%02x", output[di]);
    }
  digest[2*20] = '\0';

  return true;
#else
  digest = 0;
  return false;
#endif
}

inline bool process_file(const char *filename, unsigned char *digest)
{
  if( !filename || !digest ) return false;

  FILE *file = fopen(filename, "rb");
  if(!file) 
    {
    return false;
    }

  size_t file_size = System::FileSize(filename);
  void *buffer = malloc(file_size);
  if(!buffer) 
    {
    fclose(file);
    return false;
    }
  size_t read = fread(buffer, 1, file_size, file);
  if( read != file_size ) return false;

  SHA_CTX ctx;
  SHA1_Init(&ctx);
  SHA1_Update(&ctx, buffer, file_size);
  SHA1_Final(digest, &ctx);

  /*printf("MD5 (\"%s\") = ", test[i]); */
  /*for (int di = 0; di < 16; ++di)
  {
    printf("%02x", digest[di]);
  }*/
  //printf("\t%s\n", filename);
  free(buffer);
  fclose(file);
  return true;
}

bool SHA1::ComputeFile(const char *filename, char digest_str[20*2+1])
{
  // If not file exist
  // return false;
  unsigned char digest[20];

  /* Do the file */
  if( !process_file(filename, digest) )
    {
    return false;
    }

  for (int di = 0; di < 20; ++di)
    {
    sprintf(digest_str+2*di, "%02x", digest[di]);
    }
  digest_str[2*20] = '\0';
  return true;
}


} // end namespace gdcm
