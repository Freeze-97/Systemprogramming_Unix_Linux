/*
 * MonteCarlo.h
 *
 * Author: Tommy Yasi (toya1800)
 */

#ifndef MONTECARLO_CPP_
#define MONTECARLO_CPP_

#include<iostream>
#include <fstream>
#include <iomanip>
#include <random>

class MonteCarlo {
private:
	int throws;
	double PI;
public:
	explicit MonteCarlo(int p_throws);
	double calculate_PI() const;
	void write_to_file(const std::string &file_name) const;
};

#endif /* MONTECARLO_CPP_ */
