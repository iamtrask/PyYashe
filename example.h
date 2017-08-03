/* File : example.h */

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <pari/pari.h>
#include <time.h>
#include <string.h>
#include <iostream>
#define PARI_OLD_NAMES

double Uniform(void);
double Normal(void);
double Gauss(double mu, double sigma);
GEN Sample(int n, double sigma);

class Shape {
  public:
    Shape() {
      nshapes++;
    }
    virtual ~Shape() {
      nshapes--;
    }
    double  x, y;
    void    move(double dx, double dy);
    virtual double area() = 0;
    virtual double perimeter() = 0;
    static  int nshapes;
};

class Circle : public Shape {
  private:
    double radius;
  public:
    Circle(double r) : radius(r) { }
    virtual double area();
    virtual double perimeter();
};

class Square : public Shape {
  private:
    double width;
  public:
    Square(double w) : width(w) { }
    virtual double area();
    virtual double perimeter();
};
