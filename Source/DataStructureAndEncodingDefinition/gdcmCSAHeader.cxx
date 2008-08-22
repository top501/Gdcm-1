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
#include "gdcmCSAHeader.h"
#include "gdcmPrivateTag.h"
#include "gdcmDataElement.h"
#include "gdcmCSAElement.h"
#include "gdcmDataSet.h"
#include "gdcmExplicitDataElement.h"
#include "gdcmSwapper.h"

namespace gdcm
{
/*
 *  (0029,1110) OB 53\54\45\50\3b\0a... # 5342,1 MedCom OOG Info
 *  -> MEDCOM Object Oriented Graphics (OOG) data.
 *  I think this is the STEP-File, aka ISO standard (10303)
 *  http://sourceforge.net/projects/stepmod
 *
 *  $ ./bin/gdcmraw -i MR-SIEMENS-DICOM-WithOverlays.dcm -t 0029,1110 -o 0029_1110.raw
 *  $ gvim 0029_1110.raw
STEP;
HEADER;
    FILE_IDENTIFICATION(
	'',
	'Wed Nov 30 15:11:24 2005',
	('meduser'),
	(''),
	'1.0',
	'1.0',
	'Exchangeboard');
    FILE_DESCRIPTION('STEP format');
    IMP_LEVEL('1.0');
ENDSEC;
DATA;
@329 = CsaGraDoubleVec3DArray(0, ());
@331 = CsaGraDoubleVec3DArray(0, (69.0, 246.0, 0.0, 67.0, 245.0, 0.0, 66.0, 244.0, 0.0, 64.0, 244.0, 0.0, 62.0, 243.0, 0.0, 61.0, 243.0, 0.0, 59.0, 243.0, 0.0, 57.0, 243.0, 0.0, 56.0, 243.0, 0.0, 55.0, 243.0, 0.0, 53.0, 243.0, 0.0, 52.0, 243.0, 0.0, 51.0, 243.0, 0.0, 50.0, 243.0, 0.0, 49.0, 243.0, 0.0, 48.0, 243.0, 0.0, 48.0, 244.0, 0.0, 47.0, 244.0, 0.0, 46.0, 245.0, 0.0, 46.0, 247.0, 0.0, 46.0, 248.0, 0.0, 46.0, 249.0, 0.0, 46.0, 250.0, 0.0, 46.0, 251.0, 0.0, 46.0, 252.0, 0.0, 46.0, 253.0, 0.0, 46.0, 255.0, 0.0, 46.0, 257.0, 0.0, 47.0, 258.0, 0.0, 47.0, 259.0, 0.0, 48.0, 261.0, 0.0, 49.0, 261.0, 0.0, 50.0, 263.0, 0.0, 50.0, 264.0, 0.0, 51.0, 266.0, 0.0, 53.0, 267.0, 0.0, 54.0, 268.0, 0.0, 55.0, 270.0, 0.0, 56.0, 270.0, 0.0, 56.0, 271.0, 0.0, 57.0, 271.0, 0.0, 58.0, 271.0, 0.0, 59.0, 272.0, 0.0, 61.0, 272.0, 0.0, 62.0, 272.0, 0.0, 62.0, 273.0, 0.0, 63.0, 273.0, 0.0, 64.0, 273.0, 0.0, 65.0, 273.0, 0.0, 66.0, 273.0, 0.0, 67.0, 273.0, 0.0, 69.0, 273.0, 0.0, 69.0, 272.0, 0.0, 71.0, 271.0, 0.0, 72.0, 270.0, 0.0, 73.0, 269.0, 0.0, 73.0, 268.0, 0.0, 73.0, 267.0, 0.0, 74.0, 267.0, 0.0, 75.0, 266.0, 0.0, 75.0, 265.0, 0.0, 76.0, 265.0, 0.0, 77.0, 263.0, 0.0, 77.0, 261.0, 0.0, 78.0, 261.0, 0.0, 78.0, 259.0, 0.0, 78.0, 258.0, 0.0, 79.0, 257.0, 0.0, 79.0, 256.0, 0.0, 80.0, 256.0, 0.0, 80.0, 255.0, 0.0, 80.0, 254.0, 0.0, 80.0, 253.0, 0.0, 80.0, 252.0, 0.0, 80.0, 251.0, 0.0, 80.0, 250.0, 0.0, 79.0, 248.0, 0.0, 77.0, 247.0, 0.0, 77.0, 246.0, 0.0, 77.0, 245.0, 0.0, 77.0, 244.0, 0.0, 75.0, 244.0, 0.0, 75.0, 243.0, 0.0, 74.0, 242.0, 0.0, 73.0, 242.0, 0.0, 72.0, 241.0, 0.0, 71.0, 240.0, 0.0, 71.0, 239.0, 0.0, 70.0, 239.0, 0.0, 69.0, 239.0, 0.0, 68.0, 239.0, 0.0, 66.0, 239.0, 0.0, 64.0, 239.0, 0.0, 63.0, 239.0, 0.0, 64.0, 242.0, 0.0));
@333 = CsaImageOverlay(0, (), (), '', 0, 0, 0, $, (), (), (), (), (), (#332), 0, 0, (), '', '', (), '', (), 0, 0, $, 0, 0.0, 0.0, (), '', 0, 0, 'CAP_3D_MEANING', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, );
@332 = CsaGraphicPrimGroup(0, 0, 0, 0, 0.0, 0, #329, 1, 1, 0, 0, $, #333, $, $, $, (#330));
@335 = CsaUVString(0, (67, 111, 110, 116, 111, 117, 114, 77, 97, 110, 105, 112, 49, 54, 56, 49, 55, 100, 101, 48, 45, 97, 50, 48, 102, 45, 52, 50, 102, 99, 45, 97, 102, 102, 49, 45, 48, 55, 99, 49, 99, 51, 54, 48, 57, 102, 56, 102, 0));
@336 = CsaGraphicFrameApplContainer(0, (), 0);
@334 = CsaGraVVIDictionary(0, (#335), (#336), (-1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1), (-1));
@330 = CsaGraphicPolygon(0, 0, 0, 0, 0.0, 0, #331, 0, 1, 0, 0, #332, $, #334, $, $, 0, 63.0, 256.0, 0.0, 0, 0, 0, 0, 0, 65535, 1.0, 0.0, 1.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 0, 65535, 0.0, 0.0, 0.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 1, 0, '', 1, 0, 0, 0, 65535, 1.0, 1.0, 1.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 0, 65535, 0.0, 0.0, 0.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 2, $, $, 1);
@337 = CsaGraDoubleVec3DArray(0, ());
@339 = CsaGraDoubleVec3DArray(0, (0.84375, 0.84375, 0.0));
@341 = CsaImageOverlay(0, (), (), '', 0, 0, 0, $, (), (), (), (), (), (#340), 0, 0, (), '', '', (), '', (), 0, 0, $, 0, 0.0, 0.0, (), '', 0, 0, 'CAP_3D_MEANING', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, );
@343 = CsaGraDoubleVec3DArray(0, (0.90234375, 0.29296875, 0.0));
@345 = CsaUVString(0, (68, 101, 115, 99, 114, 105, 112, 116, 105, 111, 110, 77, 97, 110, 105, 112, 35, 51, 95, 48, 98, 51, 57, 97, 54, 98, 53, 51, 45, 56, 53, 97, 50, 45, 52, 54, 102, 48, 45, 56, 99, 49, 52, 45, 97, 55, 102, 48, 51, 99, 100, 48, 53, 51, 56, 99, 0));
@346 = CsaGraphicFrameApplContainer(0, (), 0);
@344 = CsaGraVVIDictionary(0, (#345), (#346), (-1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1), (-1));
@342 = CsaGraphicText(0, 0, 0, 0, 0.0, 0, #343, 4, 1, 1, 1, #340, $, #344, $, $, 0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 65535, 1.0, 1.0, 1.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 0, 65535, 0.0, 0.0, 0.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, (84, 114, 97, 0), 1, -1, 0, 100, -16, 0, 0, 0, 400, 0, 0, 0, 0, 3, 2, 1, 34, 77, 77, 105, 110, 99, 104, 111, 32, 102, 111, 114, 32, 83, 105, 101, 109, 101, 110, 115, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2);
@340 = CsaGraphicPrimGroup(0, 0, 0, 0, 0.0, 0, #337, 1, 1, 0, 0, $, #341, $, $, $, (#338, #342));
@348 = CsaUVString(0, (79, 114, 105, 101, 110, 116, 73, 110, 100, 105, 35, 51, 95, 48, 56, 54, 97, 100, 54, 52, 102, 50, 45, 50, 56, 49, 55, 45, 52, 102, 102, 50, 45, 57, 56, 52, 100, 45, 55, 49, 49, 101, 54, 55, 99, 52, 48, 53, 56, 57, 0));
@349 = CsaGraphicFrameApplContainer(0, (), 0);
@347 = CsaGraVVIDictionary(0, (#348), (#349), (-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1), (-1));
@338 = CsaGraphicCube(0, 0, 0, 0, 0.0, 0, #339, 4, 1, 0, 0, #340, $, #347, $, $, 0, 0.84375, 0.84375, 0.0, 0, 0, 0, 0, 0, 65535, 1.0, 1.0, 0.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 0, 65535, 0.0, 0.0, 0.0, 0.0, 0, 0.29899999999999999, 0.58699999999999997, 0.114, 1, 0, '', 1, 0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.05859375);
ENDSEC;
ENDSTEP;
 9
 */

/*
 * http://www.enac.northwestern.edu/~tew/archives/2003/02/25/incomplete-dicom-headers/
 * http://www.nmr.mgh.harvard.edu/~rudolph/software/vox2ras/download/vox2ras_rsolve.m
 * http://www.mail-archive.com/freesurfer@nmr.mgh.harvard.edu/msg03409.html
 *
 * Pretty good link:
 * http://www.mmrrcc.upenn.edu/CAMRIS/cfn/dicomhdr.html
 */

/*
 * SIEMENS-JPEG-CorruptFragClean.dcm (0029,xx10):
 * (0000,0005) FD (??) 337.625                              # 8,? (1)
 * (0000,0006) FD (??) 4.665                                # 8,? (1)
 * (0000,0007) SL (??) 955                                  # 4,? (1)
 * (0000,0008) SL (??) 200                                  # 4,? (1)
 * (0000,000c) FD (??) 4                                    # 8,? (1)
 * (0000,000d) SL (??) 2                                    # 4,? (1)
 * (0000,0013) SL (??) 337                                  # 4,? (1)
 * (0000,0014) SL (??) 0                                    # 4,? (1)
 * (0000,0016) SL (??) 50                                   # 4,? (1)
 * (0000,0018) SL (??) 672                                  # 4,? (1)
 * (0000,001d) LT (??) [N.E.C.K]                            # 8,? (1)
 * (0000,001e) FD (??) 160                                  # 8,? (1)
 * (0000,001f) FD (??) 0                                    # 8,? (1)
 * (0000,0020) FD (??) 3                                    # 8,? (1)
 * (0000,0021) SL (??) 1                                    # 4,? (1)
 * (0000,0022) SL (??) 11                                   # 4,? (1)
 * (0000,0025) SL (??) 8                                    # 4,? (1)
 * (0000,0027) FD (??) 60                                   # 8,? (1)
 * (0000,0028) SL (??) 292                                  # 4,? (1)
 * (0000,0029) SL (??) 0                                    # 4,? (1)
 * (0000,002c) SL (??) 200                                  # 4,? (1)
 * (0000,002d) SL (??) 0                                    # 4,? (1)
 * (0000,002e) SL (??) 0                                    # 4,? (1)
 * (0000,002f) FD (??) 1                                    # 8,? (1)
 * (ffff,ffff) CS (??) [END!      ]                         # 10,? (1)
 */

struct DataSetFormatEntry
{
  Tag t;
  VR  vr;
};

static DataSetFormatEntry DataSetFormatDict[] = {
 { Tag(0x0000,0x0004),VR::LT },
 { Tag(0x0000,0x0005),VR::FD },
 { Tag(0x0000,0x0006),VR::FD },
 { Tag(0x0000,0x0007),VR::SL },
 { Tag(0x0000,0x0008),VR::SL },
 { Tag(0x0000,0x000c),VR::FD },
 { Tag(0x0000,0x000d),VR::SL },
 { Tag(0x0000,0x000e),VR::SL },
 { Tag(0x0000,0x0012),VR::FD },
 { Tag(0x0000,0x0013),VR::SL },
 { Tag(0x0000,0x0014),VR::SL },
 { Tag(0x0000,0x0016),VR::SL },
 { Tag(0x0000,0x0018),VR::SL },
 { Tag(0x0000,0x001a),VR::SL },
 { Tag(0x0000,0x001d),VR::LT }, // Defined Terms: [A.B.D.O.M.E.N], [C.H.E.S.T], [E.X.T.R.E.M.I.T.Y], [H.E.A.D], [N.E.C.K], [P.E.L.V.I.S], [S.P.I.N.E]
 { Tag(0x0000,0x001e),VR::FD },
 { Tag(0x0000,0x001f),VR::FD },
 { Tag(0x0000,0x0020),VR::FD },
 { Tag(0x0000,0x0021),VR::SL },
 { Tag(0x0000,0x0022),VR::SL },
 { Tag(0x0000,0x0025),VR::SL },
 { Tag(0x0000,0x0026),VR::FD },
 { Tag(0x0000,0x0027),VR::FD },
 { Tag(0x0000,0x0028),VR::SL },
 { Tag(0x0000,0x0029),VR::SL },
 { Tag(0x0000,0x002b),VR::LT },
 { Tag(0x0000,0x002c),VR::SL },
 { Tag(0x0000,0x002d),VR::SL },
 { Tag(0x0000,0x002e),VR::SL },
 { Tag(0x0000,0x002f),VR::FD },
 { Tag(0x0000,0x0030),VR::LT },
 { Tag(0x0000,0x0033),VR::SL },
 { Tag(0x0000,0x0035),VR::SL },
 { Tag(0x0000,0x0036),VR::CS },
 { Tag(0x0000,0x0037),VR::SL },
 { Tag(0x0000,0x0038),VR::SL },
 { Tag(0x0000,0x0039),VR::SL },
 { Tag(0x0000,0x003a),VR::FD },
 { Tag(0x0000,0x003b),VR::SL },
 { Tag(0x0000,0x003c),VR::SL },
 { Tag(0x0000,0x003d),VR::FD },
 { Tag(0x0000,0x003e),VR::SL },
 { Tag(0x0000,0x003f),VR::SL },
 { Tag(0x0000,0x0101),VR::FD },
 { Tag(0x0000,0x0102),VR::FD },
 { Tag(0x0000,0x0103),VR::FD },
 { Tag(0x0000,0x0105),VR::IS },
 { Tag(0x0006,0x0006),VR::FD },
 { Tag(0x0006,0x0007),VR::FD },
 { Tag(0x0006,0x0008),VR::CS },
 { Tag(0x0006,0x000a),VR::LT },
 { Tag(0x0006,0x000b),VR::CS },
 { Tag(0x0006,0x000c),VR::FD },
 { Tag(0x0006,0x000e),VR::CS },
 { Tag(0x0006,0x000f),VR::SL },
 { Tag(0x0006,0x0024),VR::FD },
 { Tag(0xffff,0xffff),VR::CS }, // ENDS!
};

/*
 * http://www.healthcare.siemens.com/magnetom/phoenix-gallery/orthopedics/images/phoenix/29642762.ima
Image shadow data (0029,xx10)

0 - 'EchoLinePosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '427     '
1 - 'EchoColumnPosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '224     '
2 - 'EchoPartitionPosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '8       '
3 - 'UsedChannelMask' VM 1, VR UL, SyngoDT 9, NoOfItems 6, Data '63      '
4 - 'Actual3DImaPartNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
5 - 'ICE_Dims' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'X_1_1_1_1_1_1_1_1_1_1_1_41'
6 - 'B_value' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
7 - 'Filter1' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
8 - 'Filter2' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
9 - 'ProtocolSliceNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '6       '
10 - 'RealDwellTime' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '8600    '
11 - 'PixelFile' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
12 - 'PixelFileName' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
13 - 'SliceMeasurementDuration' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '338142.50000000'
14 - 'SequenceMask' VM 1, VR UL, SyngoDT 9, NoOfItems 6, Data '134217728'
15 - 'AcquisitionMatrixText' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data '421*448s'
16 - 'MeasuredFourierLines' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
17 - 'FlowEncodingDirection' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
18 - 'FlowVenc' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
19 - 'PhaseEncodingDirectionPositive' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '1       '
20 - 'NumberOfImagesInMosaic' VM 1, VR US, SyngoDT 10, NoOfItems 0, Data
21 - 'DiffusionGradientDirection' VM 3, VR FD, SyngoDT 4, NoOfItems 0, Data
22 - 'ImageGroup' VM 1, VR US, SyngoDT 10, NoOfItems 0, Data
23 - 'SliceNormalVector' VM 3, VR FD, SyngoDT 4, NoOfItems 0, Data
24 - 'DiffusionDirectionality' VM 1, VR CS, SyngoDT 16, NoOfItems 0, Data
25 - 'TimeAfterStart' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
26 - 'FlipAngle' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
27 - 'SequenceName' VM 1, VR SH, SyngoDT 22, NoOfItems 0, Data
28 - 'RepetitionTime' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
29 - 'EchoTime' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
30 - 'NumberOfAverages' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
31 - 'VoxelThickness' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
32 - 'VoxelPhaseFOV' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
33 - 'VoxelReadoutFOV' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
34 - 'VoxelPositionSag' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
35 - 'VoxelPositionCor' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
36 - 'VoxelPositionTra' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
37 - 'VoxelNormalSag' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
38 - 'VoxelNormalCor' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
39 - 'VoxelNormalTra' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
40 - 'VoxelInPlaneRot' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
41 - 'ImagePositionPatient' VM 3, VR DS, SyngoDT 3, NoOfItems 0, Data
42 - 'ImageOrientationPatient' VM 6, VR DS, SyngoDT 3, NoOfItems 0, Data
43 - 'PixelSpacing' VM 2, VR DS, SyngoDT 3, NoOfItems 0, Data
44 - 'SliceLocation' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
45 - 'SliceThickness' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
46 - 'SpectrumTextRegionLabel' VM 1, VR SH, SyngoDT 22, NoOfItems 0, Data
47 - 'Comp_Algorithm' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
48 - 'Comp_Blended' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
49 - 'Comp_ManualAdjusted' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
50 - 'Comp_AutoParam' VM 1, VR LT, SyngoDT 20, NoOfItems 0, Data
51 - 'Comp_AdjustedParam' VM 1, VR LT, SyngoDT 20, NoOfItems 0, Data
52 - 'Comp_JobID' VM 1, VR LT, SyngoDT 20, NoOfItems 0, Data
53 - 'FMRIStimulInfo' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
54 - 'FlowEncodingDirectionString' VM 1, VR SH, SyngoDT 22, NoOfItems 0, Data
55 - 'RepetitionTimeEffective' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
56 - 'CsiImagePositionPatient' VM 3, VR DS, SyngoDT 3, NoOfItems 0, Data
57 - 'CsiImageOrientationPatient' VM 6, VR DS, SyngoDT 3, NoOfItems 0, Data
58 - 'CsiPixelSpacing' VM 2, VR DS, SyngoDT 3, NoOfItems 0, Data
59 - 'CsiSliceLocation' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
60 - 'CsiSliceThickness' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
61 - 'OriginalSeriesNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
62 - 'OriginalImageNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
63 - 'ImaAbsTablePosition' VM 3, VR SL, SyngoDT 7, NoOfItems 6, Data '0       '\'0       '\'-1952   '
64 - 'NonPlanarImage' VM 1, VR US, SyngoDT 10, NoOfItems 6, Data '1       '
65 - 'MoCoQMeasure' VM 1, VR US, SyngoDT 10, NoOfItems 0, Data
66 - 'LQAlgorithm' VM 1, VR SH, SyngoDT 22, NoOfItems 0, Data
67 - 'SlicePosition_PCS' VM 3, VR FD, SyngoDT 4, NoOfItems 6, Data '-47.43732992'\'-135.75159147'\'19.57638496'
68 - 'RBMoCoTrans' VM 3, VR FD, SyngoDT 4, NoOfItems 0, Data
69 - 'RBMoCoRot' VM 3, VR FD, SyngoDT 4, NoOfItems 0, Data
70 - 'MultistepIndex' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
71 - 'ImaRelTablePosition' VM 3, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '\'0       '\'39      '
72 - 'ImaCoilString' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'T:BO1,2'
73 - 'RFSWDDataType' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'measured'
74 - 'GSWDDataType' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'measured'
75 - 'NormalizeManipulated' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
76 - 'ImaPATModeText' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'p2'
77 - 'B_matrix' VM 6, VR FD, SyngoDT 4, NoOfItems 0, Data
78 - 'BandwidthPerPixelPhaseEncode' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
79 - 'FMRIStimulLevel' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
Series shadow data (0029,xx20)

0 - 'UsedPatientWeight' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '72      '
1 - 'NumberOfPrescans' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
2 - 'TransmitterCalibration' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '541.65400000'
3 - 'PhaseGradientAmplitude' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
4 - 'ReadoutGradientAmplitude' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
5 - 'SelectionGradientAmplitude' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
6 - 'GradientDelayTime' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '13.00000000'\'14.00000000'\'10.00000000'
7 - 'RfWatchdogMask' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
8 - 'RfPowerErrorIndicator' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
9 - 'SarWholeBody' VM 3, VR DS, SyngoDT 3, NoOfItems 0, Data
10 - 'Sed' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '1000000.00000000'\'725.84052985'\'725.71234997'
11 - 'SequenceFileOwner' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'SIEMENS'
12 - 'Stim_mon_mode' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '2       '
13 - 'Operation_mode_flag' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '2       '
14 - 'dBdt_max' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
15 - 't_puls_max' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
16 - 'dBdt_thresh' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
17 - 'dBdt_limit' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
18 - 'SW_korr_faktor' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '1.00000000'
19 - 'Stim_max_online' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '2.14339137'\'17.54720879'\'0.45053142'
20 - 'Stim_max_ges_norm_online' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.63422704'
21 - 'Stim_lim' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '45.73709869'\'27.64929962'\'31.94370079'
22 - 'Stim_faktor' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '1.00000000'
23 - 'CoilForGradient' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'void'
24 - 'CoilForGradient2' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'AS092'
25 - 'CoilTuningReflection' VM 2, VR DS, SyngoDT 3, NoOfItems 0, Data
26 - 'CoilId' VM 0, VR IS, SyngoDT 6, NoOfItems 12, Data '255     '\'83      '\'238     '\'238     '\'238     '\'238     '\'238     '\'177     '\'238     '\'178     '\'238     '
27 - 'MiscSequenceParam' VM 38, VR IS, SyngoDT 6, NoOfItems 42, Data '0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '\'1086    '\'0       '\'0       '\'0       '\'0       '\'0       '\'0       '
28 - 'MrProtocolVersion' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
29 - 'DataFileName' VM 1, VR LO, SyngoDT 19, NoOfItems 0, Data
30 - 'RepresentativeImage' VM 1, VR UI, SyngoDT 25, NoOfItems 0, Data
31 - 'PositivePCSDirections' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data '+LPH'
32 - 'RelTablePosition' VM 3, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '\'0       '\'39      '
33 - 'ReadoutOS' VM 1, VR FD, SyngoDT 4, NoOfItems 6, Data '2.00000000'
34 - 'LongModelName' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'TrioTim'
35 - 'SliceArrayConcatenations' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '1       '
36 - 'SliceResolution' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '1.00000000'
37 - 'AbsTablePosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '-1952   '
38 - 'AutoAlignMatrix' VM 16, VR FL, SyngoDT 5, NoOfItems 0, Data
39 - 'MeasurementIndex' VM 1, VR FL, SyngoDT 5, NoOfItems 0, Data
40 - 'CoilString' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'T:BO1,2'
41 - 'PATModeText' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'p2'
42 - 'PatReinPattern' VM 1, VR ST, SyngoDT 23, NoOfItems 6, Data '1;HFS;72.00;15.00;2;0;2;-794520928'
43 - 'ProtocolChangeHistory' VM 1, VR US, SyngoDT 10, NoOfItems 6, Data '0       '
44 - 'Isocentered' VM 1, VR US, SyngoDT 10, NoOfItems 6, Data '1       '
45 - 'MrPhoenixProtocol' VM 1, VR UN, SyngoDT 0, NoOfItems 6, Data '<XProtocol>
<...>
46 - 'GradientMode' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'Fast'
47 - 'FlowCompensation' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'No'
48 - 'PostProcProtocol' VM 1, VR UT, SyngoDT 27, NoOfItems 0, Data
49 - 'RFSWDOperationMode' VM 1, VR SS, SyngoDT 8, NoOfItems 6, Data '+0      '
50 - 'RFSWDMostCriticalAspect' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'Whole Body'
51 - 'SARMostCriticalAspect' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '2.00000000'\'1.02534234'\'0.97963309'
52 - 'TablePositionOrigin' VM 3, VR SL, SyngoDT 7, NoOfItems 6, Data '0       '\'0       '\'-1991   '
53 - 'MrProtocol' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
54 - 'MrEvaProtocol' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
*
*
WARNING: I think this is context dependant so 0029,1010 and 0029,1110 are not supposed to mean the same thing, eg:
(CSA image data)

(0029,0010)siemens csa header
Image shadow data (0029,xx10)

0 - 'ImageNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
1 - 'ImageComments' VM 1, VR LT, SyngoDT 20, NoOfItems 0, Data
2 - 'ReferencedImageSequence' VM 0, VR UI, SyngoDT 25, NoOfItems 6, Data '1.2.840.10008.5.1.4.1.1.4'\'1.3.12.2.1107.5.2.30.25299.3.200803181402241188330606'\'1.2.840.10008.5.1.4.1.1.4'\'1.3.12.2.1107.5.2.30.25299.3.2008031814031374741230990'\'1.2.840.10008.5.1.4.1.1.4'\'1.3.12.2.1107.5.2.30.25299.3.200803181402241589130608'
3 - 'PatientOrientation' VM 1, VR CS, SyngoDT 16, NoOfItems 0, Data
4 - 'ScanningSequence' VM 0, VR CS, SyngoDT 16, NoOfItems 6, Data 'RM'
5 - 'SequenceName' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'tfi2d1_20'
6 - 'RepetitionTime' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '29.60000000'
7 - 'EchoTime' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '1.25000000'
8 - 'InversionTime' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
9 - 'NumberOfAverages' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '1.00000000'
10 - 'ImagingFrequency' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '63.61952800'
11 - 'ImagedNucleus' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data '1H'
12 - 'EchoNumbers' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '1       '
13 - 'MagneticFieldStrength' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '1.50000000'
14 - 'NumberOfPhaseEncodingSteps' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '100     '
15 - 'EchoTrainLength' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '1       '
16 - 'PercentSampling' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '32.00000000'
17 - 'PercentPhaseFieldOfView' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '100.00000000'
18 - 'TriggerTime' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '82.50000000'
19 - 'ReceivingCoil' VM 1, VR SH, SyngoDT 22, NoOfItems 0, Data
20 - 'TransmittingCoil' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data 'Body'
21 - 'AcquisitionMatrix' VM 4, VR US, SyngoDT 10, NoOfItems 6, Data '256     '\'0       '\'0       '\'82      '
22 - 'PhaseEncodingDirection' VM 1, VR CS, SyngoDT 16, NoOfItems 6, Data 'COL'
23 - 'FlipAngle' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '20.00000000'
24 - 'VariableFlipAngleFlag' VM 1, VR CS, SyngoDT 16, NoOfItems 6, Data 'N'
25 - 'SAR' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.21757985'
26 - 'dBdt' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
27 - 'SliceThickness' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '7.00000000'
28 - 'ImagePositionPatient' VM 3, VR DS, SyngoDT 3, NoOfItems 6, Data '117.38499069'\'-48.94067860'\'0.00000000'
29 - 'ImageOrientationPatient' VM 6, VR DS, SyngoDT 3, NoOfItems 6, Data '-1.00000000'\'0.00000000'\'0.00000000'\'0.00000000'\'1.00000000'\'0.00000000'
30 - 'SliceLocation' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '0.00000000'
31 - 'EchoLinePosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '50      '
32 - 'EchoColumnPosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '128     '
33 - 'EchoPartitionPosition' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '8       '
34 - 'Actual3DImaPartNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
35 - 'RealDwellTime' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '2300    '
36 - 'ProtocolSliceNumber' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
37 - 'DataFile' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
38 - 'DataFileName' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
39 - 'ICE_Dims' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data '1_1_2_1_4_1_1_1_1_1_1_1_259'
40 - 'PixelSpacing' VM 2, VR DS, SyngoDT 3, NoOfItems 6, Data '1.25000000'\'1.25000000'
41 - 'SourceImageSequence' VM 0, VR UI, SyngoDT 25, NoOfItems 0, Data
42 - 'PixelBandwidth' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '850.00000000'
43 - 'SliceMeasurementDuration' VM 1, VR DS, SyngoDT 3, NoOfItems 6, Data '4112.50000000'
44 - 'SequenceMask' VM 1, VR UL, SyngoDT 9, NoOfItems 6, Data '0       '
45 - 'AcquisitionMatrixText' VM 1, VR SH, SyngoDT 22, NoOfItems 6, Data '82*256s'
46 - 'MeasuredFourierLines' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
47 - 'CsiGridshiftVector' VM 3, VR DS, SyngoDT 3, NoOfItems 0, Data
48 - 'MultistepIndex' VM 1, VR IS, SyngoDT 6, NoOfItems 6, Data '0       '
49 - 'SpectroscopyAcquisitionPhaseColumns' VM 1, VR UL, SyngoDT 9, NoOfItems 0, Data
50 - 'SpectroscopyAcquisitionPhaseRows' VM 1, VR UL, SyngoDT 9, NoOfItems 0, Data
51 - 'SpectroscopyAcquisitionOut-of-planePhaseSteps' VM 1, VR UL, SyngoDT 9, NoOfItems 0, Data
52 - 'SpectroscopyAcquisitionDataColumns' VM 1, VR UL, SyngoDT 9, NoOfItems 0, Data
53 - 'DataPointRows' VM 1, VR UL, SyngoDT 9, NoOfItems 0, Data
54 - 'DataPointColumns' VM 1, VR UL, SyngoDT 9, NoOfItems 0, Data
55 - 'DataRepresentation' VM 1, VR CS, SyngoDT 16, NoOfItems 0, Data
56 - 'SignalDomainColumns' VM 1, VR CS, SyngoDT 16, NoOfItems 0, Data
57 - 'Columns' VM 1, VR US, SyngoDT 10, NoOfItems 6, Data '256     '
58 - 'Rows' VM 1, VR US, SyngoDT 10, NoOfItems 6, Data '100     '
59 - 'NumberOfFrames' VM 1, VR IS, SyngoDT 6, NoOfItems 0, Data
60 - 'MixingTime' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
61 - 'k-spaceFiltering' VM 1, VR CS, SyngoDT 16, NoOfItems 0, Data
62 - 'HammingFilterWidth' VM 1, VR DS, SyngoDT 3, NoOfItems 0, Data
63 - 'TransmitterReferenceAmplitude' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
64 - 'ResonantNucleus' VM 1, VR CS, SyngoDT 16, NoOfItems 0, Data
65 - 'VoiThickness' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
66 - 'VoiPhaseFoV' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
67 - 'VoiReadoutFoV' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
68 - 'VoiOrientation' VM 3, VR FD, SyngoDT 4, NoOfItems 0, Data
69 - 'VoiPosition' VM 3, VR FD, SyngoDT 4, NoOfItems 0, Data
70 - 'VoiInPlaneRotation' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
71 - 'RSatThickness' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
72 - 'RSatOrientationSag' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
73 - 'RSatOrientationCor' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
74 - 'RSatOrientationTra' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
75 - 'RSatPositionSag' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
76 - 'RSatPositionCor' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
77 - 'RSatPositionTra' VM 0, VR FD, SyngoDT 4, NoOfItems 0, Data
78 - 'SpacingBetweenSlices' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data
79 - 'DataSetInfo' VM 1, VR UN, SyngoDT 0, NoOfItems 0, Data
80 - 'ImaCoilString' VM 1, VR LO, SyngoDT 19, NoOfItems 6, Data 'BO1'
81 - 'BandwidthPerPixelPhaseEncode' VM 1, VR FD, SyngoDT 4, NoOfItems 0, Data



*/
struct equ
{ 
  uint32_t syngodt;
  const char vr[2+1];
};

// Looks like there is mapping in between syngodt and vr...
//  O <=> UN
//  3 <=> DS
//  4 <=> FD
//  5 <=> FL
//  6 <=> IS
//  9 <=> UL
// 10 <=> US
// 16 <=> CS
// 19 <=> LO
// 20 <=> LT
// 22 <=> SH
// 25 <=> UI
static equ mapping[] = {
  {  0 , "UN" },
  {  3 , "DS" },
  {  4 , "FD" },
  {  5 , "FL" },
  {  6 , "IS" },
  {  7 , "SL" },
  {  8 , "SS" },
  {  9 , "UL" },
  { 10 , "US" },
  { 16 , "CS" },
  { 19 , "LO" },
  { 20 , "LT" },
  { 22 , "SH" },
  { 23 , "ST" },
  { 25 , "UI" },
  { 27 , "UT" }
};

bool check_mapping(uint32_t syngodt, const char *vr)
{
  static const unsigned int max = sizeof(mapping) / sizeof(equ);
  const equ *p = mapping;
  assert( syngodt <= mapping[max-1].syngodt ); (void)max;
  while(p->syngodt < syngodt )
    {
    //std::cout << "mapping:" << p->vr << std::endl;
    ++p;
    }
  assert( p->syngodt == syngodt ); // or else need to update mapping
  const char* lvr = p->vr;
  int check = strcmp(vr, lvr) == 0;
  assert( check );
  return true;
}

bool checkallzero(std::istream &is)
{
  bool res = true;
  char c;
  while( is >> c )
    {
    if( c != 0 )
      {
      res = false;
      break;
      }
    }
  return res;
}

CSAHeader::CSAHeaderType CSAHeader::GetFormat() const
{
  return InternalType;
}

// dcmInfo.exe print like this:
// 67 - 'SlicePosition_PCS' VM 3, VR FD, SyngoDT 4, NoOfItems 6, Data '-185.77913332'\'-163.80459213'\'72.73944092'
bool CSAHeader::LoadFromDataElement(DataElement const &de)
{
  if( de.IsEmpty() ) return false;

  InternalCSADataSet.clear();
  gdcmDebugMacro( "Entering print" );
  InternalType = UNKNOWN; // reset
  gdcm::Tag t1(0x0029,0x0010);
  gdcm::Tag t2(0x0029,0x0020);
  Tag toto = de.GetTag().GetPrivateCreator();
  uint16_t v = (de.GetTag().GetElement() << 8);
  uint16_t v2 = v >> 8;
  //if( de.GetTag().GetPrivateCreator() == t1 )
  if( v2 == t1.GetElement() )
    {
    //std::cout << "Image shadow data (0029,xx10)\n\n";
    DataElementTag = t1;;
    }
  //else if( de.GetTag().GetPrivateCreator() == t2 )
  else if( v2 == t2.GetElement() )
    {
    //std::cout << "Series shadow data (0029,xx20)\n\n";
    DataElementTag = t2;;
    }
  else
    {
    //  std::cerr << "Unhandled tag: " << de.GetTag() << std::endl;
    DataElementTag = de.GetTag();
    }
  gdcmDebugMacro( "found type" );
 
  const ByteValue *bv = de.GetByteValue();
  assert( bv );
  const char *p = bv->GetPointer();
  assert( p );
  std::string s( bv->GetPointer(), bv->GetLength() );
  std::stringstream ss;
  ss.str( s );
  char dummy[4+1];
  dummy[4] = 0;
  ss.read(dummy, 4);
  //std::cout << dummy << std::endl;
  // 1. NEW FORMAT
  // 2. OLD FORMAT
  // 3. Zero out
  // 4. DATASET FORMAT (Explicit Little Endian), with group=0x0 elements:
  if( strcmp( dummy, "SV10" ) != 0 )
    {
    if( checkallzero(ss) )
      {
      //std::cout << "Zeroed out" << std::endl;
      InternalType = ZEROED_OUT;
      return true;
      }
    else if( strcmp(dummy, "\0\0\0\0" ) == 0 )
      {
      // Most often start with an element (0000,xxxx)
      // And ends with element:
      // (ffff,ffff)	CS	10	END!      
      ss.seekg( 0, std::ios::beg );
      // SIEMENS-JPEG-CorruptFragClean.dcm
      InternalType = DATASET_FORMAT;
      DataSet &ds = InternalDataSet;
      DataElement xde;
      while( xde.Read<ExplicitDataElement,SwapperNoOp>( ss ) )
        {
        ds.Insert( xde );
        }
      //std::cout << ds << std::endl;
      assert( ss.eof() );
      return true;
      }
    else
      {
      //abort();
      ss.seekg( 0, std::ios::beg );
      // No magic number for this one:
      // SIEMENS_Sonata-16-MONO2-Value_Multiplicity.dcm
      InternalType = NOMAGIC;
      }
    }
  if( strcmp( dummy, "SV10" ) == 0 )
    {
    // NEW FORMAT !
    ss.read(dummy, 4);
    assert( strcmp( dummy, "\4\3\2\1" ) == 0 );
    InternalType = SV10;
    }
  assert( InternalType != UNKNOWN );
  gdcmDebugMacro( "Found Type: " << InternalType );

  uint32_t n;
  ss.read((char*)&n, sizeof(n));
  uint32_t unused;
  ss.read((char*)&unused, sizeof(unused));
  assert( unused == 77 ); // 'M' character...

  for(uint32_t i = 0; i < n; ++i)
    {
    CSAElement csael;
    //std::cout << i;
    csael.SetKey( i );
    //std::cout << " - ";
    char name[64+1];
    name[64] = 0; // security
    ss.read(name, 64);
    csael.SetName( name );
    //std::cout << "'" << name << "' ";
    uint32_t vm;
    ss.read((char*)&vm, sizeof(vm));
    csael.SetVM( VM::GetVMTypeFromLength(vm,1) );
    //std::cout << "VM " << vm <<  ", ";
    char vr[4];
    ss.read(vr, 4);
    // In dataset without magic signature (OLD FORMAT) vr[3] is garbage...
    assert( /*vr[3] == 0 &&*/ vr[2] == 0 );
    csael.SetVR( VR::GetVRTypeFromFile(vr) );
    //std::cout << "VR " << vr << ", ";
    uint32_t syngodt;
    ss.read((char*)&syngodt, sizeof(syngodt));
    bool cm = check_mapping(syngodt, vr);
    if( !cm )
      {
      gdcmErrorMacro( "SyngoDT is not handled, please submit bug report" );
      return false;
      }
    csael.SetSyngoDT( syngodt );
    //std::cout << "SyngoDT " << syngodt << ", ";
    uint32_t nitems;
    ss.read((char*)&nitems, sizeof(nitems));
    csael.SetNoOfItems( nitems );
    //std::cout << "NoOfItems " << nitems << ", ";
    uint32_t xx;
    ss.read((char*)&xx, sizeof(xx));
    //std::cout << "xx=" << xx<< std::endl;
    assert( xx == 77 || xx == 205 );

    //std::cout << "Data ";
    std::ostringstream os;
    for( uint32_t j = 0; j < nitems; ++j)
      {
      uint32_t item_xx[4];
      ss.read((char*)&item_xx, 4*sizeof(uint32_t));
      assert( item_xx[2] == 77 || item_xx[2] == 205 );
      uint32_t len = item_xx[1]; // 2nd element
      assert( item_xx[0] == item_xx[1] && item_xx[1] == item_xx[3] );
      if( len )
        {
        char *val = new char[len+1];
        val[len] = 0; // security
        ss.read(val,len);
        // WARNING vr does not means anything AFAIK, 
        // simply print the value as if it was IS/DS or LO (ASCII)
        if( j )
          {
          //std::cout << '\\';
          os << '\\';
          }
        //std::cout << "'" << val << "'";
        os << val;

        char dummy[4];
        uint32_t dummy_len = (4 - len % 4) % 4;
        ss.read(dummy, dummy_len );

        for(uint32_t d= 0; d < dummy_len; ++d)
          {
          // dummy[d]  is zero in the NEW format
          //assert( dummy[d] == 0 );
          //for the old format there appears to be some garbage:
          if( dummy[d] )
            {
            //std::cout << "dummy=" << (int)dummy[d] << std::endl;
            }
          }
        delete[] val;     
        }
      }
    std::string str = os.str();
    if( !str.empty() )
      csael.SetByteValue( &str[0], str.size());
    //std::cout << std::endl;
    InternalCSADataSet.insert( csael );
    }
  return true;
}

void CSAHeader::Print(std::ostream &os) const
{
  std::set<CSAElement>::const_iterator it = InternalCSADataSet.begin();
  gdcm::Tag t1(0x0029,0x0010);
  gdcm::Tag t2(0x0029,0x0020);
  if( DataElementTag == t1 )
    {
    os << "Image shadow data (0029,xx10)\n\n";
    }
  else if( DataElementTag == t2 )
    {
    os << "Series shadow data (0029,xx20)\n\n";
    }
  else
    {
    std::cerr << "Unhandled tag: " << DataElementTag << std::endl;
    }

  for(; it != InternalCSADataSet.end(); ++it)
    {
    std::cout << *it << std::endl;
    }
}

const CSAElement &CSAHeader::GetCSAElementByName(const char *name)
{
  //int s =  InternalCSADataSet.size();
  std::set<CSAElement>::const_iterator it = InternalCSADataSet.begin();
  for(; it != InternalCSADataSet.end(); ++it)
    {
    const char *itname = it->GetName();
    if( strcmp(name, itname) == 0 )
      {
      return *it;
      }
    }
  return Dummy;
}

static const char csaheader[] = "SIEMENS CSA HEADER";
static const gdcm::PrivateTag t1(0x0029,0x0010,csaheader);
static const gdcm::PrivateTag t2(0x0029,0x0020,csaheader);

const PrivateTag & CSAHeader::GetCSAImageHeaderInfoTag()
{
  return t1;
}

const PrivateTag & CSAHeader::GetCSASeriesHeaderInfoTag()
{
  return t2;
}

} // end namespace gdcm

