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
/*
 * HISTORY:
 * In GDCM 1.X the prefered terms was 'ReWrite', however one author of GDCM dislike
 * the term ReWrite since it is associated with the highly associated with the Rewrite
 * notion in software programming where using reinvent the wheel and rewrite from scratch code
 * the term convert was prefered
 *
 * Tools to conv. Goals being to 'purify' a DICOM file.
 * For now it will do the minimum:
 * - If Group Length is present, the length is garantee to be correct
 * - If Element with Group Tag 0x1, 0x3, 0x5 or 0x7 are present they are
 *   simply discarded (not written).
 * - Elements are written in alphabetical order
 * - 32bits VR have the residue bytes sets to 0x0,0x0
 * - Same goes from Item Length end delimitor, sets to 0x0,0x0
 * - All buggy files (wrong length: GE, 13 and Siemens Leonardo) are fixed
 * - All size are even (no odd length from gdcm 1.x)
 *
 * // \todo:
 * // --preamble: clean preamble
 * // --meta: clean meta (meta info version...)
 * // --dicomV3 (use TS unless not supported)
 * // --recompute group-length
 * // --undefined sq
 * // --explicit sq *
 * \todo in a close future:
 * - Set appropriate VR from DICOM dict
 * - Rewrite PMS SQ into DICOM SQ
 * - Rewrite Implicit SQ with defined length as undefined length
 * - PixelData with `overlay` in unused bits should be cleanup
 * - Any broken JPEG file (wrong bits) should be fixed
 * - DicomObject bug should be fixed
 * - Meta and Dataset should have a matching UID (more generally File Meta
 *   should be correct (Explicit!) and consistant with DataSet)
 * - User should be able to specify he wants Group Length (or remove them)
 * - Media SOP should be correct (deduct from something else or set to
 *   SOP Secondary if all else fail).
 * - Padding character should be correct
 *
 * \todo distant future:
 * - Later on, it should run through a Validator
 *   which will make sure all field 1, 1C are present and those only
 * - In a perfect world I should remove private tags and transform them into
 *   public fields.
 * - DA should be correct, PN should be correct (no space!)
 * - Enumerated Value should be correct
 */
#include "gdcmReader.h"
#include "gdcmImageReader.h"
#include "gdcmImageWriter.h"
#include "gdcmWriter.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
#include "gdcmSequenceOfItems.h"
#include "gdcmUIDGenerator.h"

#include <string>
#include <iostream>

#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>
#include <string.h>

struct SetSQToUndefined
{
  void operator() (gdcm::DataElement &de) {
    de.SetVLToUndefined();
  }
};

