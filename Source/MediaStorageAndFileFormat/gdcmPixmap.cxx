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
#include "gdcmPixmap.h"
#include "gdcmRAWCodec.h"
#include "gdcmSequenceOfFragments.h"
#include "gdcmRAWCodec.h"
#include "gdcmJPEGCodec.h"
#include "gdcmPVRGCodec.h"
#include "gdcmJPEGLSCodec.h"
#include "gdcmJPEG2000Codec.h"
#include "gdcmRLECodec.h"

namespace gdcm
{
/*
 * PICKER-16-MONO2-Nested_icon.dcm:
(0088,0200) SQ (Sequence with undefined length #=1)     # u/l, 1 PixmapSequence
  (fffe,e000) na (Item with undefined length #=10)        # u/l, 1 Item
    (0028,0002) US 1                                        #   2, 1 SamplesPerPixel
    (0028,0004) CS [MONOCHROME2]                            #  12, 1 PhotometricInterpretation
    (0028,0010) US 64                                       #   2, 1 Rows
    (0028,0011) US 64                                       #   2, 1 Columns
    (0028,0034) IS [1\1]                                    #   4, 2 PixelAspectRatio
    (0028,0100) US 8                                        #   2, 1 BitsAllocated
    (0028,0101) US 8                                        #   2, 1 BitsStored
    (0028,0102) US 7                                        #   2, 1 HighBit
    (0028,0103) US 0                                        #   2, 1 PixelRepresentation
    (7fe0,0010) OW 0000\0000\0000\0000\0000\0000\0000\0000\0000\0000\0000\0000\0000... # 4096, 1 PixelData
  (fffe,e00d) na (ItemDelimitationItem)                   #   0, 0 ItemDelimitationItem
(fffe,e0dd) na (SequenceDelimitationItem)               #   0, 0 SequenceDelimitationItem
*/

Pixmap::Pixmap():
  PlanarConfiguration(0),
  NumberOfDimensions(2),
  TS(),
  PF(),
  PI(),
  NeedByteSwap(false),
  Dimensions(),
  LUT(0),
  PixelData() {}

Pixmap::~Pixmap() {}

/*
 * Internal implementation everything assume that NumberOfDimensions was set
 */
unsigned int Pixmap::GetNumberOfDimensions() const
{
  assert( NumberOfDimensions );
  return NumberOfDimensions;
}

void Pixmap::SetNumberOfDimensions(unsigned int dim)
{
  NumberOfDimensions = dim;
  assert( NumberOfDimensions );
  Dimensions.resize( 3 /*NumberOfDimensions*/ ); // fill with 0
  assert( NumberOfDimensions == 2 || NumberOfDimensions == 3 );
  if( NumberOfDimensions == 2 )
    {
    Dimensions[2] = 1;
    //Spacing[2] = 1;
    }
}

const unsigned int *Pixmap::GetDimensions() const
{
  assert( NumberOfDimensions );
  return &Dimensions[0];
}

unsigned int Pixmap::GetDimension(unsigned int idx) const
{
  assert( NumberOfDimensions );
  return Dimensions[idx];
}

void Pixmap::SetDimensions(const unsigned int *dims)
{
  assert( NumberOfDimensions );
  //assert( Dimensions.empty() );
  Dimensions = std::vector<unsigned int>(dims, 
    dims+NumberOfDimensions);
}

void Pixmap::SetDimension(unsigned int idx, unsigned int dim)
{
  assert( NumberOfDimensions );
  assert( idx < NumberOfDimensions );
  Dimensions.resize( 3 /*NumberOfDimensions*/ );
  // Can dim be 0 ??
  // -> no !
  //assert( dim ); // PhilipsLosslessRice.dcm 
  Dimensions[idx] = dim;
  if( NumberOfDimensions == 2 )
    {
    Dimensions[2] = 1;
    }
}

// TODO does it make sense to PlanarConfiguration in Pixmap
// and SamplesPerPixel in PixelFormat when those two are linked...
unsigned int Pixmap::GetPlanarConfiguration() const
{
  if( PlanarConfiguration && PF.GetSamplesPerPixel() != 3 )
    {
    abort();
    // LEADTOOLS_FLOWERS-8-PAL-RLE.dcm
    // User specify PlanarConfiguration whereas SamplesPerPixel != 3
    gdcmWarningMacro( 
      "Can't set PlanarConfiguration if SamplesPerPixel is not 3" );
    // Let's assume it's this way...
    return 0;
    }
  return PlanarConfiguration;
}

void Pixmap::SetPlanarConfiguration(unsigned int pc)
{
  // precondition
  assert( pc == 0 || pc == 1 );
  // LEADTOOLS_FLOWERS-8-MONO2-Uncompressed.dcm   
  if( pc ) assert( PF.GetSamplesPerPixel() == 3 ); // Please set PixelFormat first
  PlanarConfiguration = pc;
  // \postcondition
  assert( PlanarConfiguration == 0 || PlanarConfiguration == 1 );
}



void Pixmap::Clear()
{
  Dimensions.clear();
}

const PhotometricInterpretation &Pixmap::GetPhotometricInterpretation() const
{
  return PI;
}

void Pixmap::SetPhotometricInterpretation(
  PhotometricInterpretation const &pi)
{
  PI = pi;
}

#if 0
bool Pixmap::GetBuffer(char *buffer) const
{
  if( IsEmpty() )
    {
    buffer = 0;
    return false;
    }

  const ByteValue *bv = PixelData.GetByteValue();
  if( !bv )
    {
    // KODAK_CompressedIcon.dcm
    // contains a compressed Icon Sequence, one has to guess this is lossless jpeg...
#ifdef MDEBUG
    const SequenceOfFragments *sqf = PixelData.GetSequenceOfFragments();
    std::ofstream os( "/tmp/kodak.ljpeg");
    sqf->WriteBuffer( os );
#endif
    gdcmWarningMacro( "Compressed Icon are not support for now" );
    buffer = 0;
    return false;
    }
  assert( bv );
  RAWCodec codec;
  //assert( GetPhotometricInterpretation() == PhotometricInterpretation::MONOCHROME2 );
  //codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
  if( GetPhotometricInterpretation() != PhotometricInterpretation::MONOCHROME2 )
    {
    gdcmWarningMacro( "PhotometricInterpretation: " << GetPhotometricInterpretation() << " not handled for now" );
    }
  codec.SetPhotometricInterpretation( PhotometricInterpretation::MONOCHROME2 );
  codec.SetPixelFormat( GetPixelFormat() );
  codec.SetPlanarConfiguration( 0 );
  DataElement out;
  bool r = codec.Decode(PixelData, out);
  assert( r );
  const ByteValue *outbv = out.GetByteValue();
  assert( outbv );
  //unsigned long check = outbv->GetLength();  // FIXME
  memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME
  return r;
}
#endif

unsigned long Pixmap::GetBufferLength() const
{
  assert( NumberOfDimensions );
  //assert( NumberOfDimensions == Dimensions.size() );
  if( NumberOfDimensions != Dimensions.size() )
    {
    assert( Dimensions[2] == 1 );
    }
  unsigned long len = 0;
  unsigned int mul = 1;
  // First multiply the dimensions:
  std::vector<unsigned int>::const_iterator it = Dimensions.begin();
  for(; it != Dimensions.end(); ++it)
    {
    mul *= *it;
    }
  // Multiply by the pixel size:
  // Special handling of packed format:
  if( PF == PixelFormat::UINT12 )
    {
    assert( PF.GetSamplesPerPixel() == 1 );
    unsigned int save = mul;
    save *= 12;
    save /= 8;
    assert( save * 8 / 12 == mul );
    mul = save;
    }
  else if( PF.GetBitsAllocated() % 8 != 0 )
    {
    // gdcmDataExtra/gdcmSampleData/images_of_interest/USBitsAllocated14.dcm
    // BitsAllocated      :14
    // BitsStored         :14
    // HighBit            :13
    assert( PF.GetSamplesPerPixel() == 1 );
    const ByteValue *bv = PixelData.GetByteValue();
    assert( bv );
    unsigned int ref = bv->GetLength() / mul;
    assert( bv->GetLength() % mul == 0 );
    mul *= ref;
    }
  else
    {
    mul *= PF.GetPixelSize();
    }
  len = mul;

  assert( len != 0 );
  return len;
}

bool Pixmap::TryRAWCodec(char *buffer, bool &lossyflag) const
{
  if(!buffer)
    {
    lossyflag = false;
    return true;
    }
  const TransferSyntax &ts = GetTransferSyntax();

  const ByteValue *bv = PixelData.GetByteValue();
  if( bv )
    {
    unsigned long len = GetBufferLength();
    RAWCodec codec;
    if( !codec.CanDecode( ts ) ) return false;
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetLUT( GetLUT() );
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetNeedByteSwap( GetNeedByteSwap() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    DataElement out;
    bool r = codec.Decode(PixelData, out);
    if( !r ) return false;
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    if( len != bv->GetLength() )
      {
      // SIEMENS_GBS_III-16-ACR_NEMA_1.acr
      gdcmDebugMacro( "Pixel Length " << bv->GetLength() <<
        " is different from computed value " << len );
      ((ByteValue*)outbv)->SetLength( len );
      }
    unsigned long check = outbv->GetLength();  // FIXME
    assert( check == len );
    if(buffer) memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME
    return r;
    }
  return false;
}

bool Pixmap::TryJPEGCodec(char *buffer, bool &lossyflag) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  JPEGCodec codec;
  if( codec.CanDecode( ts ) )
    {
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    DataElement out;
    bool r = codec.Decode(PixelData, out);
    // PHILIPS_Gyroscan-12-MONO2-Jpeg_Lossless.dcm    
    if( !r )
      {
      return false;
      }
    // FIXME ! This should be done all the time for all codec:
    // Did PI change or not ?
    if ( GetPhotometricInterpretation() != codec.GetPhotometricInterpretation() )
      {
      // HACK
      // YBRisGray.dcm 
      gdcm::Pixmap *i = (gdcm::Pixmap*)this;
      i->SetPhotometricInterpretation( codec.GetPhotometricInterpretation() );
      }
    if ( GetPhotometricInterpretation() == PhotometricInterpretation::YBR_FULL_422 )
      {
      gdcm::Pixmap *i = (gdcm::Pixmap*)this;
      i->SetPhotometricInterpretation( PhotometricInterpretation::RGB );
      }
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    unsigned long check = outbv->GetLength();  // FIXME
    // DermaColorLossLess.dcm has a len of 63531, but DICOM will give us: 63532 ...
    assert( len <= outbv->GetLength() );
    if(buffer) memcpy(buffer, outbv->GetPointer(), len /*outbv->GetLength()*/ );  // FIXME

    lossyflag = codec.IsLossy();
    //assert( codec.IsLossy() == ts.IsLossy() );

    return true;
    }
  return false;
}

bool Pixmap::TryJPEGCodec2(std::ostream &os) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  JPEGCodec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( GetDimensions() );
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    DataElement out;
    bool r = codec.Code(PixelData, out);
    // PHILIPS_Gyroscan-12-MONO2-Jpeg_Lossless.dcm    
    if( !r )
      {
      return false;
      }
    // FIXME ! This should be done all the time for all codec:
    // Did PI change or not ?
    if ( GetPhotometricInterpretation() != codec.GetPhotometricInterpretation() )
      {
      // HACK
      //gdcm::Pixmap *i = (gdcm::Pixmap*)this;
      //i->SetPhotometricInterpretation( codec.GetPhotometricInterpretation() );
      }
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    unsigned long check = outbv->GetLength();  // FIXME
    // DermaColorLossLess.dcm has a len of 63531, but DICOM will give us: 63532 ...
    assert( outbv->GetLength() < len );
    //memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );
    os.write( outbv->GetPointer(), outbv->GetLength() );

