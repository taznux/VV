/*=========================================================================
  Program:   vv                     http://www.creatis.insa-lyon.fr/rio/vv

  Authors belong to: 
  - University of LYON              http://www.universite-lyon.fr/
  - Léon Bérard cancer center       http://oncora1.lyon.fnclcc.fr
  - CREATIS CNRS laboratory         http://www.creatis.insa-lyon.fr

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the copyright notices for more information.

  It is distributed under dual licence

  - BSD        See included LICENSE.txt file
  - CeCILL-B   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
  ======================================================================-====*/

#ifndef CLITKAUTOCROPFILTER_TXX
#define CLITKAUTOCROPFILTER_TXX

// clitk
#include "clitkCommon.h"
#include "clitkSegmentationUtils.h"

// itk
#include "itkAutoCropLabelMapFilter.h"
#include "itkStatisticsLabelObject.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"

namespace clitk {

  //--------------------------------------------------------------------
  template <class TImageType>
  AutoCropFilter<TImageType>::
  AutoCropFilter():itk::ImageToImageFilter<TImageType, TImageType>() {
    this->SetNumberOfRequiredInputs(1);
    m_BackgroundValue  = 0;
  }
  //--------------------------------------------------------------------


  //--------------------------------------------------------------------
  template <class TImageType>
  void 
  AutoCropFilter<TImageType>::
  SetInput(const TImageType * image) {
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput(0, const_cast<TImageType *>( image ));
  }
  //--------------------------------------------------------------------
  

  //--------------------------------------------------------------------
  template <class TImageType>
  void 
  AutoCropFilter<TImageType>::  
  SetBackgroundValue(ImagePixelType p) {
    m_BackgroundValue = p;
  }
  //--------------------------------------------------------------------


  //--------------------------------------------------------------------
  template <class TImageType>
  void 
  AutoCropFilter<TImageType>::
  GenerateOutputInformation() {    

    // Superclass
    // do not call the superclass' implementation of this method since
    // this filter allows the input the output to be of different dimensions
    // Superclass::GenerateOutputInformation();

    // Get input pointers
    ImageConstPointer input = dynamic_cast<const ImageType*>(itk::ProcessObject::GetInput(0));
    
    // Get output pointer
    ImagePointer output = this->GetOutput(0);
  
    // Convert to LabelMap
    static const unsigned int Dim = ImageType::ImageDimension;
    //    typedef unsigned long LabelType; // unsigned long needed (!!??)
    typedef itk::StatisticsLabelObject< LabelType, Dim > LabelObjectType;
    typedef itk::LabelMap< LabelObjectType > LabelMapType;
    typedef itk::LabelImageToLabelMapFilter<ImageType, LabelMapType> ImageToMapFilterType;
    typename ImageToMapFilterType::Pointer imageToLabelFilter = ImageToMapFilterType::New();  
    imageToLabelFilter->SetBackgroundValue(m_BackgroundValue);
    imageToLabelFilter->SetInput(input);
    
    // AutoCrop
    typedef itk::AutoCropLabelMapFilter<LabelMapType> AutoCropFilterType;
    typename AutoCropFilterType::Pointer autoCropFilter = AutoCropFilterType::New();
    autoCropFilter->SetInput(imageToLabelFilter->GetOutput());

    // Convert to LabelImage
    typedef itk::LabelMapToLabelImageFilter<LabelMapType, ImageType> MapToImageFilterType;
    typename MapToImageFilterType::Pointer labelToImageFilter = MapToImageFilterType::New();       
    labelToImageFilter->SetInput(autoCropFilter->GetOutput());

    // Go ! (needed)
    labelToImageFilter->Update();
    m_labeImage = labelToImageFilter->GetOutput();

    // Update the output size
    m_Region = m_labeImage->GetLargestPossibleRegion();
    output->SetLargestPossibleRegion(m_Region);
    output->SetRequestedRegion(m_Region);
    output->SetBufferedRegion(m_Region);
    output->SetRegions(m_Region);
  }
  //--------------------------------------------------------------------
   
  //--------------------------------------------------------------------
  template <class TImageType>
  void 
  AutoCropFilter<TImageType>::
  GenerateData() {
    // Get input pointers
    ImageConstPointer input = dynamic_cast<const ImageType*>(itk::ProcessObject::GetInput(0));
  
    // Extract the region
    typedef itk::RegionOfInterestImageFilter<ImageType, ImageType> CropFilterType;
    m_labeImage->SetRequestedRegion(m_labeImage->GetLargestPossibleRegion());
    typename CropFilterType::Pointer cropFilter = CropFilterType::New();
    cropFilter->SetInput(m_labeImage);
    cropFilter->SetRegionOfInterest(m_Region);

    // Go ! 
    cropFilter->Update();

    // Get (graft) output (SetNthOutput does not fit here because of Origin).
    this->GraftOutput(cropFilter->GetOutput());
  }
  //--------------------------------------------------------------------
   
}//end clitk
 
#endif //#define CLITKAUTOCROPFILTER