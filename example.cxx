/* File : example.cxx */
#include "example.h"

using namespace std;

int verbose = 0;
int detailedverbose = 0;
int errorhalt = 0;

// Some functions for random sample generation
double Uniform(void) {
	return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}
/*********************************
Standard Normal Distribution: using Box-Miller method
**********************************/
double Normal(void) {
	return sqrt( -log(Uniform())*2.0 ) * sin( 2.0*M_PI*Uniform() );
}
/*******************************
Gaussian Distribution
********************************/
double Gauss(double mu, double sigma) {
	double z=sqrt( -2.0*log(Uniform()) ) * sin( 2.0*M_PI*Uniform() );
	return mu + sigma*z;
}
/********************************************************************
Error sampling function
GEN Sample(
	int n			: polynomial dimension
	double sigma	: standard deviation parameter

Contents :  Create n dimension random number vector(signed integer) based on Gaussian distribution (0, σ)
********************************************************************/
GEN Sample(int n, double sigma)
{
	// GEN ret	= cgetg(n + 1, t_VEC);
	// double z;
	// int i;
  //
	// for (i = 1; i <= n; i++) {
	// 	z = Gauss(0, sigma); z = abs(round(z)); /*absolute value of Gaussian distribution */
	// 	ret[i] = (long) stoi((long) z);
	// }

	return NULL;
}

// GEN randomElement(int n){
// 	GEN ret;
// 	ret = cgetg(n + 1, t_VEC);
// 	for(int i=0; i<n; i++){
// 		//gel(ret, i+1) = stoi(rand());
// 		gel(ret, i+1) = lift(gmodulo(stoi(rand()), stoi(300000)));
// 	}
// 	return ret;
// }
//
// GEN randomElementredbyt(int n, GEN t){
// 	GEN ret;
// 	ret = cgetg(n + 1, t_VEC);
// 	for(int i=0; i<n; i++){
// 		gel(ret, i+1) = lift(gmodulo(stoi(rand()), t));
// 	}
// 	return ret;
// }
//
// GEN encodeinput(GEN n, GEN t, long long int input){
// 	if(input>itos(t)){
// 		cout<<"[Error] Using the current encoding and t value, this input will give incorrect result due to reduction.";
// 		errorhalt = 1;
// 		return stoi(0);
// 	}
// 	GEN ret;
// 	ret = cgetg(itos(n) + 1, t_VEC);
// 	for(int i=0; i<itos(n); i++){
// 		if(i==0)
// 			gel(ret, i+1) = stoi(input);
// 		else
// 			gel(ret, i+1) = stoi(0);
// 	}
// 	if(verbose)
// 		cout<<"Input has been encoded\n";
// 	return ret;
// }
















/* Move the shape to a new location */
void Shape::move(double dx, double dy) {
  x += dx;
  y += dy;
}

int Shape::nshapes = 0;

double Circle::area() {
  return M_PI*radius*radius;
}

double Circle::perimeter() {
  return 2*M_PI*radius;
}

double Square::area() {
  return width*width;
}

double Square::perimeter() {
  return 4*width;
}
