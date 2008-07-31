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
#include "gdcmFileMetaInformation.h"
#include "gdcmAttribute.h"
#include "gdcmVR.h"
#include "gdcmExplicitDataElement.h"
#include "gdcmImplicitDataElement.h"
#include "gdcmByteValue.h"
#include "gdcmSwapper.h"
#include "gdcmException.h"
#include "gdcmTagToType.h"

#include "gdcmTag.h"

namespace gdcm
{

const char FileMetaInformation::GDCM_FILE_META_INFORMATION_VERSION[] = "\0\1";

const char FileMetaInformation::GDCM_IMPLEMENTATION_CLASS_UID[] = "1.2.826.0.1.3680043.2.1143.107.104.103.115." GDCM_VERSION;
const char FileMetaInformation::GDCM_IMPLEMENTATION_VERSION_NAME[] = "GDCM " GDCM_VERSION;
const char FileMetaInformation::GDCM_SOURCE_APPLICATION_ENTITY_TITLE[] = "GDCM";
// Default initialize those static std::string, with GDCM values:
std::string FileMetaInformation::ImplementationClassUID = GetGDCMImplementationClassUID();
std::string FileMetaInformation::ImplementationVersionName = GetGDCMImplementationVersionName();
std::string FileMetaInformation::SourceApplicationEntityTitle = GetGDCMSourceApplicationEntityTitle();

const char * FileMetaInformation::GetFileMetaInformationVersion()
{
  return GDCM_FILE_META_INFORMATION_VERSION;
}
const char * FileMetaInformation::GetGDCMImplementationClassUID()
{
  return GDCM_IMPLEMENTATION_CLASS_UID;
}
const char * FileMetaInformation::GetGDCMImplementationVersionName()
{
  return GDCM_IMPLEMENTATION_VERSION_NAME;
}
const char * FileMetaInformation::GetGDCMSourceApplicationEntityTitle()
{
  return GDCM_SOURCE_APPLICATION_ENTITY_TITLE;
}

void FileMetaInformation::SetImplementationClassUID(const char * imp)
{
abort();
}

void FileMetaInformation::AppendImplementationClassUID(const char * imp)
{
  ImplementationClassUID = GetGDCMImplementationClassUID();
  ImplementationClassUID += ".";
  ImplementationClassUID += imp;
}
void FileMetaInformation::SetImplementationVersionName(const char * version)
{
  // Simply override the value since we cannot have more than 16bytes...
  assert( strlen(version) <= 16 );
  //ImplementationVersionName = GetGDCMImplementationVersionName();
  //ImplementationVersionName += "-";
  //ImplementationVersionName += version;
  ImplementationVersionName = version;
}
void FileMetaInformation::SetSourceApplicationEntityTitle(const char * title)
{
  //SourceApplicationEntityTitle = GetGDCMSourceApplicationEntityTitle();
  //SourceApplicationEntityTitle += "/";
  SourceApplicationEntityTitle = title;
}
const char *FileMetaInformation::GetImplementationClassUID()
{
  return ImplementationClassUID.c_str();
}
const char *FileMetaInformation::GetImplementationVersionName()
{
  return ImplementationVersionName.c_str();
}
const char *FileMetaInformation::GetSourceApplicationEntityTitle()
{
  return SourceApplicationEntityTitle.c_str();
}

void FileMetaInformation::FillFromDataSet(DataSet const &ds)
{
  // Example: CR-MONO1-10-chest.dcm is missing a file meta header:
  DataElement xde;
  // File Meta Information Version (0002,0001) -> computed
  if( !FindDataElement( Tag(0x0002, 0x0001) ) )
    {
    xde.SetTag( Tag(0x0002, 0x0001) );
    xde.SetVR( VR::OB );
    const char *version = FileMetaInformation::GetFileMetaInformationVersion();
    xde.SetByteValue( version, 2 /*strlen(version)*/ );
    Insert( xde );
    }
  else
    {
    const DataElement &de = GetDataElement( Tag(0x0002,0x0001) );
    const ByteValue *bv = de.GetByteValue();
    if( bv->GetLength() != 2 
      || memcmp( bv->GetPointer(), FileMetaInformation::GetFileMetaInformationVersion(), 2 ) != 0 )
      {
      xde.SetTag( Tag(0x0002, 0x0001) );
      xde.SetVR( VR::OB );
      const char *version = FileMetaInformation::GetFileMetaInformationVersion();
      xde.SetByteValue( version, 2 /*strlen(version)*/ );
      Replace( xde );
      }
    }
  // Media Storage SOP Class UID (0002,0002) -> see (0008,0016)
  if( !FindDataElement( Tag(0x0002, 0x0002) ) )
    {
    if( !ds.FindDataElement( Tag(0x0008, 0x0016) ) )
      {
      abort();
      }
    else
      {
      const DataElement& msclass = ds.GetDataElement( Tag(0x0008, 0x0016) );
      xde = msclass;
      xde.SetTag( Tag(0x0002, 0x0002) );
      if( msclass.GetVR() == VR::UN || msclass.GetVR() == VR::INVALID )
        {
        xde.SetVR( VR::UI );
        }
      Insert( xde );
      }
    }
  else // Ok there is a value in (0002,0002) let see if it match (0008,0016)
    {
    if( !ds.FindDataElement( Tag(0x0008, 0x0016) ) )
      {
      abort();
      }
    const DataElement& sopclass = ds.GetDataElement( Tag(0x0008, 0x0016) );
    DataElement mssopclass = GetDataElement( Tag(0x0002, 0x0002) );
    const ByteValue *bv = sopclass.GetByteValue();
    mssopclass.SetByteValue( bv->GetPointer(), bv->GetLength() );
    Replace( mssopclass );
    }
  // Media Storage SOP Instance UID (0002,0003) -> see (0008,0018)
  const DataElement &dummy = GetDataElement(Tag(0x0002,0x0003));
  if( !FindDataElement( Tag(0x0002, 0x0003) ) || GetDataElement( Tag(0x0002,0x0003) ).IsEmpty() )
    {
    if( ds.FindDataElement( Tag(0x0008, 0x0018) ) )
      {
      const DataElement& msinst = ds.GetDataElement( Tag(0x0008, 0x0018) );
      if( msinst.IsEmpty() )
        {
        // Ok there is nothing...
        //UIDGenerator uid;
        //const char *s = uid.Generate();
        //xde.SetByteValue( s, strlen(s) );
        // FIXME somebody before should make sure there is something...
        xde = msinst;
        }
      else
        {
        xde = msinst;
        }
      xde.SetTag( Tag(0x0002, 0x0003) );
      if( msinst.GetVR() == VR::UN || msinst.GetVR() == VR::INVALID )
        {
        xde.SetVR( VR::UI );
        }
      Replace( xde );
      }
    else
      {
      abort();
      }
    }
  else // Ok there is a value in (0002,0003) let see if it match (0008,0018)
    {
    if( !ds.FindDataElement( Tag(0x0008, 0x0018) ) )
      {
      abort();
      }
    const DataElement& sopinst = ds.GetDataElement( Tag(0x0008, 0x0018) );
    DataElement mssopinst = GetDataElement( Tag(0x0002, 0x0003) );
    const ByteValue *bv = sopinst.GetByteValue();
    mssopinst.SetByteValue( bv->GetPointer(), bv->GetLength() );
    Replace( mssopinst );
    }
  //assert( !GetDataElement( Tag(0x0002,0x0003) ).IsEmpty() );
  // Transfer Syntax UID (0002,0010) -> ??? (computed at write time at most)
  if( FindDataElement( Tag(0x0002, 0x0010) ) )
    {
    const DataElement& tsuid = GetDataElement( Tag(0x0002, 0x0010) );
    if( tsuid.GetVR() != VR::UI )
      {
      xde = tsuid;
      xde.SetVR( VR::UI );
      Replace( xde );
      }
    }
  else
    {
    // Very bad !!
    throw Exception( "No (0002,0010) element found" );
    }
  // Implementation Class UID (0002,0012) -> ??
  if( !FindDataElement( Tag(0x0002, 0x0012) ) )
    {
    xde.SetTag( Tag(0x0002, 0x0012) );
    xde.SetVR( VR::UI );
    //const char implementation[] = GDCM_IMPLEMENTATION_CLASS_UID;
    const char *implementation = FileMetaInformation::GetImplementationClassUID();
    xde.SetByteValue( implementation, strlen(implementation) );
    Insert( xde );
    }
  // Implementation Version Name (0002,0013) -> ??
  if( !FindDataElement( Tag(0x0002, 0x0013) ) )
    {
    xde.SetTag( Tag(0x0002, 0x0013) );
    xde.SetVR( VR::SH );
    //const char version[] = GDCM_IMPLEMENTATION_VERSION_NAME;
    const char *version = FileMetaInformation::GetImplementationVersionName();
    xde.SetByteValue( version, strlen(version) );
    Insert( xde );
    }
  // Source Application Entity Title (0002,0016) -> ??
  if( !FindDataElement( Tag(0x0002, 0x0016) ) )
    {
    xde.SetTag( Tag(0x0002, 0x0016) );
    xde.SetVR( VR::AE );
    //const char title[] = GDCM_SOURCE_APPLICATION_ENTITY_TITLE;
    const char *title = FileMetaInformation::GetSourceApplicationEntityTitle();
    xde.SetByteValue(title, strlen(title) );
    Insert( xde );
    }
  // Do this one last !
  // (Meta) Group Length (0002,0000) -> computed
  Attribute<0x0002, 0x0000> filemetagrouplength;
  Remove( filemetagrouplength.GetTag() );
  unsigned int glen = GetLength<ExplicitDataElement>();
  assert( (glen % 2) == 0 );
  filemetagrouplength.SetValue( glen );
  Insert( filemetagrouplength.GetAsDataElement() );

  assert( !IsEmpty() );
}

// FIXME
// This code should clearly be rewritten with some template meta programing to 
// enable reuse of code...
//
// \postcondition after the file meta information (well before the dataset...)
template <typename TSwap>
bool ReadExplicitDataElement(std::istream &is, ExplicitDataElement &de)
{
  // Read Tag
  std::streampos start = is.tellg();
  //std::cout << "Start: " << start << std::endl;
  Tag t;
  if( !t.template Read<TSwap>(is) )
    {
    assert(0 && "Should not happen" );
    return false;
    }
  //std::cout << "Tag: " << t << std::endl;
  if( t.GetGroup() != 0x0002 )
    {
    gdcmDebugMacro( "Done reading File Meta Information" );
    std::streampos currentpos = is.tellg();
    // old code was fseeking from the beginning of file
    // which seems to be quite different than fseeking in reverse from
    // the current position... ???
    //is.seekg( start, std::ios::beg );
    assert( (start - currentpos) <= 0);
    assert( (int)(start - currentpos) == -4 );
    is.seekg( (start - currentpos), std::ios::cur );
    return false;
    }
  // Read VR
  VR vr;
  if( !vr.Read(is) )
    {
    is.seekg( start, std::ios::beg );
    return false;
    }
  //std::cout << "VR : " << vr << std::endl;
  // Read Value Length
  VL vl;
  if( vr & VR::VL32 )
    {
    if( !vl.template Read<TSwap>(is) )
      {
      assert(0 && "Should not happen");
      return false;
      }
    }
  else
    {
    // Value Length is stored on 16bits only
    vl.template Read16<TSwap>(is);
    }
  //gdcmDebugMacro( "VL : " << vl );
  // Read the Value
  ByteValue *bv = NULL;
  if( vr == VR::SQ )
    {
    assert(0 && "Should not happen");
    return false;
    }
  else if( vl.IsUndefined() )
    {
    assert(0 && "Should not happen");
    return false;
    }
  else
    {
    bv = new ByteValue;
    }
  // We have the length we should be able to read the value
  bv->SetLength(vl); // perform realloc
  if( !bv->template Read<TSwap>(is) )
    {
    assert(0 && "Should not happen");
    return false;
    }
  //std::cout << "Value : ";
  //bv->Print( std::cout );
  //std::cout << std::endl;

  de.SetTag(t);
  de.SetVR(vr);
  de.SetVL(vl);
  de.SetValue(*bv);

  return true;
}

template <typename TSwap>
bool ReadImplicitDataElement(std::istream &is, ImplicitDataElement &de)
{
  // See PS 3.5, 7.1.3 Data Element Structure With Implicit VR
  std::streampos start = is.tellg();
  // Read Tag
  Tag t;
  if( !t.template Read<TSwap>(is) )
    {
    assert(0 && "Should not happen");
    return false;
    }
  //std::cout << "Tag: " << t << std::endl;
  if( t.GetGroup() != 0x0002 )
    {
    gdcmDebugMacro( "Done reading File Meta Information" );
    is.seekg( start, std::ios::beg );
    return false;
    }
  // Read Value Length
  VL vl;
  if( !vl.template Read<TSwap>(is) )
    {
    assert(0 && "Should not happen");
    return false;
    }
  ByteValue *bv = 0;
  if( vl.IsUndefined() )
    {
    assert(0 && "Should not happen");
    return false;
    }
  else
    {
    bv = new ByteValue;
    }
  // We have the length we should be able to read the value
  bv->SetLength(vl); // perform realloc
  if( !bv->template Read<TSwap>(is) )
    {
    assert(0 && "Should not happen");
    return false;
    }
  de.SetTag(t);
  de.SetVL(vl);
  de.SetValue(*bv);

  return true;
}

/*
 * Except for the 128 bytes preamble and the 4 bytes prefix, the File Meta 
 * Information shall be encoded using the Explicit VR Little Endian Transfer
 * Syntax (UID=1.2.840.10008.1.2.1) as defined in DICOM PS 3.5.
 * Values of each File Meta Element shall be padded when necessary to achieve
 * an even length as specified in PS 3.5 by their corresponding Value
 * Representation. For compatibility with future versions of this Standard, 
 * any Tag (0002,xxxx) not defined in Table 7.1-1 shall be ignored.
 * Values of all Tags (0002,xxxx) are reserved for use by this Standard and
 * later versions of DICOM.
 * Note: PS 3.5 specifies that Elements with Tags (0001,xxxx), (0003,xxxx),
 * (0005,xxxx), and (0007,xxxx) shall not be used.
 */
/// \TODO FIXME
/// For now I do a Seek back of 6 bytes. It would be better to finish reading 
/// the first element of the FMI so that I can read the group length and 
/// therefore compare it against the actual value we found...
std::istream &FileMetaInformation::Read(std::istream &is)
{
  //ExplicitAttribute<0x0002,0x0000> metagl;
  //metagl.Read(is);

  // TODO: Can now load data from std::ios::cur to std::ios::cur + metagl.GetValue()

  ExplicitDataElement xde;
  Tag gl;
  gl.Read<SwapperNoOp>(is);
  if( gl.GetGroup() != 0x2 ) throw Exception( "INVALID" );
  if( gl.GetElement() != 0x0 ) throw Exception( "INVALID" );
  VR vr;
  vr.Read(is);
  if( vr == VR::INVALID ) throw Exception( "INVALID" );
  if( vr != VR::UL ) throw Exception( "INVALID" );
  // TODO FIXME: I should not do seekg for valid file this is costly
  is.seekg(-6,std::ios::cur);
  xde.Read<SwapperNoOp>(is);
  Insert( xde );
  // See PS 3.5, Data Element Structure With Explicit VR
  while( ReadExplicitDataElement<SwapperNoOp>(is, xde ) )
    {
    Insert( xde );
    }

  // Now is a good time to compute the transfer syntax:
  ComputeDataSetTransferSyntax();

  // we are at the end of the meta file information and before the dataset
  return is;
}

std::istream &FileMetaInformation::ReadCompat(std::istream &is)
{
  // First off save position in case we fail (no File Meta Information)
  // See PS 3.5, Data Element Structure With Explicit VR
  if( !IsEmpty() )
    {
    throw Exception( "Serious bug" );
    }
  Tag t;
  if( !t.Read<SwapperNoOp>(is) )
    {
    throw Exception( "Cannot read very first tag" );
    return is;
    }
  if( t.GetGroup() == 0x0002 )
    {
    // GE_DLX-8-MONO2-PrivateSyntax.dcm is in Implicit...
    return ReadCompatInternal<SwapperNoOp>(is);
    }
  else if( t.GetGroup() == 0x0008 ) // 
    {
    char vr_str[3];
    is.read(vr_str, 2);
    vr_str[2] = '\0';
    VR::VRType vr = VR::GetVRType(vr_str);
    if( vr != VR::VR_END )
      {
      // File start with a 0x0008 element but no FileMetaInfo and is Explicit
      DataSetTS = TransferSyntax::ExplicitVRLittleEndian;
      }
    else
      {
      // File start with a 0x0008 element but no FileMetaInfo and is Implicit
      DataSetTS = TransferSyntax::ImplicitVRLittleEndian;
      }
    is.seekg(-6, std::ios::cur); // Seek back
    }
  else if( t.GetGroup() == 0x0800 ) // Good ol' ACR NEMA
    {
    is.seekg(-4, std::ios::cur); // Seek back
    DataSetTS = TransferSyntax::ImplicitVRBigEndianACRNEMA;
    }
  else if( t.GetElement() == 0x0010 ) // Hum, is it a private creator ?
    {
    is.seekg(-4, std::ios::cur); // Seek back
    DataSetTS = TransferSyntax::ImplicitVRLittleEndian;
    }
  else
    {
    //assert( t.GetElement() == 0x0 );
    char vr_str[3];
    VR::VRType vr = VR::VR_END;
    if( is.read(vr_str, 2) )
      {
      vr_str[2] = '\0';
      vr = VR::GetVRType(vr_str);
      }
    else
      {
      throw Exception( "Impossible: cannot read 2bytes for VR" );
      }
    is.seekg(-6, std::ios::cur); // Seek back
    if( vr != VR::VR_END )
      {
      // Ok we found a VR, this is 99% likely to be our safe bet
      DataSetTS = TransferSyntax::ExplicitVRLittleEndian;
      }
    else
      {
    throw Exception( "Cannot find DICOM type. Giving up." );
    //  std::streampos start = is.tellg();
    //  ImplicitDataElement ide;
    //  ide.Read<SwapperNoOp>(is); // might throw an expection which will NOT be caught
    //  std::streampos cur = is.tellg();
    //  std::cout << "s-c" << start - cur << std::endl;
    //  is.seekg( start - cur, std::ios::cur );
    //  // ok we could read at least one implicit element
    //  DataSetTS = TransferSyntax::ImplicitVRLittleEndian;
      }
    }
  return is;
}

#define ADDVRIMPLICIT( element ) \
    case element: \
      de.SetVR( (VR::VRType)TagToType<0x0002,element>::VRType ); \
      break;

bool AddVRToDataElement(DataElement &de)
{
  switch(de.GetTag().GetElement())
    {
    ADDVRIMPLICIT(0x0000);
    ADDVRIMPLICIT(0x0001);
    ADDVRIMPLICIT(0x0002);
    ADDVRIMPLICIT(0x0003);
    ADDVRIMPLICIT(0x0010);
    ADDVRIMPLICIT(0x0012);
    ADDVRIMPLICIT(0x0013);
    ADDVRIMPLICIT(0x0016);
    ADDVRIMPLICIT(0x0100);
    ADDVRIMPLICIT(0x0102);
    default:
      return false;
    }
  return true;
}

template <typename TSwap>
std::istream &FileMetaInformation::ReadCompatInternal(std::istream &is)
{
  //assert( t.GetGroup() == 0x0002 );
//  if( t.GetGroup() == 0x0002 )
    {
    // Purposely not Re-use ReadVR since we can read VR_END
    char vr_str[2];
    is.read(vr_str, 2);
    if( VR::IsValid(vr_str) )
      {
      MetaInformationTS = TransferSyntax::Explicit;
      // Hourah !
      // Looks like an Explicit File Meta Information Header.
      is.seekg(-6, std::ios::cur); // Seek back
      //is.seekg(start, std::ios::beg); // Seek back
      std::streampos dpos = is.tellg();
      ExplicitDataElement xde;
      while( ReadExplicitDataElement<SwapperNoOp>(is, xde ) )
        {
        //std::cout << xde << std::endl;
        if( xde.GetVR() == VR::UN )
          {
          gdcmWarningMacro( "VR::UN found in file Meta header. "
            "VR::UN will be replaced with proper VR for tag: " << xde.GetTag() );
          AddVRToDataElement(xde);
          }
        Insert( xde );
        }
      // Now is a good time to find out the dataset transfer syntax
      ComputeDataSetTransferSyntax();
      }
    else
      {
      MetaInformationTS = TransferSyntax::Implicit;
      gdcmWarningMacro( "File Meta Information is implicit. VR will be explicitely added" );
      // Ok this might be an implicit encoded Meta File Information header...
      // GE_DLX-8-MONO2-PrivateSyntax.dcm
      is.seekg(-6, std::ios::cur); // Seek back
      ImplicitDataElement ide;
      while( ReadImplicitDataElement<SwapperNoOp>(is, ide ) )
        {
        if( AddVRToDataElement(ide) )
          {
          Insert(ide);
          }
        else
          {
          gdcmWarningMacro( "Unknown element found in Meta Header: " << ide.GetTag() );
          }
        }
      // Now is a good time to find out the dataset transfer syntax
      ComputeDataSetTransferSyntax();
      }
    }
//  else
//    {
//    gdcmDebugMacro( "No File Meta Information. Start with Tag: " << t );
//    is.seekg(-4, std::ios::cur); // Seek back
//    }

  // we are at the end of the meta file information and before the dataset
  return is;
}

//void FileMetaInformation::SetTransferSyntaxType(TS const &ts)
//{
//  //assert( DS == 0 );
//  //InternalTS = ts;
//}

// FIXME: If any boozoo ever write a SQ in the meta header
// we are in bad shape...
void FileMetaInformation::ComputeDataSetTransferSyntax()
{
  const Tag t(0x0002,0x0010);
  const DataElement &de = GetDataElement(t);
  //TransferSyntax::NegociatedType nt = GetNegociatedType();
  std::string ts;
//  if( const ExplicitDataElement *xde = dynamic_cast<const ExplicitDataElement*>(&de) )
    {
    const Value &v = de.GetValue();
    const ByteValue &bv = dynamic_cast<const ByteValue&>(v);
    // Pad string with a \0
    ts = std::string(bv.GetPointer(), bv.GetLength());
    }
//  else if( const ImplicitDataElement *ide = dynamic_cast<const ImplicitDataElement*>(&de) )
//    {
//    const Value &v = ide.GetValue();
//    const ByteValue &bv = dynamic_cast<const ByteValue&>(v);
//    // Pad string with a \0
//    ts = std::string(bv.GetPointer(), bv.GetLength());
//    }
//  else
//    {
//    assert( 0 && "Cannot happen" );
//    }
  gdcmDebugMacro( "TS: " << ts );
  TransferSyntax tst(TransferSyntax::GetTSType(ts.c_str()));
  if( tst == TransferSyntax::TS_END )
    {
    throw Exception( "Unknown Transfer syntax" );
    }
  DataSetTS = tst;

  // postcondition
  DataSetTS.IsValid();
}

MediaStorage FileMetaInformation::GetMediaStorage() const
{
  // D 0002|0002 [UI] [Media Storage SOP Class UID]
  // [1.2.840.10008.5.1.4.1.1.12.1]
  // ==>       [X-Ray Angiographic Image Storage]
  const Tag t(0x0002,0x0002);
  if( !FindDataElement( t ) )
    {
    gdcmDebugMacro( "File Meta information is present but does not"
      " contains " << t );
    return MediaStorage::MS_END;
    }
  const DataElement &de = GetDataElement(t);
  std::string ts;
    {
    const ByteValue *bv = de.GetByteValue();
    assert( bv );
    if( bv->GetPointer() && bv->GetLength() )
      {
      // Pad string with a \0
      ts = std::string(bv->GetPointer(), bv->GetLength());
      }
    }
  // Paranoid check: if last character of a VR=UI is space let's pretend this is a \0
  if( ts.size() )
    {
    char &last = ts[ts.size()-1];
    if( last == ' ' )
      {
      gdcmWarningMacro( "Media Storage Class UID: " << ts << " contained a trailing space character" );
      last = '\0';
      }
    }
  gdcmDebugMacro( "TS: " << ts );
  MediaStorage ms = MediaStorage::GetMSType(ts.c_str());
  if( ms == MediaStorage::MS_END )
    {
    gdcmWarningMacro( "Media Storage Class UID: " << ts << " is unknow" );
    }
  return ms;
}

void FileMetaInformation::Default()
{
}

std::ostream &FileMetaInformation::Write(std::ostream &os) const
{
//  if( !IsValid() )
//    {
//    gdcmErrorMacro( "File is not valid" );
//    return os;
//    }
  P.Write(os);

//  if( IsEmpty() )
//  {
//    std::cerr << "IsEmpty" << std::endl;
//    FileMetaInformation fmi;
//    fmi.Default();
//    //fmi.Write(os);
//    IOSerialize<SwapperNoOp>::Write(os,fmi);
//  }
//  else if( IsValid() )
  {
    this->DataSet::Write<ExplicitDataElement,SwapperNoOp>(os);
  }
//  else
//  {
//    abort();
//  }
#if 0
    // At least make sure to have group length
    //if( !DS->FindDataElement( Tag(0x0002, 0x0000) ) )
      {
      //if( DS->GetNegociatedType() == TransferSyntax::Explicit )
        {
        ExplicitDataElement xde( Tag(0x0002, 0x0000), 4, VR::UL );
        SmartPointer<ByteValue> bv = new ByteValue;
        bv->SetLength( 4 );
        uint32_t len = DS->GetLength();
        Element<VR::UL, VM::VM1> el = 
          reinterpret_cast< Element<VR::UL, VM::VM1>& > ( len );
        std::stringstream ss;
        el.Write( ss );
        bv->Read( ss );
        xde.SetValue( *bv );
        // This is the first element, so simply write the element and
        // then start writing the remaining of the File Meta Information
        xde.Write(os);
        }
      }

#endif
  return os;
}

} // end namespace gdcm

