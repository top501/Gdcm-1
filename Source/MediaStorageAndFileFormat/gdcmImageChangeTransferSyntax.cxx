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
#include "gdcmImageChangeTransferSyntax.h"
#include "gdcmSequenceOfFragments.h"
#include "gdcmSequenceOfItems.h"
#include "gdcmFragment.h"
#include "gdcmRAWCodec.h"
#include "gdcmJPEGCodec.h"
#include "gdcmJPEGLSCodec.h"
#include "gdcmJPEG2000Codec.h"
#include "gdcmRLECodec.h"

namespace gdcm
{

/*
bool ImageChangeTransferSyntax::TryRAWCodecIcon(const DataElement &pixelde)
{
  unsigned long len = Input->GetIconImage().GetBufferLength();
  //assert( len == pixelde.GetByteValue()->GetLength() );
  const TransferSyntax &ts = GetTransferSyntax();

  RAWCodec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( Input->GetIconImage().GetDimensions() );
    codec.SetPlanarConfiguration( Input->GetIconImage().GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( Input->GetIconImage().GetPhotometricInterpretation() );
    codec.SetPixelFormat( Input->GetIconImage().GetPixelFormat() );
    codec.SetNeedOverlayCleanup( Input->GetIconImage().AreOverlaysInPixelData() );
    DataElement out;
    //bool r = codec.Code(Input->GetDataElement(), out);
    bool r = codec.Code(pixelde, out);

    DataElement &de = Output->GetIconImage().GetDataElement();
    de.SetValue( out.GetValue() );
    if( !r )
      {
      return false;
      }
    return true;
    }
  return false;
}
*/

bool ImageChangeTransferSyntax::TryRAWCodec(const DataElement &pixelde, Pixmap const &input, Pixmap &output)
{
  unsigned long len = input.GetBufferLength();
  //assert( len == pixelde.GetByteValue()->GetLength() );
  const TransferSyntax &ts = GetTransferSyntax();

  RAWCodec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( input.GetDimensions() );
    codec.SetPlanarConfiguration( input.GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( input.GetPhotometricInterpretation() );
    codec.SetPixelFormat( input.GetPixelFormat() );
    codec.SetNeedOverlayCleanup( input.AreOverlaysInPixelData() );
    DataElement out;
    //bool r = codec.Code(input.GetDataElement(), out);
    bool r = codec.Code(pixelde, out);

    DataElement &de = output.GetDataElement();
    de.SetValue( out.GetValue() );
    if( !r )
      {
      return false;
      }
    return true;
    }
  return false;
}

bool ImageChangeTransferSyntax::TryRLECodec(const DataElement &pixelde, Pixmap const &input, Pixmap &output)
{
  unsigned long len = input.GetBufferLength();
  //assert( len == pixelde.GetByteValue()->GetLength() );
  const TransferSyntax &ts = GetTransferSyntax();

  RLECodec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( input.GetDimensions() );
    codec.SetPlanarConfiguration( input.GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( input.GetPhotometricInterpretation() );
    codec.SetPixelFormat( input.GetPixelFormat() );
    codec.SetNeedOverlayCleanup( input.AreOverlaysInPixelData() );
    DataElement out;
    //bool r = codec.Code(input.GetDataElement(), out);
    bool r = codec.Code(pixelde, out);

    DataElement &de = output.GetDataElement();
    de.SetValue( out.GetValue() );
    if( !r )
      {
      return false;
      }
    return true;
    }
  return false;
}

bool ImageChangeTransferSyntax::TryJPEGCodec(const DataElement &pixelde, Pixmap const &input, Pixmap &output)
{
  unsigned long len = input.GetBufferLength();
  //assert( len == pixelde.GetByteValue()->GetLength() );
  const TransferSyntax &ts = GetTransferSyntax();

  JPEGCodec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( input.GetDimensions() );
    // FIXME: GDCM always apply the planar configuration to 0...
    //if( input.GetPlanarConfiguration() )
    //  {
    //  output.SetPlanarConfiguration( 0 );
    //  }
    codec.SetPlanarConfiguration( input.GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( input.GetPhotometricInterpretation() );
    codec.SetPixelFormat( input.GetPixelFormat() );
    codec.SetNeedOverlayCleanup( input.AreOverlaysInPixelData() );
    DataElement out;
    //bool r = codec.Code(input.GetDataElement(), out);
    bool r = codec.Code(pixelde, out);
    // FIXME: this is not the best place to change the Output image internal type,
    // but since I know IJG is always applying the Planar Configuration, it does make
    // any sense to EVER produce a JPEG image where the Planar Configuration would be one
    // so let's be nice and actually sync JPEG configuration with DICOM Planar Conf.
    output.SetPlanarConfiguration( 0 );
    //output.SetPhotometricInterpretation( PhotometricInterpretation::RGB );

    DataElement &de = output.GetDataElement();
    de.SetValue( out.GetValue() );
    // PHILIPS_Gyroscan-12-MONO2-Jpeg_Lossless.dcm    
    if( !r )
      {
      return false;
      }
    // When compressing with JPEG I think planar should always be:
    //output.SetPlanarConfiguration(0);
    // FIXME ! This should be done all the time for all codec:
    // Did PI change or not ?
    if ( output.GetPhotometricInterpretation() != codec.GetPhotometricInterpretation() )
      {
      // HACK
      //gdcm::Image *i = (gdcm::Image*)this;
      //i->SetPhotometricInterpretation( codec.GetPhotometricInterpretation() );
abort();
      }
    return true;
    }
  return false;
}

bool ImageChangeTransferSyntax::TryJPEGLSCodec(const DataElement &pixelde, Pixmap const &input, Pixmap &output)
{
  unsigned long len = input.GetBufferLength();
  //assert( len == pixelde.GetByteValue()->GetLength() );
  const TransferSyntax &ts = GetTransferSyntax();

  JPEGLSCodec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( input.GetDimensions() );
    codec.SetPixelFormat( input.GetPixelFormat() );
    //codec.SetNumberOfDimensions( input.GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( input.GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( input.GetPhotometricInterpretation() );
    codec.SetNeedOverlayCleanup( input.AreOverlaysInPixelData() );
    DataElement out;
    //bool r = codec.Code(input.GetDataElement(), out);
    bool r = codec.Code(pixelde, out);

    DataElement &de = output.GetDataElement();
    de.SetValue( out.GetValue() );
    assert( r );
    return r;
    }
  return false;
}