    return true;
    }
  return false;
}

bool Pixmap::TryPVRGCodec(char *buffer, bool &lossyflag) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  PVRGCodec codec;
  if( codec.CanDecode( ts ) )
    {
    codec.SetPixelFormat( GetPixelFormat() );
    //codec.SetBufferLength( len );
    //codec.SetNumberOfDimensions( GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    codec.SetDimensions( GetDimensions() );
    DataElement out;
    bool r = codec.Decode(PixelData, out);
    if(!r) return false;
    assert( r );
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    unsigned long check = outbv->GetLength();  // FIXME
    if(buffer) memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME

    lossyflag = codec.IsLossy();
    //assert( codec.IsLossy() == ts.IsLossy() );

    return r;
    }
  return false;
}
   
bool Pixmap::TryJPEGLSCodec(char *buffer, bool &lossyflag) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  JPEGLSCodec codec;
  if( codec.CanDecode( ts ) )
    {
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetBufferLength( len );
    codec.SetNumberOfDimensions( GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    codec.SetDimensions( GetDimensions() );
    DataElement out;
    bool r = codec.Decode(PixelData, out);
    if( !r ) return false;
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    unsigned long check = outbv->GetLength();  // FIXME
    if(buffer) memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME

    //assert( codec.IsLossy() == ts.IsLossy() );
    lossyflag = codec.IsLossy();
    if( codec.IsLossy() != ts.IsLossy() )
      {
      gdcmErrorMacro( "EVIL file, it is declared as lossless but is in fact lossy." );
      }

    return r;
    }
  return false;
}

