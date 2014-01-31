#include <iostream>
#include <string>
#include <cstring>
#include <array>
#include <tuple>
#include <algorithm>
#include <stdexcept>

using namespace std;

class Matrix {
public: // Construction, Destruction 
	Matrix(int width, int height) 
		:  width_(width), height_(height) {

			array_ = new double[width * height];
			memset(array_, 0, sizeof(array_));
	}

	Matrix(int width, int height, const vector<double>& array) 
		:  width_(width), height_(height) {

		if (array.size() != (width*height))
			throw invalid_argument("items size should be width*height");

		array_ = new double[array.size()];

		int i = 0;
		for_each(begin(array), end(array), [this, &i](int n) {
			array_[i++] = n;
		});
	}

	Matrix(const Matrix& that) 
		:  width_(that.width_), height_(that.height_) {
		
		array_ = new double[width_ * height_];
		memcpy(array_, that.array_, sizeof(array_));
	}

	Matrix(Matrix&& that) 
		: width_(that.width_), height_(that.height_), array_(that.array_) {

		that.width_ = 0;
		that.height_ = 0;
		that.array_ = NULL;
	}

	Matrix& operator=(Matrix that) {
		 
		 width_ = that.width_;
		 height_ = that.height_;
		 std::swap(array_, that.array_);
		 
		 return *this;
	}

	~Matrix() {
		delete [] array_;
	}

public: // Element access
	double& at(int i, int j) {
		return array_[i*width_+j];
	}

	double at(int i, int j) const {
		return array_[i*width_+j];
	}

	double& operator()(int i, int j) {
		return at(i,j);
	}

	double operator()(int i, int j) const {
		return at(i,j);
	}
	
public: // Operations returning a modified object
	
	Matrix rotateBy90() {
		Matrix ret = transpose();

		// Swap each row
		for (auto i = 0; i < ret.height_; ++i) {
			for (auto j = 0; j < ret.width_ / 2; ++j) {
				swap(ret.at(i,j), ret.at(i, ret.width_-j-1));
			}
		}
		return ret;
	}

	Matrix transpose() {
		Matrix ret(height_, width_);

		for (auto i = 0; i < height_; ++i) {
			for (auto j = 0; j < width_; ++j) {
				ret.at(j,i) = at(i,j);
			}
		}

		return ret;
	}

private:
	void swap(double& a, double& b) {
		double temp = a;
		a = b;
		b = temp;
	}

public: // Input, Output
	friend ostream& operator<<(ostream& os, const Matrix& m);

private:	
	void print(ostream& out) const {
		for (auto i = 0; i < height_; ++i) {
			for (auto j = 0; j < width_; ++j) {
				out << at(i,j) << " ";
			}
			out << endl;
		}
	}

private: // Data
	int width_; // number of columns
	int height_; // number of rows
	double* array_;
};

ostream& operator<<(ostream& os, const Matrix& m)
{
    m.print(os);
    return os;
}

int main () {
	Matrix m(4,3, 
		vector<double>{
			1,2,3,4,
			5,6,7,8,
			9,10,11,12
		}
	);
	
	cout << "Matrix" << endl;
	cout << m << endl;

	cout << "Transpose" << endl;
	cout << m.transpose() << endl;

	cout << "Rotate by 90" << endl;
	cout << m.rotateBy90() << endl;

	return 0;
}