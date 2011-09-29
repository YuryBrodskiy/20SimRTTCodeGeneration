/*
 * XVMatrix.h
 *
 *  Created on: Sep 7, 2011
 *      Author: yury
 */

#ifndef XVMATRIX_H_
#define XVMATRIX_H_
#include <vector>
#include "xxmatrix.h"
#include <memory.h>
#include <iostream>
#include "rtt/types/carray.hpp"

namespace common20sim {

class XVMatrix {
	double* const mat;
	RTT::types::carray<double> mat_carray;
	std::size_t rows;
	std::size_t columns;
	bool sizeCheck(std::size_t size);
public:
	XVMatrix();
	XVMatrix(XXMatrix& mat_source);
	XVMatrix(double* mat_source,std::size_t rows, std::size_t columns);
	XVMatrix(XVMatrix& copy);
	void makeClean();
	virtual ~XVMatrix();

	 std::size_t getColumns();
	 std::size_t getRows();
	 std::vector<double> getVector();
	 RTT::types::carray<double>& getCArray();
	 std::size_t size() const;
	 std::size_t capacity() const;

	double operator() (std::size_t row, std::size_t column) const;
	double& operator() (std::size_t row, std::size_t column);
	void setValues(std::vector<double>& inputs);
	void setValues(RTT::types::carray<double>& inputs);
	void setValues(double& inputs,std::size_t size);
	//Only used to fake vector<vector> behaviour
	// the values can not be changed
	// it returns the copy of the vector
//	inline std::vector<double> operator[](std::size_t row);
//	RTT::types::carray<double>& operator[](std::size_t row);
	//double& operator[] (std::size_t row, std::size_t column);

};

} /* namespace common20sim */


#endif /* XVMATRIX_H_ */
