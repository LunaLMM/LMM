#pragma once

#include <vector>
#include "assert.h"
#include <boost/shared_ptr.hpp>
#include <iostream>

//c'est la courbe spot des taux ZC

//Singleton...
class CourbeInput
{
private:
	std::vector<double> listeMatu_ ;
	std::vector<double> tauxZC_ ;

public:
	CourbeInput(void);
	CourbeInput(std::vector<double> listeMatu, std::vector<double> tauxZC);
	~CourbeInput(void);

	double get_tauxZC0(double T) ;
	void showCourbeInput() ;
};

typedef boost::shared_ptr<CourbeInput> courbeInput_PTR;
typedef boost::shared_ptr<const CourbeInput> courbeInput_CONSTPTR;