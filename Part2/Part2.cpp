#include "BayesClassifier.h"
#include "Dataset.h"
#include <iostream>
#include <sstream>
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

		// Normalize data between -1 and 1
		double normalizeMin = -1.0;
		double normalizeMax = 1.0;
		testData.normalize(normalizeMin, normalizeMax);

		// Split Dataset training data-> ensure even amount of points in remaining set
		Dataset trainingData = testData.split(true);

		// Using Feature 1 (Determined best classifying feature from Part 1):
		size_t feature = 1;
		BayesClassifier classifier(trainingData, feature, normalizeMin, normalizeMax);

		// 3. Run training data through classifier
		size_t nCorrectClassified = 0;
		for (size_t i = 0; i < testData.getNClasses(); ++i)
		{
			for (size_t j = 0; j < testData.getClass(i).getNPoints(); ++j)
			{
				double point = testData.getClass(i).getPoint(j,feature);
				size_t classID = classifier.classify(point);

				std::cout << "\nPoint:          " << point << std::endl;
				std::cout << "Expected:       " << (i+1) << std::endl;
				std::cout << "Classification: " << (classID + 1) << std::endl;

				if (classID == i) ++nCorrectClassified;
			}
		}
		
		// 4. Print results for classifier
		std::cout << "-------------------------------------" << std::endl;
		std::cout << "Recognition Rate: " << (double)nCorrectClassified / testData.getNPoints() << std::endl;

		std::stringstream ss;
		ss << "seeds_distributions_feature"; ss << (feature + 1); ss << ".csv";
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

