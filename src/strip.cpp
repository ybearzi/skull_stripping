#include "strip.h"

#include "csv_writer.h"

// Inputs, Outputs

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

// Closing Filter

#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

// Qt declarations

#include <QString>

#include <iostream>

// Input Ouputs

typedef itk::ImageFileReader< Image > Reader;
typedef itk::ImageFileWriter< Image > Writer;
typedef itk::ImageFileReader< ImageBinary > ReaderBinary;
typedef itk::ImageFileWriter< ImageBinary > WriterBinary;

// Iterators

typedef itk::ImageRegionConstIterator< Image > ConstIterator;
typedef itk::ImageRegionIterator< Image > Iterator;
typedef itk::ImageRegionConstIterator< ImageBinary > ConstIteratorBinary;
typedef itk::ImageRegionIterator< ImageBinary > IteratorBinary;

// For closing

typedef itk::BinaryBallStructuringElement<PixelBinaryType, Dim> StructuringElementBinary;
typedef itk::BinaryMorphologicalClosingImageFilter< ImageBinary, ImageBinary, StructuringElementBinary > ClosingFilterBinary;

void strip(const char *input_t1, const char *input_t2, const char *input_label, const char *output_t1, const char *output_t2, const char *output_excel)
{
	// Reading files

	Reader::Pointer reader_t1 = Reader::New(), reader_t2 = Reader::New(), reader_label = Reader::New();
	reader_t1->SetFileName(input_t1);
	reader_t2->SetFileName(input_t2);
	reader_label->SetFileName(input_label);
	reader_t1->Update(); reader_t2->Update(); reader_label->Update();

	// Making Images

	Image::Pointer image_t1 = reader_t1->GetOutput();
	Image::Pointer image_t2 = reader_t2->GetOutput();
	Image::Pointer image_label = reader_label->GetOutput();

	// Making a binary mask from label image
	
	ImageBinary::Pointer mask = label2mask(image_label);

	WriterBinary::Pointer writer_mask_before = WriterBinary::New();
	writer_mask_before->SetFileName("mask_before.nrrd");
	writer_mask_before->SetInput(mask);
	writer_mask_before->Update();

	// Closing mask because of holes between labels
	
	mask = close(mask);
	
	fill_csv(image_t1, image_t2, image_label, output_excel);

	// Skull stripping

	Image::Pointer image_output_t1 = binary_and(image_t1, mask);
	Image::Pointer image_output_t2 = binary_and(image_t2, mask);

	// Writting 2 files

	Writer::Pointer writer_t1 = Writer::New();
	writer_t1->SetFileName(output_t1);
	writer_t1->SetInput(image_output_t1);
	writer_t1->Update();

	Writer::Pointer writer_t2 = Writer::New();
	writer_t2->SetFileName(output_t2);
	writer_t2->SetInput(image_output_t2);
	writer_t2->Update();

	WriterBinary::Pointer writer_mask = WriterBinary::New();
	writer_mask->SetFileName("mask.nrrd");
	writer_mask->SetInput(mask);
	writer_mask->Update();

	std::cout<<"Skull stripping done successfully"<<std::endl;
}

ImageBinary::Pointer label2mask(Image::Pointer label, unsigned int element)
{
	ImageBinary::Pointer mask = ImageBinary::New();
	Image::RegionType region = label->GetLargestPossibleRegion();
	mask->SetRegions(region);
	mask->Allocate();
	mask->FillBuffer(0);


	ConstIterator iterator_label(label, label->GetLargestPossibleRegion());
	IteratorBinary iterator_mask(mask, mask->GetLargestPossibleRegion());	

	try
	{
		while(!iterator_label.IsAtEnd())
		{
			if(iterator_label.Get() == element && element || iterator_label.Get() != 0 && !element)
				iterator_mask.Set(1);
			++iterator_label; ++iterator_mask;
		}
	}
	catch(itk::ExceptionObject & err)
	{
		std::cerr<<"Exception caught, outside bounds using iterators"<<std::endl;
		std::cerr<<err<<std::endl;
	}

	return mask;
}

ImageBinary::Pointer close(ImageBinary::Pointer image_binary)
{
	// Initialising filters
	
	ClosingFilterBinary::Pointer filter = ClosingFilterBinary::New();

	// Initialising structuring element

	StructuringElementBinary structuring_element;
	structuring_element.SetRadius(2);
	structuring_element.CreateStructuringElement();

	// Putting structuring element in filter
	
	filter->SetKernel(structuring_element);

	filter->SetInput(image_binary);
	filter->SetForegroundValue(1);

	ImageBinary::Pointer output_binary = ImageBinary::New();
	output_binary = filter->GetOutput();
	output_binary->Update();

	return output_binary;
}

Image::Pointer binary_and(Image::Pointer input, ImageBinary::Pointer mask)
{
	Image::Pointer output = Image::New();
	Image::RegionType region = input->GetLargestPossibleRegion();
	output->SetRegions(region);
	output->Allocate();

	ConstIteratorBinary iterator_mask(mask, mask->GetLargestPossibleRegion());
	Iterator iterator_input(input, input->GetLargestPossibleRegion());
	Iterator iterator_output(output, output->GetLargestPossibleRegion());

	// all images should be the same size
	
	try
	{
		while(!iterator_input.IsAtEnd())
		{
			if(iterator_mask.Get() == 0)
				iterator_output.Set(0);
			else
				iterator_output.Set(iterator_input.Get());
			++iterator_input; ++iterator_mask; ++iterator_output;
		}
	}
	catch(itk::ExceptionObject & err)
	{
		std::cout<<"Error in binary process, images might not be the same size"<<std::endl;
		std::cout<<err<<std::endl;
	}
	
	return output;
}