bool Pixmap::IsLossy() const
{
  // FIXME each call is expensive...
  bool lossyflag;
  if( this->GetBufferInternal(0, lossyflag) )
    {
    return lossyflag;
    }
  return false;
}

bool Pixmap::TryJPEG2000Codec(char *buffer, bool &lossyflag) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  JPEG2000Codec codec;
  if( codec.CanDecode( ts ) )
    {
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetNumberOfDimensions( GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    codec.SetDimensions( GetDimensions() );
    DataElement out;
    bool r = codec.Decode(PixelData, out);
    if(!r) return false;
    assert( r );
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    unsigned long check = outbv->GetLength();  // FIXME
    if(buffer) memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME

    lossyflag = codec.IsLossy();
    if( codec.IsLossy() != ts.IsLossy() )
      {
      gdcmErrorMacro( "EVIL file, it is declared as lossless but is in fact lossy." );
      }
    return r;
    }
  return false;
}

bool Pixmap::TryJPEG2000Codec2(std::ostream &os) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  JPEG2000Codec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( GetDimensions() );
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetNumberOfDimensions( GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    DataElement out;
    bool r = codec.Code(PixelData, out);
    assert( r );
    const ByteValue *outbv = out.GetByteValue();
    assert( outbv );
    unsigned long check = outbv->GetLength();  // FIXME
    //memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME
    os.write(outbv->GetPointer(), outbv->GetLength() );
    return r;
    }
  return false;
}

