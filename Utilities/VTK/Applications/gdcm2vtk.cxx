/*=========================================================================

  Program: GDCM (Grassroots DICOM). A DICOM library
  Module:  $URL$

  Copyright (c) 2006-2010 Mathieu Malaterre
  All rights reserved.
  See Copyright.txt or http://gdcm.sourceforge.net/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*
 * TODO: This app should be a suclass of gdcmimg application to avoid code duplication
 */
#include "vtkGDCMImageReader.h"
#include "vtkGDCMImageWriter.h"

#include "vtkVersion.h"
#include "vtkImageReader2Factory.h"
#include "vtkImageReader2.h"
#include "vtkImageData.h"
#include "vtkMetaImageReader.h"
#include "vtkMetaImageWriter.h"
#include "vtkTIFFWriter.h"
#include "vtkPNGWriter.h"
#if VTK_MAJOR_VERSION >= 5 && VTK_MINOR_VERSION > 0
#include "vtkMINCImageReader.h"
#include "vtkMINCImageAttributes.h"
#include "vtk_tiff.h" // ORIENTATION_BOTLEFT
#include "vtkImageRGBToYBR.h"
#endif
#include "vtkMedicalImageProperties.h"
#include "vtkTIFFReader.h"
#include "vtkGESignaReader.h"
#include "vtkImageExtractComponents.h"
#include "vtkJPEGReader.h"
#include "vtkBMPReader.h"
#include "vtkLookupTable.h"
#include "vtkPointData.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkStructuredPoints.h"
#include "vtkXMLImageDataWriter.h"

#include "gdcmFilename.h"
#include "gdcmTrace.h"
#include "gdcmVersion.h"
#include "gdcmImageHelper.h"
#include "gdcmFileMetaInformation.h"
#include "gdcmSystem.h"
#include "gdcmUIDGenerator.h"

#include <getopt.h>

void PrintVersion()
{
  std::cout << "gdcm2vtk: gdcm " << gdcm::Version::GetVersion() << " ";
  const char date[] = "$Date$";
  std::cout << date << std::endl;
  //std::cout << "             VTK " << vtkVersion::GetVTKVersion() << std::endl;
  std::cout << "          " << vtkVersion::GetVTKSourceVersion() << std::endl;
}

void PrintHelp()
{
  PrintVersion();
  std::cout << "Usage: gdcm2vtk [OPTION] input output" << std::endl;
  std::cout << "Convert a vtk-supported file into DICOM.\n";
  std::cout << "Options:" << std::endl;
  std::cout << "     --force-rescale    force rescale." << std::endl;
  std::cout << "     --force-spacing    force spacing." << std::endl;
  std::cout << "     --palette-color    when supported generate a PALETTE COLOR file." << std::endl;
  std::cout << "     --argb             when supported generate a ARGB file." << std::endl;
  std::cout << "     --modality         set Modality." << std::endl;
  std::cout << "  -T --study-uid        Study UID." << std::endl;
  std::cout << "  -S --series-uid       Series UID." << std::endl;
  std::cout << "     --root-uid         Root UID." << std::endl;
  std::cout << "Compression Types (lossless):" << std::endl;
  std::cout << "  -J --jpeg                           Compress image in jpeg." << std::endl;
  std::cout << "  -K --j2k                            Compress image in j2k." << std::endl;
  std::cout << "  -L --jpegls                         Compress image in jpeg-ls." << std::endl;
  std::cout << "  -R --rle                            Compress image in rle (lossless only)." << std::endl;
  std::cout << "General Options:" << std::endl;
  std::cout << "  -V --verbose    more verbose (warning+error)." << std::endl;
  std::cout << "  -W --warning    print warning info." << std::endl;
  std::cout << "  -D --debug      print debug info." << std::endl;
  std::cout << "  -E --error      print error info." << std::endl;
  std::cout << "  -h --help       print help." << std::endl;
  std::cout << "  -v --version    print version." << std::endl;
  std::cout << "Env var:" << std::endl;
  std::cout << "  GDCM_ROOT_UID Root UID" << std::endl;
}

