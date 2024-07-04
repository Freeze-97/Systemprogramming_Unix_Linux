/*
 * MonteCarlo.cpp
 *
 * Author: Tommy Yasi (toya1800)
 */
#include "MonteCarlo.h"

MonteCarlo::MonteCarlo(int p_throws) {
	throws = p_throws;
	PI = calculate_PI();
}

double MonteCarlo::calculate_PI() const {
	// x and Y coordinates
	double random_x, random_y, diameter, pi;
	int circle_points = 0, square_points = 0;

	// Throw as long as it's less than p_throws, which is the max
	for(int i = 0; i < throws; i++) {
        // Give random number between 0 and 1 with 6 decimals to x and y
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        random_x = dis(gen);
        random_y = dis(gen);

        // Calculate diameter d=x²+y²
        diameter = random_x * random_x + random_y * random_y;

        // Check if it's less than 1
        if (diameter <= 1) {
            circle_points++;
        }

        // Increase square points no matter result
        square_points++;
    }

	// Estimate PI based on all circle and square points
	pi = double(4 * circle_points) / square_points;

	return pi;
}

void MonteCarlo::write_to_file(const std::string &file_name) const {
    // Create text file
    std::ofstream file(file_name);

    if(file.is_open()) {
        // Write to file
        file << PI;

        // Close file
        file.close();
    } else {
        perror("Unable to open file");
    }
}


