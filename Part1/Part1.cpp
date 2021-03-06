// Part1.cpp : Calculate FDR for each feature and determine the best
#include "Dataset.h"
#include <iostream>
#include <iomanip>
#include <memory>

int main(int argc, char *argv[])
{
	// Get input file
	std::string inputFile = "";
	if (argc < 2)
	{
		std::cout << "Please enter full pathname to input data file: ";
		std::getline(std::cin, inputFile);
	}
	else
	{
		inputFile = argv[argc - 1];
	}

	try
	{
		// Create initial Dataset
		Dataset testData(inputFile);
		testData.normalize(-1.0, 1.0);

		// Print recieved data
		//std::cout.precision(3);
		//std::cout << "Data Recieved:" << std::endl;
		//std::cout << "--------------" << std::endl;
		//testData.write(std::cout, "\t");

		// Split Dataset training data
		Dataset trainingData = testData.splitEven();

		// Find best feature for the dataset
		size_t bestFeature = -1;
		double fdr = -DBL_MAX;

		// Calculate and print FDR values
		std::cout.precision(3);
		std::cout << "------------------------------" << std::endl;
		std::cout << "Calculated Feature FDR Values:" << std::endl;
		std::cout << "------------------------------" << std::endl;

		for (size_t f = 0; f < trainingData.getNFeatures(); ++f)
		{
			double tmpFDR = trainingData.calculateFDRForFeature(f);

			if (tmpFDR > fdr)
			{
				bestFeature = f + 1;
				fdr = tmpFDR;
			}

			std::cout << "Feature ID: " << (f + 1) << "\tFDR: " << tmpFDR << std::endl;
		}

		// Print best feature
		std::cout << "-------------------------------" << std::endl;
		std::cout << "Best Feature: " << bestFeature << "\tFDR: " << fdr << std::endl;
		std::cout << "-------------------------------" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << std::endl;
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