bool ImageChangeTransferSyntax::TryJPEG2000Codec(const DataElement &pixelde, Pixmap const &input, Pixmap &output)
{
  unsigned long len = input.GetBufferLength();
  //assert( len == pixelde.GetByteValue()->GetLength() );
  const TransferSyntax &ts = GetTransferSyntax();

  JPEG2000Codec codec;
  if( codec.CanCode( ts ) )
    {
    codec.SetDimensions( input.GetDimensions() );
    codec.SetPixelFormat( input.GetPixelFormat() );
    codec.SetNumberOfDimensions( input.GetNumberOfDimensions() );
    codec.SetPlanarConfiguration( input.GetPlanarConfiguration() );
    codec.SetPhotometricInterpretation( input.GetPhotometricInterpretation() );
    codec.SetNeedOverlayCleanup( input.AreOverlaysInPixelData() );
    DataElement out;
    //bool r = codec.Code(input.GetDataElement(), out);
    bool r = codec.Code(pixelde, out);

    DataElement &de = output.GetDataElement();
    de.SetValue( out.GetValue() );
    assert( r );
    return r;
    }
  return false;
}

bool ImageChangeTransferSyntax::Change()
{
  Output = Input;

  // Fast path
  if( Input->GetTransferSyntax() == TS && !Force ) return true;

  // FIXME
  // For now only support raw input, otherwise we would need to first decompress them
  if( Input->GetTransferSyntax() != TransferSyntax::ImplicitVRLittleEndian 
    && Input->GetTransferSyntax() != TransferSyntax::ExplicitVRLittleEndian
    && Input->GetTransferSyntax() != TransferSyntax::ExplicitVRBigEndian
    || Force )
    {
    // In memory decompression:
    gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
    gdcm::ByteValue *bv = new gdcm::ByteValue();
    unsigned long len = Input->GetBufferLength();
    bv->SetLength( len );
    bool b = Input->GetBuffer( (char*)bv->GetPointer() );
    if( !b )
      {
      return false;
      }
    pixeldata.SetValue( *bv );

    // same goes for icon
    gdcm::DataElement iconpixeldata( gdcm::Tag(0x7fe0,0x0010) );
{
    gdcm::ByteValue *bv = new gdcm::ByteValue();
    unsigned long len = Input->GetIconImage().GetBufferLength();
    bv->SetLength( len );
    bool b = Input->GetIconImage().GetBuffer( (char*)bv->GetPointer() );
    if( !b )
      {
      return false;
      }
    iconpixeldata.SetValue( *bv );
}

    bool success = false;
    if( !success ) success = TryRAWCodec(pixeldata, *Input, *Output);
    if( !success ) success = TryJPEGCodec(pixeldata, *Input, *Output);
    if( !success ) success = TryJPEGLSCodec(pixeldata, *Input, *Output);
    if( !success ) success = TryJPEG2000Codec(pixeldata, *Input, *Output);
    if( !success ) success = TryRLECodec(pixeldata, *Input, *Output);
    Output->SetTransferSyntax( TS );
    if( !success )
      {
      //abort();
      return false;
      }

    // same goes for icon
    success = false;
    if( !success ) success = TryRAWCodec(iconpixeldata, Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryJPEGCodec(iconpixeldata, Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryJPEGLSCodec(iconpixeldata, Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryJPEG2000Codec(iconpixeldata, Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryRLECodec(iconpixeldata, Input->GetIconImage(), Output->GetIconImage());
    Output->GetIconImage().SetTransferSyntax( TS );
    if( !success )
      {
      //abort();
      return false;
      }

    assert( Output->GetTransferSyntax() == TS );
    assert( Output->GetIconImage().GetTransferSyntax() == TS );
    return success;
    }

  // too bad we actually have to do some work...
  bool success = false;
  if( !success ) success = TryRAWCodec(Input->GetDataElement(), *Input, *Output);
  if( !success ) success = TryJPEGCodec(Input->GetDataElement(), *Input, *Output);
  if( !success ) success = TryJPEG2000Codec(Input->GetDataElement(), *Input, *Output);
  if( !success ) success = TryJPEGLSCodec(Input->GetDataElement(), *Input, *Output);
  if( !success ) success = TryRLECodec(Input->GetDataElement(), *Input, *Output);
  Output->SetTransferSyntax( TS );
  if( !success )
    {
    //abort();
    return false;
    }

  if( CompressIconImage )
  {
    // same goes for icon
    success = false;
    if( !success ) success = TryRAWCodec(Input->GetIconImage().GetDataElement(), Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryJPEGCodec(Input->GetIconImage().GetDataElement(), Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryJPEGLSCodec(Input->GetIconImage().GetDataElement(), Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryJPEG2000Codec(Input->GetIconImage().GetDataElement(), Input->GetIconImage(), Output->GetIconImage());
    if( !success ) success = TryRLECodec(Input->GetIconImage().GetDataElement(), Input->GetIconImage(), Output->GetIconImage());
    Output->GetIconImage().SetTransferSyntax( TS );
    if( !success )
      {
      //abort();
      return false;
      }
  assert( Output->GetIconImage().GetTransferSyntax() == TS );
  }

  assert( Output->GetTransferSyntax() == TS );
  return success;
}


} // end namespace gdcm

