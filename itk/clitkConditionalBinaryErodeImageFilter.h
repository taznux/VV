/*=========================================================================
  Program:   vv                     http://www.creatis.insa-lyon.fr/rio/vv

  Authors belong to: 
  - University of LYON              http://www.universite-lyon.fr/
  - Léon Bérard cancer center       http://www.centreleonberard.fr
  - CREATIS CNRS laboratory         http://www.creatis.insa-lyon.fr

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the copyright notices for more information.

  It is distributed under dual licence

  - BSD        See included LICENSE.txt file
  - CeCILL-B   http://www.cecill.info/licences/Licence_CeCILL-B_V1-en.html
===========================================================================**/
#ifndef clitkConditionalBinaryErodeImageFilter_h
#define clitkConditionalBinaryErodeImageFilter_h

/* =================================================
 * @file   clitkConditionalBinaryErodeImageFilter.h
 * @author 
 * @date   
 * 
 * @brief 
 * 
 ===================================================*/


// clitk include
#include "clitkIO.h"
#include "clitkCommon.h"

//itk include
#include "itkImageToImageFilter.h"
#include <vector>
#include <queue>
#include "itkBinaryMorphologyImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkNeighborhoodIterator.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhood.h"
#include "itkImageBoundaryCondition.h"
#include "itkImageRegionIterator.h"
#include "itkConceptChecking.h"

namespace clitk 
{

  template <class TInputImage, class TOutputImage, class TKernel>
  class ITK_EXPORT ConditionalBinaryErodeImageFilter :
    public itk::BinaryMorphologyImageFilter< TInputImage, TOutputImage, TKernel >
  {
  public:
    //----------------------------------------
    // ITK
    //----------------------------------------
    typedef ConditionalBinaryErodeImageFilter                         Self;
    typedef itk::BinaryMorphologyImageFilter<TInputImage, TOutputImage, TKernel> Superclass;
    typedef itk::SmartPointer<Self>                                   Pointer;
    typedef itk::SmartPointer<const Self>                             ConstPointer;
   
    // Method for creation through the object factory
    itkNewMacro(Self);  

    // Run-time type information (and related methods)
    itkTypeMacro(BinaryErodeImageFilter, BinaryMorphologyImageFilterr );

    /** Dimension of the domain space. */
    itkStaticConstMacro(InputImageDimension, unsigned int, Superclass::InputImageDimension);
    itkStaticConstMacro(OutputImageDimension, unsigned int, Superclass::OutputImageDimension);
    /** Extract the dimension of the kernel */
    itkStaticConstMacro(KernelDimension, unsigned int,
			TKernel::NeighborhoodDimension);
 
    //----------------------------------------
    // Typedefs
    //----------------------------------------
    typedef TInputImage  InputImageType;
    typedef TOutputImage OutputImageType;
    typedef TKernel      KernelType;

    /** Kernel (structuring element) iterator. */
    typedef typename KernelType::ConstIterator KernelIteratorType;
    
    /** Image typedef support. */
    typedef typename InputImageType::PixelType               InputPixelType;
    typedef typename OutputImageType::PixelType              OutputPixelType;
    typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
    typedef typename InputImageType::OffsetType              OffsetType;
    typedef typename InputImageType::IndexType               IndexType;
    
    typedef typename InputImageType::RegionType  InputImageRegionType;
    typedef typename OutputImageType::RegionType OutputImageRegionType;
    typedef typename InputImageType::SizeType    InputSizeType;
    
    /** Set the value in the image to consider as "foreground". Defaults to
     * maximum value of PixelType. This is an alias to the
     * ForegroundValue in the superclass. */
    void SetErodeValue(const InputPixelType& value)
    { this->SetForegroundValue( value ); }
    
    /** Get the value in the image considered as "foreground". Defaults to
     * maximum value of PixelType. This is an alias to the
     * ForegroundValue in the superclass. */
    InputPixelType GetErodeValue() const
    { return this->GetForegroundValue(); }
    
  protected:
    ConditionalBinaryErodeImageFilter();
    virtual ~ConditionalBinaryErodeImageFilter(){}
    void PrintSelf(std::ostream& os, itk::Indent indent) const;
    
    void GenerateData();
    
    // type inherited from the superclass
    typedef typename Superclass::NeighborIndexContainer NeighborIndexContainer;
    
  private:
    ConditionalBinaryErodeImageFilter(const Self&); //purposely not implemented
    void operator=(const Self&); //purposely not implemented
    
  };
  
  
} // end namespace clitk

#ifndef ITK_MANUAL_INSTANTIATION
#include "clitkConditionalBinaryErodeImageFilter.txx"
#endif

#endif // #define clitkConditionalBinaryErodeImageFilter_h