int main(int argc, char *argv[])
{
  int c;
  //int digit_optind = 0;

  std::string root_uid;
  int rootuid = 0;
  std::vector<std::string> filenames;
  int forcerescale = 0;
  int forcespacing = 0;
  int palettecolor = 0;
  int argb = 0;
  int modality = 0;
  std::string modality_str;
  int studyuid = 0;
  int seriesuid = 0;
  // compression
  int jpeg = 0;
  int jpegls = 0;
  int j2k = 0;
  int rle = 0;

  int verbose = 0;
  int warning = 0;
  int debug = 0;
  int error = 0;
  int help = 0;
  int version = 0;

  gdcm::UIDGenerator uid;
  std::string series_uid = uid.Generate();
  std::string study_uid = uid.Generate();
  while (1) {
    //int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
        {"force-rescale", 0, &forcerescale, 1},
        {"force-spacing", 0, &forcespacing, 1},
        {"palette-color", 0, &palettecolor, 1},
        {"argb", 0, &argb, 1},
        {"modality", 1, &modality, 1},
        {"study-uid", 1, &studyuid, 1},
        {"series-uid", 1, &seriesuid, 1},
        {"root-uid", 1, &rootuid, 1}, // specific Root (not GDCM)
        {"jpeg", 0, &jpeg, 1}, // JPEG lossy / lossless
        {"jpegls", 0, &jpegls, 1}, // JPEG-LS: lossy / lossless
        {"j2k", 0, &j2k, 1}, // J2K: lossy / lossless
        {"rle", 0, &rle, 1}, // lossless !

// General options !
        {"verbose", 0, &verbose, 1},
        {"warning", 0, &warning, 1},
        {"debug", 0, &debug, 1},
        {"error", 0, &error, 1},
        {"help", 0, &help, 1},
        {"version", 0, &version, 1},

        {0, 0, 0, 0}
    };

    c = getopt_long (argc, argv, "JKLRT:S:VWDEhv",
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
        //printf ("option %s", s);
        if (optarg)
          {
          if( option_index == 0 ) /* input */
            {
            assert( strcmp(s, "input") == 0 );
            //assert( filename.empty() );
            //filename = optarg;
            }
          else if( option_index == 4 ) /* modality */
            {
            assert( strcmp(s, "modality") == 0 );
            //assert( filename.empty() );
            modality_str = optarg;
            }
          else if( option_index == 5 ) /* study-uid */
            {
            assert( strcmp(s, "study-uid") == 0 );
            series_uid = optarg;
            }
          else if( option_index == 6 ) /* series-uid */
            {
            assert( strcmp(s, "series-uid") == 0 );
            study_uid = optarg;
            }
          else if( option_index == 7 ) /* root-uid */
            {
            assert( strcmp(s, "root-uid") == 0 );
            root_uid = optarg;
            }
          //printf (" with arg %s", optarg);
          }
        //printf ("\n");
        }
      break;

    case 'J':
      jpeg = 1;
      break;

    case 'K':
      j2k = 1;
      break;

    case 'L':
      jpegls = 1;
      break;

    case 'R':
      rle = 1;
      break;

    case 'T':
      studyuid = 1;
      study_uid = optarg;
      break;

    case 'S':
      seriesuid = 1;
      series_uid = optarg;
      break;

    case 'V':
      verbose = 1;
      break;

    case 'W':
      warning = 1;
      break;

    case 'D':
      debug = 1;
      break;

    case 'E':
      error = 1;
      break;

    case 'h':
      help = 1;
      break;

    case 'v':
      version = 1;
      break;

    case '?':
      break;

    default:
      printf ("?? getopt returned character code 0%o ??\n", c);
      }
  }

  if (optind < argc)
    {
    //printf ("non-option ARGV-elements: ");
    while (optind < argc)
      {
      //printf ("%s ", argv[optind]);
      filenames.push_back( argv[optind++] );
      }
    //printf ("\n");
    }

  if( version )
    {
    //std::cout << "version" << std::endl;
    PrintVersion();
    return 0;
    }

  if( help )
    {
    //std::cout << "help" << std::endl;
    PrintHelp();
    return 0;
    }

  // Debug is a little too verbose
  gdcm::Trace::SetDebug( debug );
  gdcm::Trace::SetWarning( warning );
  gdcm::Trace::SetError( error );
  // when verbose is true, make sure warning+error are turned on:
  if( verbose )
    {
    gdcm::Trace::SetWarning( verbose );
    gdcm::Trace::SetError( verbose);
    }

  if( filenames.size() != 2 )
    {
    PrintHelp();
    return 1;
    }
  const char *filename = filenames[0].c_str();
  const char *outfilename = filenames[1].c_str();

  gdcm::ImageHelper::SetForceRescaleInterceptSlope(forcerescale);
  gdcm::ImageHelper::SetForcePixelSpacing(forcespacing);

  vtkGDCMImageReader *reader = vtkGDCMImageReader::New();

  vtkImageReader2Factory* imgfactory = vtkImageReader2Factory::New();
  imgfactory->RegisterReader( reader );
  vtkImageReader2* imgreader =
    imgfactory->CreateImageReader2(filename);
  vtkStructuredPointsReader *datareader = vtkStructuredPointsReader::New();
  datareader->SetFileName( filename );
  int res = 0;
  if( !imgreader )
    {
    res = datareader->IsFileStructuredPoints();
    if( !res )
      {
      std::cerr << "could not find no reader to handle file: " << filename << std::endl;
      return 1;
      }
    }
  imgfactory->Delete(); 

  vtkImageData *imgdata;
  if( imgreader )
    {
    imgreader->SetFileName(filename);
    imgreader->Update();
    imgdata = imgreader->GetOutput();
    if( verbose )
      std::cout << imgreader->GetClassName() << std::endl;
    }
  else if( res )
    {
    datareader->Update();
    imgdata = datareader->GetOutput();
    }

  gdcm::Filename fn = outfilename;
  const char *outputextension = fn.GetExtension();

  if ( outputextension )
    {
    if(  gdcm::System::StrCaseCmp(outputextension,".vtk") == 0  )
      {
      vtkStructuredPointsWriter * writer = vtkStructuredPointsWriter::New();
      writer->SetFileName( outfilename );
      writer->SetFileTypeToBinary();
      writer->SetInput( imgdata );
      writer->Write();
      return 0;
      }
    else if(  gdcm::System::StrCaseCmp(outputextension,".png") == 0 )
      {
      vtkPNGWriter * writer = vtkPNGWriter::New();
      writer->SetFileName( outfilename );
      writer->SetInput( imgdata );
      writer->Write();
      return 0;
      }
    else if(  gdcm::System::StrCaseCmp(outputextension,".tif") == 0  
      ||  gdcm::System::StrCaseCmp(outputextension,".tiff") == 0  ) // 
      {
      vtkTIFFWriter * writer = vtkTIFFWriter::New();
      writer->SetFileName( outfilename );
      writer->SetInput( imgdata );
      writer->Write();
      return 0;
      }
    else if(  gdcm::System::StrCaseCmp(outputextension,".vti") == 0  ) // vtkXMLImageDataWriter::GetDefaultFileExtension()
      {
      vtkXMLImageDataWriter * writer = vtkXMLImageDataWriter::New();
      writer->SetFileName( outfilename );
      writer->SetDataModeToBinary();
      writer->SetInput( imgdata );
      writer->Write();
      return 0;
      }
    else if(  gdcm::System::StrCaseCmp(outputextension,".mha") == 0 ||
              gdcm::System::StrCaseCmp(outputextension,".mhd") == 0  ) // vtkMetaImageReader::GetFileExtensions()
      {
      // Weird, the writer does not offer the same API as the Reader, for instance
      // One cannot set the patient name to store (see vtkMetaImageReader::GetPatientName ...)
      vtkMetaImageWriter * writer = vtkMetaImageWriter::New();
      writer->SetFileName( outfilename );
      writer->SetInput( imgdata );
      writer->Write();
      return 0;
      }
    }
  // else

  vtkGDCMImageWriter * writer = vtkGDCMImageWriter::New();
  if( studyuid )
    {
    writer->SetStudyUID( study_uid.c_str() );
    }
  if( seriesuid )
    {
    writer->SetSeriesUID( series_uid.c_str() );
    }

  // Default:
  writer->SetCompressionType( vtkGDCMImageWriter::NO_COMPRESSION ); // Implicit VR Little Endian
  if( jpeg )
    {
    writer->SetCompressionType( vtkGDCMImageWriter::JPEG_COMPRESSION );
    }
  else if( j2k )
    {
    writer->SetCompressionType( vtkGDCMImageWriter::JPEG2000_COMPRESSION );
    }
  else if( jpegls )
    {
    writer->SetCompressionType( vtkGDCMImageWriter::JPEGLS_COMPRESSION );
    }
  else if( rle )
    {
    writer->SetCompressionType( vtkGDCMImageWriter::RLE_COMPRESSION );
    }

  // HACK: call it *after* instanciating vtkGDCMImageWriter
  if( !gdcm::UIDGenerator::IsValid( study_uid.c_str() ) )
    {
    std::cerr << "Invalid UID for Study UID: " << study_uid << std::endl;
    return 1;
    }
  if( !gdcm::UIDGenerator::IsValid( series_uid.c_str() ) )
    {
    std::cerr << "Invalid UID for Series UID: " << series_uid << std::endl;
    return 1;
    }
  gdcm::FileMetaInformation::SetSourceApplicationEntityTitle( "gdcm2vtk" );
  if( !rootuid )
    {
    // only read the env var is no explicit cmd line option
    // maybe there is an env var defined... let's check
    const char *rootuid_env = getenv("GDCM_ROOT_UID");
    if( rootuid_env )
      {
      rootuid = 1;
      root_uid = rootuid_env;
      }
    }
  if( rootuid )
    {
    if( !gdcm::UIDGenerator::IsValid( root_uid.c_str() ) )
      {
      std::cerr << "specified Root UID is not valid: " << root_uid << std::endl;
      return 1;
      }
    gdcm::UIDGenerator::SetRoot( root_uid.c_str() );
    }

  writer->SetFileName( outfilename );

  if( imgreader && imgreader->GetOutput()->GetNumberOfScalarComponents() == 4 && !argb )
    {
    if( verbose )
      std::cout << "alpha channel will be lost" << std::endl;
    vtkImageExtractComponents *extract = vtkImageExtractComponents::New();
    extract->SetInput( imgreader->GetOutput() );
    extract->SetComponents( 0,1,2 );
    writer->SetInput( extract->GetOutput() );
    extract->Delete();
    }
  else
    {
    //writer->SetInput( imgreader->GetOutput() );
    writer->SetInput( imgdata );

#if 0
    vtkImageRGBToYBR * rgb2ybr = vtkImageRGBToYBR::New();
    rgb2ybr->SetInput( imgreader->GetOutput() );
    writer->SetInput( rgb2ybr->GetOutput() );
    writer->SetImageFormat( VTK_YBR );
#endif
    }

  // If input is 3D, let's write output as 3D if possible:
  if( imgdata->GetDimensions()[2] != 1 )
    {
    writer->SetFileDimensionality( 3 );
    }

  if( imgreader )
    {
    if( vtkGDCMImageReader * reader = vtkGDCMImageReader::SafeDownCast(imgreader) )
      {
      writer->SetMedicalImageProperties( reader->GetMedicalImageProperties() );
      writer->SetDirectionCosines( reader->GetDirectionCosines() );
      writer->SetShift( reader->GetShift() );
      writer->SetScale( reader->GetScale() );
      writer->SetImageFormat( reader->GetImageFormat() );
      writer->SetLossyFlag( reader->GetLossyFlag() );
      if( verbose )
        {
        reader->GetOutput()->Print( std::cout );
        reader->GetMedicalImageProperties()->Print( std::cout );
        }
      }
    else if( vtkJPEGReader * reader = vtkJPEGReader::SafeDownCast(imgreader) )
      {
      // vtk JPEG reader only read 8bits lossy file
      writer->SetLossyFlag( 1 );
      // TODO: It would be nice to specify the original encoder was JPEG -> ISO_10918_1
      }
    else if( vtkBMPReader * reader = vtkBMPReader::SafeDownCast(imgreader) )
      {
      if( palettecolor )
        reader->Allow8BitBMPOn( );
      reader->Update( );
      //reader->GetLookupTable()->Print( std::cout );
      if( palettecolor )
        {
        reader->GetOutput()->GetPointData()->GetScalars()->SetLookupTable( reader->GetLookupTable() );
        writer->SetImageFormat( VTK_LOOKUP_TABLE );
        }
      }
    else if( vtkGESignaReader * reader = vtkGESignaReader::SafeDownCast(imgreader) )
      {
#if VTK_MAJOR_VERSION >= 5 && VTK_MINOR_VERSION > 0
      writer->SetMedicalImageProperties( reader->GetMedicalImageProperties() );
#endif
      //reader->GetMedicalImageProperties()->Print( std::cout );
      }
#if VTK_MAJOR_VERSION >= 5 && VTK_MINOR_VERSION > 0
    else if( vtkMINCImageReader *reader = vtkMINCImageReader::SafeDownCast( imgreader ) )
      {
      writer->SetDirectionCosines( reader->GetDirectionCosines() );
      //writer->GetMedicalImageProperties()->SetModality( "MR" );
      // the following does not work with VTKData/Data/t3_grid_0.mnc
      //writer->SetScale( reader->GetRescaleSlope() );
      //writer->SetShift( reader->GetRescaleIntercept() );
      if( verbose )
        reader->GetImageAttributes()->PrintFileHeader();
      }
#endif
    else if( vtkTIFFReader *reader = vtkTIFFReader::SafeDownCast( imgreader ) )
      {
      // TIFF has resolution (spacing), and VTK make sure to set set in mm
      // For some reason vtkTIFFReader is all skrew up and will load the image in whatever orientation
      // as stored on file, thus this is up to the user to set it properly...
      // If anyone has any clue why...
#if VTK_MAJOR_VERSION >= 5 && VTK_MINOR_VERSION > 0
      reader->SetOrientationType( ORIENTATION_BOTLEFT );
#endif
      }
    else if( vtkMetaImageReader *reader = vtkMetaImageReader::SafeDownCast( imgreader ) )
      {
//  vtkGetMacro(RescaleSlope, double);
//  vtkGetMacro(RescaleOffset, double);
      writer->SetScale( reader->GetRescaleSlope() );
      writer->SetShift( reader->GetRescaleOffset() );
//  vtkGetStringMacro(Modality);
      writer->GetMedicalImageProperties()->SetModality( reader->GetModality() );

//  vtkGetStringMacro(DistanceUnits);
  // -> this one is insane, the default behavior is 'um' . What in the world is 'um' unit ?
  // I think I should discard any non 'mm' distance unit, simple as that

// The following looks nice from the API, but the VTK implementation only sets them to '?' ... weird
//  vtkGetMacro(BitsAllocated, int);
//  vtkGetStringMacro(AnatomicalOrientation);
//  vtkGetMacro(GantryAngle, double); 
//  vtkGetStringMacro(PatientName);
//  vtkGetStringMacro(PatientID);
//  vtkGetStringMacro(Date);
//  vtkGetStringMacro(Series);
//  vtkGetStringMacro(ImageNumber);
//  vtkGetStringMacro(StudyID);
//  vtkGetStringMacro(StudyUID);
//  vtkGetStringMacro(TransferSyntaxUID);
      }
    }
  // nothing special need to be done for vtkStructuredPointsReader 

  // Handle here the specific modality:
  if ( modality )
    {
    if( !modality_str.empty() )
      writer->GetMedicalImageProperties()->SetModality( modality_str.c_str() );;
    }

  // Pass on the filetime of input file
  time_t studydatetime = gdcm::System::FileTime( filename );
  char date[22];
  gdcm::System::FormatDateTime(date, studydatetime);
  // ContentDate
  const size_t datelen = 8;
    {
    // Do not copy the whole cstring:
    std::string s( date, date+datelen );
    writer->GetMedicalImageProperties()->SetImageDate( s.c_str() );;
    }
  // ContentTime
  const size_t timelen = 6; // get rid of milliseconds
    {
    // Do not copy the whole cstring:
    std::string s( date+datelen, timelen );
    writer->GetMedicalImageProperties()->SetImageTime( s.c_str() );;
    }

  writer->Write();

  if( verbose )
    writer->GetInput()->Print( std::cout );

  writer->Delete();
  if( imgreader ) imgreader->Delete();
  datareader->Delete();
  reader->Delete();

  return 0;
}