int main (int argc, char *argv[])
{
  int c;
  //int digit_optind = 0;

  std::string filename;
  std::string outfilename;
  std::string root;
  int raw = 0;
  int rootuid = 0;
  while (1) {
    //int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
        {"input", 1, 0, 0},
        {"output", 1, 0, 0},
        {"group-length", 1, 0, 0}, // valid / create / remove
        {"preamble", 1, 0, 0}, // valid / create / remove
        {"padding", 1, 0, 0}, // valid (\0 -> space) / optimize (at most 1 byte of padding)
        {"vr", 1, 0, 0}, // valid
        {"sop", 1, 0, 0}, // default to SC...
        {"iod", 1, 0, 0}, // valid
        {"meta", 1, 0, 0}, // valid / create / remove
        {"dataset", 1, 0, 0}, // valid / create / remove?
        {"sequence", 1, 0, 0}, // defined / undefined 
        {"deflate", 1, 0, 0}, // 1 - 9 / best = 9 / fast = 1
        {"tag", 1, 0, 0}, // need to specify a tag xxxx,yyyy = value to override default
        {"name", 1, 0, 0}, // same as tag but explicit use of name
        {"root-uid", 1, &rootuid, 1}, // specific Root (not GDCM)
// Image specific options:
        {"pixeldata", 1, 0, 0}, // valid
        {"raw", 0, &raw, 1}, // default (implicit VR, LE) / Explicit LE / Explicit BE
        {"jpeg", 1, 0, 0}, // JPEG lossy
        {"jpegll", 1, 0, 0}, // JPEG lossless
        {"jpegls", 1, 0, 0}, // JPEG-LS: lossy / lossless
        {"j2k", 1, 0, 0}, // J2K: lossy / lossless
        {"rle", 1, 0, 0}, // lossless !
        {"mpeg2", 1, 0, 0}, // lossy !
        {"jpip", 1, 0, 0}, // ??
        {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "i:o:",
      long_options, &option_index);
    if (c == -1)
      {
      break;
      }

    switch (c)
      {
    case 0:
        {
        const char *s = long_options[option_index].name;
        printf ("option %s", s);
        if (optarg)
          {
          if( option_index == 0 ) /* input */
            {
            assert( strcmp(s, "input") == 0 );
            assert( filename.empty() );
            filename = optarg;
            }
          else if( option_index == 14 ) /* root-uid */
            {
            assert( strcmp(s, "root-uid") == 0 );
            assert( root.empty() );
            root = optarg;
            }
          printf (" with arg %s, index = %d", optarg, option_index);
          }
        printf ("\n");
        }
      break;

    case 'i':
      printf ("option i with value '%s'\n", optarg);
      assert( filename.empty() );
      filename = optarg;
      break;

    case 'o':
      printf ("option o with value '%s'\n", optarg);
      assert( outfilename.empty() );
      outfilename = optarg;
      break;

    case '?':
      break;

    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
      }
  }

  // For now only support one input / one output
  if (optind < argc)
    {
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
      {
      printf ("%s ", argv[optind++]);
      }
    printf ("\n");
    return 1;
    }

  if( filename.empty() )
    {
    std::cerr << "Need input file (-i)\n";
    return 1;
    }
  if( outfilename.empty() )
    {
    std::cerr << "Need output file (-o)\n";
    return 1;
    }
  
  gdcm::FileMetaInformation::SetSourceApplicationEntityTitle( "gdcmconv" );
  if( rootuid )
    {
    if( !gdcm::UIDGenerator::IsValid( root.c_str() ) )
      {
      std::cerr << "specified Root UID is not valid: " << root << std::endl;
      return 1;
      }
    gdcm::UIDGenerator::SetRoot( root.c_str() );
    }
  if( raw )
    {
    gdcm::ImageReader reader;
    reader.SetFileName( filename.c_str() );
    if( !reader.Read() )
      {
      std::cerr << "could not read: " << filename << std::endl;
      return 1;
      }

    const gdcm::Image &ir = reader.GetImage();

    gdcm::ImageValue image( ir );
    const gdcm::TransferSyntax &ts = ir.GetTransferSyntax();
    if( ts.IsExplicit() )
      {
      image.SetTransferSyntax( gdcm::TransferSyntax::ExplicitVRLittleEndian );
      }
    else
      {
      assert( ts.IsImplicit() );
      image.SetTransferSyntax( gdcm::TransferSyntax::ImplicitVRLittleEndian );
      }

/*
    image.SetNumberOfDimensions( ir.GetNumberOfDimensions() );

    const unsigned int *dims = ir.GetDimensions();
    image.SetDimension(0, dims[0] );
    image.SetDimension(1, dims[1] );

    const gdcm::PixelFormat &pixeltype = ir.GetPixelFormat();
    image.SetPixelFormat( pixeltype );

    const gdcm::PhotometricInterpretation &pi = ir.GetPhotometricInterpretation();
    image.SetPhotometricInterpretation( pi );
*/

    unsigned long len = ir.GetBufferLength();
    //assert( len = ir.GetBufferLength() );
    std::vector<char> buffer;
    buffer.resize(len); // black image

    ir.GetBuffer( &buffer[0] );
    gdcm::ByteValue *bv = new gdcm::ByteValue(buffer);
    gdcm::DataElement pixeldata( gdcm::Tag(0x7fe0,0x0010) );
    pixeldata.SetValue( *bv );
    image.SetDataElement( pixeldata );

    gdcm::ImageWriter writer;
    writer.SetFile( reader.GetFile() );
    writer.SetImage( image );
    writer.SetFileName( outfilename.c_str() );

    gdcm::File& file = writer.GetFile();
    gdcm::DataSet& ds = file.GetDataSet();

    if( !writer.Write() )
      {
      std::cerr << "could not write: " << outfilename << std::endl;
      return 1;
      }
    }
  else
    {
    gdcm::Reader reader;
    reader.SetFileName( filename.c_str() );
    if( !reader.Read() )
      {
      std::cerr << "Failed to read: " << filename << std::endl;
      return 1;
      }

#if 0
    // if preamble create:
    gdcm::File f(reader.GetFile());
    gdcm::Preamble p;
    p.Create();
    f.SetPreamble(p);
    gdcm::DataSet ds = reader.GetFile().GetDataSet();
    SetSQToUndefined undef;
    ds.ExecuteOperation(undef);

    gdcm::File f(reader.GetFile());
    f.SetDataSet(ds);
#endif

    gdcm::DataSet& ds = reader.GetFile().GetDataSet();
#if 0
    gdcm::DataElement de = ds.GetDataElement( gdcm::Tag(0x0010,0x0010) );
    const char patname[] = "John^Doe";
    de.SetByteValue(patname, strlen(patname));
    std::cout << de << std::endl;

    ds.Replace( de );
    std::cout << ds.GetDataElement( gdcm::Tag(0x0010,0x0010) ) << std::endl;
#endif

    /*
    //(0020,0032) DS [-158.135803\-179.035797\-75.699997]     #  34, 3 ImagePositionPatient
    //(0020,0037) DS [1.000000\0.000000\0.000000\0.000000\1.000000\0.000000] #  54, 6 ImageOrientationPatient
    gdcm::Attribute<0x0020,0x0032> at = { -158.135803, -179.035797, -75.699997 };
    gdcm::DataElement ipp = at.GetAsDataElement();
    ds.Remove( at.GetTag() );
    ds.Remove( ipp.GetTag() );
    ds.Replace( ipp );
     */

    gdcm::Writer writer;
    writer.SetFileName( outfilename.c_str() );
    writer.SetCheckFileMetaInformation( false );
    //writer.SetFile( f );
    writer.SetFile( reader.GetFile() );
    if( !writer.Write() )
      {
      std::cerr << "Failed to write: " << outfilename << std::endl;
      return 1;
      }
    }

  return 0;
}