void fill_csv(Image::Pointer im1, Image::Pointer im2, Image::Pointer label, const char *output_excel)
{
	CsvWriter csv = CsvWriter(QString(output_excel));

	csv.insert(2,1,QString("T1"));
	csv.insert(2,2,QString("WM"));
	csv.insert(3,2,QString("GM"));
	csv.insert(4,2,QString("CSF"));
	csv.insert(1,3,QString("Volume"));
	csv.insert(1,4,QString("Mean"));
	csv.insert(1,5,QString("Min"));
	csv.insert(1,6,QString("Max"));

	csv.insert(7,1,QString("T2"));
	csv.insert(7,2,QString("WM"));
	csv.insert(8,2,QString("GM"));
	csv.insert(9,2,QString("CSF"));
	csv.insert(6,3,QString("Volume"));
	csv.insert(6,4,QString("Mean"));
	csv.insert(6,5,QString("Min"));
	csv.insert(6,6,QString("Max"));
	
	// White Matter

	Image::Pointer wm1 = binary_and(im1, label2mask(label,1));
	unsigned int min = get_min(wm1);
	unsigned int max = get_max(wm1);
	float mean = get_mean(wm1);
	unsigned int volume = get_volume(wm1);

	csv.insert(2,3,QString("%1").arg(volume));
	csv.insert(2,4,QString("%1").arg(mean));
	csv.insert(2,5,QString("%1").arg(min));
	csv.insert(2,6,QString("%1").arg(max));

	Image::Pointer wm2 = binary_and(im2, label2mask(label,1));
	min = get_min(wm2);
	max = get_max(wm2);
	mean = get_mean(wm2);
	volume = get_volume(wm2);

	csv.insert(7,3,QString("%1").arg(volume));
	csv.insert(7,4,QString("%1").arg(mean));
	csv.insert(7,5,QString("%1").arg(min));
	csv.insert(7,6,QString("%1").arg(max));

	// Gray Matter
	
	Image::Pointer gm1 = binary_and(im1, label2mask(label,2));
	min = get_min(gm1);
	max = get_max(gm1);
	mean = get_mean(gm1);
	volume = get_volume(gm1);

	csv.insert(3,3,QString("%1").arg(volume));
	csv.insert(3,4,QString("%1").arg(mean));
	csv.insert(3,5,QString("%1").arg(min));
	csv.insert(3,6,QString("%1").arg(max));

	Image::Pointer gm2 = binary_and(im2, label2mask(label,2));
	min = get_min(gm2);
	max = get_max(gm2);
	mean = get_mean(gm2);
	volume = get_volume(gm2);

	csv.insert(8,3,QString("%1").arg(volume));
	csv.insert(8,4,QString("%1").arg(mean));
	csv.insert(8,5,QString("%1").arg(min));
	csv.insert(8,6,QString("%1").arg(max));

	// CSF
	
	Image::Pointer csf1 = binary_and(im1, label2mask(label,3));
	min = get_min(csf1);
	max = get_max(csf1);
	mean = get_mean(csf1);
	volume = get_volume(csf1);

	csv.insert(4,3,QString("%1").arg(volume));
	csv.insert(4,4,QString("%1").arg(mean));
	csv.insert(4,5,QString("%1").arg(min));
	csv.insert(4,6,QString("%1").arg(max));

	Image::Pointer csf2 = binary_and(im2, label2mask(label,3));
	min = get_min(csf2);
	max = get_max(csf2);
	mean = get_mean(csf2);
	volume = get_volume(csf2);

	csv.insert(9,3,QString("%1").arg(volume));
	csv.insert(9,4,QString("%1").arg(mean));
	csv.insert(9,5,QString("%1").arg(min));
	csv.insert(9,6,QString("%1").arg(max));

	csv.update();
}

unsigned int get_min(Image::Pointer im)
{
	ConstIterator it(im, im->GetLargestPossibleRegion());
	
	unsigned int min = ~0;

	try
	{
		while(!it.IsAtEnd())
		{
			if(it.Get() && it.Get()<min)
				min = it.Get();
			++it;
		}
	}
	catch( itk::ExceptionObject & err)
	{
		std::cout<<"Error calculating minimum, out of bounds"<<std::endl;
		std::cout<<err<<std::endl;
	}

	return min;
}

unsigned int get_max(Image::Pointer im)
{
	ConstIterator it(im, im->GetLargestPossibleRegion());
	
	unsigned int max = 0;

	try
	{
		while(!it.IsAtEnd())
		{
			if(it.Get()>max)
				max = it.Get();
			++it;
		}
	}
	catch( itk::ExceptionObject & err)
	{
		std::cout<<"Error calculating maximum, out of bounds"<<std::endl;
		std::cout<<err<<std::endl;
	}

	return max;
}

float get_mean(Image::Pointer im)
{
	ConstIterator it(im, im->GetLargestPossibleRegion());
	
	float mean = 0, N=0;

	try
	{
		while(!it.IsAtEnd())
		{
			if(it.Get()!=0)
			{
				mean = N/(N+1)*mean + it.Get()/(N+1);
				++N;
			}
			++it;
		}
	}
	catch( itk::ExceptionObject & err)
	{
		std::cout<<"Error calculating mean, out of bounds"<<std::endl;
		std::cout<<err<<std::endl;
	}

	return mean;
}

unsigned int get_volume(Image::Pointer im)
{
	ConstIterator it(im, im->GetLargestPossibleRegion());
	
	unsigned int volume;

	try
	{
		while(!it.IsAtEnd())
		{
			if(it.Get()!=0)
				++volume;
			++it;
		}
	}
	catch( itk::ExceptionObject & err)
	{
		std::cout<<"Error calculating volume, out of bounds"<<std::endl;
		std::cout<<err<<std::endl;
	}

	return volume;
}
