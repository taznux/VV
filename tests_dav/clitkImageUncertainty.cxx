/*=========================================================================
                                                                                
  Program:   clitk
  Module:    $RCSfile: clitkImageUncertainty.cxx,v $
  Language:  C++
  Date:      $Date: 2010/01/19 16:02:53 $
  Version:   $Revision: 1.1 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                             
=========================================================================*/

#ifndef CLITKIMAGEUNCERTAINTY_CXX
#define CLITKIMAGEUNCERTAINTY_CXX

/**
 =================================================
 * @file   clitkImageUncertainty.cxx
 * @author David Sarrut <david.sarrut@creatis.insa-lyon.fr>
 * @date   04 Jul 2006 14:03:57
 * 
 * @brief  
 * 
 * 
 =================================================*/

// clitk include
#include "clitkImageUncertainty_ggo.h"
#include "clitkIO.h"

// itk include
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

//====================================================================
int main(int argc, char * argv[]) {

  // init command line
  GGO(clitkImageUncertainty, args_info);
  CLITK_INIT;

  // Declare main types
  typedef float                          PixelType;
  const unsigned int                              Dimension=3;
  typedef itk::Image< PixelType, Dimension >      ImageType;
  
  // Read images
  ImageType::Pointer input = 
	clitk::readImage<ImageType>(args_info.input_arg, args_info.verbose_flag);
  ImageType::Pointer inputSquared = 
	clitk::readImage<ImageType>(args_info.inputSquared_arg, args_info.verbose_flag);

  // Create Output
  ImageType::Pointer output = ImageType::New();
  output->SetRegions(input->GetLargestPossibleRegion());
  output->CopyInformation(input);
  output->Allocate();  

  // Loop
  typedef itk::ImageRegionConstIterator<ImageType> ConstIteratorType;
  ConstIteratorType pi(input, input->GetLargestPossibleRegion());
  ConstIteratorType pii(inputSquared, inputSquared->GetLargestPossibleRegion());
  pi.Begin();
  pii.Begin();
  typedef itk::ImageRegionIterator<ImageType> IteratorType;
  IteratorType po(output, output->GetLargestPossibleRegion());
  po.Begin();
  
  int NumberOfEvents = args_info.NumberOfEvents_arg;
  while ( !pi.IsAtEnd() ) {  
	double squared = pii.Get();
	double mean = pi.Get();
	po.Set( sqrt((NumberOfEvents*squared - mean*mean) / 
				 ((NumberOfEvents-1)*(mean*mean)) ) );
	++pi;
	++pii;
	++po;
  }
// 	  *po = sqrt( (NumberOfEvents*squared - mean*mean) / 
// ((NumberOfEvents-1)*(mean*mean)) );
// 	  ++po;



  // Write output image
  // DD(clitk::GetExtension(args_info.output_arg));
  if (clitk::GetExtension(args_info.output_arg) != "txt") {
	clitk::writeImage<ImageType>(output, args_info.output_arg, args_info.verbose_flag);
  }
  else {
	std::ofstream os;													
	clitk::openFileForWriting(os, args_info.output_arg);				
	typedef itk::ImageRegionConstIterator<ImageType> IteratorType; 
	IteratorType pi(output, output->GetLargestPossibleRegion());		
	pi.Begin();														
	os << "# Image size = " << output->GetLargestPossibleRegion().GetSize() << std::endl; 
	os << "# Image spacing = " << output->GetSpacing() << std::endl;
	os << "# Number of events = " << NumberOfEvents << std::endl;
	while (!pi.IsAtEnd()) {											
	  os << pi.Get() << std::endl;									
	  ++pi;															
	}			
  }
}


#endif /* end #define CLITKIMAGEUNCERTAINTY_CXX */