bool Pixmap::TryRLECodec(char *buffer, bool &lossyflag ) const
{
  unsigned long len = GetBufferLength();
  const TransferSyntax &ts = GetTransferSyntax();

  RLECodec codec;
  if( codec.CanDecode( ts ) )
    {
    //assert( sf->GetNumberOfFragments() == 1 );
    //assert( sf->GetNumberOfFragments() == GetDimensions(2) );
    codec.SetNumberOfDimensions( GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( GetPhotometricInterpretation() );
    codec.SetPixelFormat( GetPixelFormat() );
    codec.SetLUT( GetLUT() );
    codec.SetNeedOverlayCleanup( AreOverlaysInPixelData() );
    codec.SetBufferLength( len );
    DataElement out;
    bool r = codec.Decode(PixelData, out);
    assert( r );
    const ByteValue *outbv = out.GetByteValue();
    //unsigned long check = outbv->GetLength();  // FIXME
    if(buffer) memcpy(buffer, outbv->GetPointer(), outbv->GetLength() );  // FIXME
    lossyflag = false;
    return true;
    }
  return false;
}

// Acces the raw data
bool Pixmap::GetBuffer(char *buffer) const
{
  bool dummy;
  return GetBufferInternal(buffer, dummy);
}

bool Pixmap::GetBufferInternal(char *buffer, bool &lossyflag) const
{
  bool success = false;
  if( !success ) success = TryRAWCodec(buffer, lossyflag);
  if( !success ) success = TryJPEGCodec(buffer, lossyflag);
  if( !success ) success = TryPVRGCodec(buffer, lossyflag); // AFTER IJG trial !
  if( !success ) success = TryJPEG2000Codec(buffer, lossyflag);
  if( !success ) success = TryJPEGLSCodec(buffer, lossyflag);
  if( !success ) success = TryRLECodec(buffer, lossyflag);
  //if( !success ) success = TryDeltaEncodingCodec(buffer);
  if( !success )
    {
    buffer = 0;
    //throw Exception( "No codec found for this image");
    }

  return success;
}

// Compress the raw data
bool Pixmap::GetBuffer2(std::ostream &os) const
{
  bool success = false;
  //if( !success ) success = TryRAWCodec2(buffer);
  if( !success ) success = TryJPEGCodec2(os);
  //if( !success ) success = TryJPEG2000Codec2(os);
  //if( !success ) success = TryRLECodec2(buffer);
  if( !success )
    {
    //buffer = 0;
    throw Exception( "No codec found for this image");
    }

  return success;
}


}
