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
#ifndef __gdcmDICOMDIRGenerator_h
#define __gdcmDICOMDIRGenerator_h

#include "gdcmDirectory.h"
#include "gdcmTag.h"
#include <utility> // std::pair

namespace gdcm
{
class File;
class Scanner;
class SequenceOfItems;
class VL;
class DICOMDIRGeneratorInternal;

/**
 * \brief DICOMDIRGenerator class
 * This is a STD-GEN-CD DICOMDIR generator.
 * ref: PS 3.11-2008 Annex D (Normative) - General Purpose CD-R and DVD Interchange Profiles
 *
 * \note
 * PS 3.11 - 2008 / D.3.2 Physical Medium And Medium Format
 * The STD-GEN-CD and STD-GEN-SEC-CD application profiles require the 120 mm CD-R physical
 * medium with the ISO/IEC 9660 Media Format, as defined in PS3.12.
 * See also PS 3.12 - 2008 / Annex F 120mm CD-R Medium (Normative) and 
 * PS 3.10 - 2008 / 8 DICOM File Service / 8.1 FILE-SET
 *
 * \warning:
 * PS 3.11 - 2008 / D.3.1 SOP Classes and Transfer Syntaxes
 * Composite Image & Stand-alone Storage are required to be stored as Explicit VR Little 
 * Endian Uncompressed (1.2.840.10008.1.2.1). When a DICOM file is found using another
 * Transfer Syntax the generator will simply stops.
 *
 * \warning
 * - Input files should be Explicit VR Little Endian
 * - filenames should be valid VR::CS value (16 bytes, upper case)
 *
 */
class GDCM_EXPORT DICOMDIRGenerator
{
public:
  typedef Directory::FilenamesType  FilenamesType;
  DICOMDIRGenerator();
  ~DICOMDIRGenerator();

  /// Set the list of filenames from which the DICOMDIR should be generated from
  void SetFilenames( FilenamesType const & fns );

  /// Set the File Set ID. 
  /// \warning this need to be a valid VR::CS value
  void SetDescriptor( const char *d );

  /// Main function to generate the DICOMDIR
  bool Generate();

  /// Set/Get file. The DICOMDIR file will be valid once a call to Generate has been done
  void SetFile(const File& f);
  File &GetFile();

protected:
  Scanner &GetScanner();
  bool AddPatientDirectoryRecord();
  bool AddStudyDirectoryRecord();
  bool AddSeriesDirectoryRecord();
  bool AddImageDirectoryRecord();

private:
  bool TraverseDirectoryRecords(VL start );
  bool ComputeDirectoryRecordsOffset(const SequenceOfItems *sqi, VL start);
  unsigned int FindNextDirectoryRecord( unsigned int item1, const char *directorytype );
  SequenceOfItems *GetDirectoryRecordSequence();
  unsigned int FindLowerLevelDirectoryRecord( unsigned int item1, const char *directorytype );
  typedef std::pair< std::string, Tag> MyPair;
  MyPair GetReferenceValueForDirectoryType(unsigned int item);
  bool SeriesBelongToStudy(const char *seriesuid, const char *studyuid);
  bool ImageBelongToSeries(const char *sopuid, const char *seriesuid, Tag const &t1, Tag const &t2);
  bool ImageBelongToSameSeries(const char *sopuid, const char *seriesuid, Tag const &t);

  DICOMDIRGeneratorInternal * Internals;
};

} // end namespace gdcm

#endif //__gdcmDICOMDIRGenerator_h
