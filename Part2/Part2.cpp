// Part2.cpp : Build a Bayesian classifier with a particular feature of the dataset
//             Test/Training set size restrictions imposed from Part 1
#include "BayesClassifier.h"
#include "Dataset.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>

std::string getInputFile();
size_t getFeatureID();

int main(int argc, char *argv[])
{
	// Get input file and feature ID
	std::string inputFile;
	size_t featureID;

	if (argc < 2)
	{
		inputFile = getInputFile();
		featureID = getFeatureID();
	}
	else if (argc < 3)
	{
		inputFile = argv[argc - 1];
		featureID = getFeatureID();
	}
	else
	{
		inputFile = argv[argc - 2];
		featureID = std::stoi(argv[argc - 1]);
	}

	try
	{
		// Create initial Dataset
		Dataset testData(inputFile);

		// Normalize data between -1 and 1
		double normalizeMin = -1.0;
		double normalizeMax = 1.0;
		testData.normalize(normalizeMin, normalizeMax);

		// Split Dataset training data-> ensure even amount of points in remaining set
		Dataset trainingData = testData.splitEven();

		// Build classifier using specified feature
		BayesClassifier classifier(trainingData, featureID, normalizeMin, normalizeMax);

		// Run testing data through classifier
		size_t nCorrectClassified = 0;
		for (size_t i = 0; i < testData.getNClasses(); ++i)
		{
			for (size_t j = 0; j < testData.getClass(i).getNPoints(); ++j)
			{
				double point = testData.getClass(i).getPoint(j, featureID);
				size_t classID = classifier.classify(point);

				std::cout << "\nPoint:          " << point << std::endl;
				std::cout << "Expected:       " << (i+1) << std::endl;
				std::cout << "Classification: " << (classID + 1) << std::endl;

				if (classID == i) ++nCorrectClassified;
			}
		}
		
		// Print results for classifier
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "Recognition Rate: " << (double)nCorrectClassified / testData.getNPoints() << std::endl;

		std::stringstream ss;
		ss << "distributions_feature"; ss << (featureID + 1); ss << ".csv";
		classifier.writeDistributions2CSV(ss.str());
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << std::endl;
		std::cerr << e.what() << std::endl;
		return -1;
	}

	return 0;
}

std::string getInputFile()
{
	std::string input;
	std::cout << "Please enter full pathname to input data file: ";
	std::getline(std::cin, input);
	return input;
}

size_t getFeatureID()
{
	std::string input;
	std::cout << "Please enter feature ID [1-N]: ";
	std::getline(std::cin, input);
	return std::stoi(input)-1;
}