/* 
 * Program: vtkGRASSBridge
 * COPYRIGHT: (C) 2009 by Soeren Gebbert, soerengebbert@googlemail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

/**
 * \brief This class reads a grass raster map from a valid location/mapset
 * and creates a vtkImageData as output.
 *
 * To use this class make sure you are in a grass session and vtkGRASSInit was called first.
 *
 * The user can choose the current region, the default region, the raster map region or
 * a user defined region to open the raster map.
 * The position of the lower left edge of the created image will be set to the
 * east/south coordinates of the choosen region. The data spacing in x and y
 * direction will be set by the ew and ns resolution of the choosen region.
 * The raster map types CELL, FCELL or DCELL
 * will be conveted into int, float or double types.
 *
 * This class uses the vtkGRASSRasterMapReader to read the raster map rows into the memory.
 *
 * \author Soeren Gebbert
 * \author Berlin, Germany Aug. 2009
 * \author soerengebbert@googlemail.com
 * */

#ifndef __vtkGRASSRasterImageReader_h
#define __vtkGRASSRasterImageReader_h

#include <vtkImageAlgorithm.h>
#include "vtkGRASSBridgeIOWin32Header.h"
#include "vtkGRASSDefines.h"
#include "vtkGRASSRegion.h"
#include "vtkGRASSRasterMapReader.h"

class VTK_GRASS_BRIDGE_IO_EXPORT vtkGRASSRasterImageReader : public vtkImageAlgorithm
{
public:
  static vtkGRASSRasterImageReader *New();
  vtkTypeRevisionMacro(vtkGRASSRasterImageReader,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  //! \brief Get the data type of pixels in the imported data.
  //! As a convenience, the OutputScalarType is set to the same value.
  vtkGetMacro(DataScalarType, int);
  const char *GetDataScalarTypeAsString() { 
    return vtkImageScalarTypeNameMacro(this->DataScalarType);
  }

  vtkSetStringMacro(RasterName);
  vtkGetStringMacro(RasterName);
  vtkGetStringMacro(Mapset);

  void UseDefaultRegion(){this->SetRegionUsage(VTK_GRASS_REGION_DEFAULT);}
  void UseCurrentRegion(){this->SetRegionUsage(VTK_GRASS_REGION_CURRENT);}
  void UseRasterRegion(){this->SetRegionUsage(VTK_GRASS_REGION_RASTER);}
  void UseUserDefinedRegion(){this->SetRegionUsage(VTK_GRASS_REGION_USER);}
  vtkGetMacro(RegionUsage, int);

  /*! \brief Set the region which should be used to open the raster map
   *
   * */
  void SetRegion(vtkGRASSRegion *region) {this->RasterMap->SetRegion(region);}
  vtkGRASSRegion *GetRegion() {return this->RasterMap->GetRegion();}

  /*! \brief Return the RasterMap object
   *
   * */
  vtkGetObjectMacro(RasterMap, vtkGRASSRasterMapReader);

protected:
  vtkGRASSRasterImageReader();
  ~vtkGRASSRasterImageReader();

  vtkSetStringMacro(Mapset);
  vtkSetMacro(RegionUsage, int);

  char *RasterName;
  char *Mapset;
  int DataScalarType;
  int RegionUsage;

  int DataExtent[6];
  double DataSpacing[3];
  double DataOrigin[3];

  vtkGRASSRasterMapReader *RasterMap;

  vtkSetMacro(DataScalarType,int);
  void SetDataScalarTypeToDouble(){this->SetDataScalarType(VTK_DOUBLE);}
  void SetDataScalarTypeToFloat(){this->SetDataScalarType(VTK_FLOAT);}
  void SetDataScalarTypeToInt(){this->SetDataScalarType(VTK_INT);}

  virtual int RequestInformation (vtkInformation*,
                                  vtkInformationVector**,
                                  vtkInformationVector*);
  virtual void ExecuteData(vtkDataObject *data);

private:
  vtkGRASSRasterImageReader(const vtkGRASSRasterImageReader&);  // Not implemented.
  void operator=(const vtkGRASSRasterImageReader&);  // Not implemented.
};


#endif