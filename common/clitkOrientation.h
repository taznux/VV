#ifndef CLITKORIENTATION_H
#define CLITKORIENTATION_H
/**
 ===================================================================
 * @file   clitkOrientation.h
 * @author David Sarrut <David.Sarrut@creatis.insa-lyon.fr>
 * @date   01 Nov 2006 18:00:32

 * @brief  

 ===================================================================*/

// itk include
#include "itkSpatialOrientation.h"
#include "itkPoint.h"

namespace clitk {
  
   //====================================================================
  // From a letter to an SpatialOrientation::CoordinateTerms
  itk::SpatialOrientation::CoordinateTerms GetOrientation(char c);

  //====================================================================
  // From three letters to an SpatialOrientation
  itk::SpatialOrientation::ValidCoordinateOrientationFlags GetOrientation(char a, char b, char c);
  itk::SpatialOrientation::ValidCoordinateOrientationFlags GetOrientation(const std::string & orient);
  
  //====================================================================
  // Check that the orientations are differents
  bool CheckOrientation(itk::SpatialOrientation::CoordinateTerms a, itk::SpatialOrientation::CoordinateTerms b);
   
  //====================================================================
  // Get orientation at position i
  itk::SpatialOrientation::CoordinateTerms GetOrientation(const int i, const itk::SpatialOrientation::ValidCoordinateOrientationFlags orient);
  
  //====================================================================
  // Find where is the dimension Dim in this orientaof orientation
  int WhereIsDimInThisOrientation(const int dim, const itk::SpatialOrientation::ValidCoordinateOrientationFlags flag);
  
  //====================================================================
  // Flip point coordinate
  void FlipPoint(const itk::Point<double, 3> in, 
				 const itk::SpatialOrientation::ValidCoordinateOrientationFlags inFlag, 
				 const itk::SpatialOrientation::ValidCoordinateOrientationFlags outFlag, 
				 const itk::Point<double, 3> & imageSize,
				 itk::Point<double, 3> & out);
  
  //====================================================================
  // Get the dimension of this term
  int GetDim(const itk::SpatialOrientation::CoordinateTerms t); 

} // end namespace

#endif /* end #define CLITKORIENTATION_H */

