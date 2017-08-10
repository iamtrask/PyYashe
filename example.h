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

using namespace std;

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

double Uniform(void) {
    return ((double)rand()+1.0)/((double)RAND_MAX+2.0);
}

double Normal(void) {
    return sqrt( -log(Uniform())*2.0 ) * sin( 2.0*M_PI*Uniform() );
}

double Gauss(double mu, double sigma) {
    double z=sqrt( -2.0*log(Uniform()) ) * sin( 2.0*M_PI*Uniform() );
    return mu + sigma*z;
}

GEN Sample(int n, double sigma)
{
    GEN ret = cgetg(n + 1, t_VEC);
    double z;
    int i;
    
    for (i = 1; i <= n; i++) {
        z = Gauss(0, sigma); z = abs(round(z)); /*absolute value of Gaussian distribution */
        ret[i] = (long) stoi((long) z);
    }
    
    return ret;
}

GEN randomElement(int n){
    GEN ret;
    ret = cgetg(n + 1, t_VEC);
    for(int i=0; i<n; i++){
        gel(ret, i+1) = lift(gmodulo(stoi(rand()), stoi(300000)));
    }
    return ret;
}

// Coefficient encoding of a single number into a polynomial
GEN encodeinput(GEN n, GEN t, long long int input){
    if(input>itos(t)){
        std::cout<<"[Error] Using the current encoding and t value, this input will give incorrect result due to reduction.";
        return stoi(0);
    }
    GEN ret;
    ret = cgetg(itos(n) + 1, t_VEC);
    for(int i=0; i<itos(n); i++){
        if(i==0)
            gel(ret, i+1) = stoi(input);
        else
            gel(ret, i+1) = stoi(0);
    }
    return ret;
}

// Coefficient encoding of an integer vector into a polynomial
GEN encodeinputvector(GEN n, GEN t, GEN input){
    GEN ret;
    ret = cgetg(itos(n) + 1, t_VEC);
    int len = lg(input)-1;
    for(int i=0; i<itos(n); i++){
        if(i<len)
            gel(ret, i+1) = gel(input, i+1);
        else
            gel(ret, i+1) = stoi(0);
    }
    return ret;
}

struct keyset{
    GEN pk;
    GEN sk;
    GEN finv = stoi(0);
};

struct params{
    GEN n;
    GEN q;
    GEN t;
    GEN modulus = stoi(0);
};

// Function to generate keys.
keyset* keygen(params* param){
    GEN n = param->n;
    GEN q = param->q;
    GEN t = param->t;
    keyset* keys = new keyset;
    GEN one = stoi(1);
    GEN temp, temp1, modulus, f, fdash, finv, g, h;
    temp = cgetg(itos(n) + 2, t_VEC);
    for(int i=0; i<itos(n)+1; i++){
        gel(temp, i+1) = stoi(0);
    }
    gel(temp, 1) = stoi(1);
    gel(temp, itos(n)+1) = stoi(1);
    modulus = gtopoly(temp, -1);
    param->modulus = modulus;
    
    do{
        temp1 = cgetg(itos(n) + 1, t_VEC);
        temp = cgetg(itos(n) + 1, t_VEC);
        temp1 = randomElement(itos(n));
        temp = randomElement(itos(n));
        fdash = gtopolyrev(temp1,-1);
        g = gtopolyrev(temp, -1);
        fdash = gmodulo(fdash, q);
        fdash = gmodulo(fdash, modulus);
        g = gmodulo(g, q);
        g = gmodulo(g, modulus);
        f = gmul(t, fdash);
        f = gadd(f, one);
        finv = ginvmod(f, modulus);
    }
    while (lg(lift(gtovecrev(lift(finv))))-1 != itos(n)); // Loop until an invertible f is found.
    
    h = gmul(t, g);
    h = gmul(h, finv);
    
    keys->pk = h;
    keys->sk = f;
    keys->finv = finv;
    return keys;
    
}

// Function to encrypt an integer vector in form of an element of the Ring R_q^n
GEN encrypt(GEN input, params* param, keyset* key){
    if(lg(input)-1 >= itos(param->n)){
        std::cout<<"The input vector cannot be greater than n "<<lg(input)-1<<" "<<itos(param->n)<<endl;
        return stoi(0);
    }
    GEN n = param->n;
    GEN q = param->q;
    GEN t = param->t;
    GEN modulus = param->modulus;
    GEN h = key->pk;
    
    GEN two = stoi(2);
    GEN temp, m, s, e, c, qbyt;
    
    qbyt = gfloor(gdiv(q, t));
    temp = cgetg(itos(n) + 1, t_VEC);
    temp = encodeinputvector(n, t, input);
    m = gtopolyrev(temp, -1);
    m = gmodulo(m, q);
    m = gmodulo(m, modulus);
    
    e = Sample(itos(n), 2);
    e = gtopolyrev(e, -1);
    s = Sample(itos(n), 2);
    s = gtopolyrev(s, -1);
    e = gmodulo(e, q);
    e = gmodulo(e, modulus);
    s = gmodulo(s, q);
    s = gmodulo(s, modulus);
    
    temp = gmul(h, s);
    c = gadd(e, temp);
    
    m = gtovecrev(lift(lift(m)));
    for(int i=1; i<=lg(m)-1; i++){
        gel(m, i) = gmul(qbyt, gel(m, i));
    }
    m = gtopolyrev(m, -1);
    m = gmodulo(m, q);
    m = gmodulo(m, modulus);
    c = gadd(c, m);
    
    // Mapping the ciphertext to the symmetric interval of (-q/2, q/2]
    
    c = gtovecrev(lift(lift(c)));
    for(int i=1; i<=itos(n); i++){
        if(gcmp(gmul(two,gel(c, i)),q)==1){
            gel(c, i) = gsub(gel(c,i), q);
        }
    }
    c = gtopolyrev(c, -1);
    c = gmodulo(c, modulus);
    
    return c;
    
}

// This decrypt function should be called when ciphertext is added homomorphically or it is a simple ciphertext
GEN decryptSimple(GEN input, params* param, keyset* key){
    
    GEN n = param->n;
    GEN q = param->q;
    GEN t = param->t;
    GEN modulus = param->modulus;
    GEN h = key->pk;
    GEN f = key->sk;
    GEN two = stoi(2);
    
    GEN temp, temp1;
    
    temp = lift(lift(gmul(f, input)));
    temp = gtovecrev(temp);
    for(int i=1; i<=itos(n); i++){
        while(gcmp(gmul(two,gel(temp, i)),q)==1){
            gel(temp, i) = gsub(gel(temp, i), q);
        }
    }
    
    for(int i=1; i<=itos(n); i++){
        gel(temp, i) = diviiround(gmul(gel(temp, i), t), q);
    }
    temp = lift(gmodulo(temp, t));
    temp1 = gtopolyrev(temp, -1);
    temp1 = gmodulo(temp1, modulus);
    temp = lift(temp1);
    temp = gtovecrev(temp);
    GEN result = stoi(0);
    for(int i=0; i<lg(temp)-1; i++){
        result = gadd(gel(temp, i+1), result);
    }
    
    cout<<"Simple decryption result "<<GENtostr(result)<<std::endl;
    
    return temp;
    
}

// This function should be used when the ciphertext has been operated using homomorphic multiplication.
GEN decryptOneMul(GEN input, params* param, keyset* key){
    
    GEN n = param->n;
    GEN q = param->q;
    GEN t = param->t;
    GEN modulus = param->modulus;
    GEN h = key->pk;
    GEN f = key->sk;
    GEN two = stoi(2);
    
    GEN temp, temp1, temp2, c;
    
    temp1 = gtovecrev(lift(input));
    for(int i=1; i<=itos(n); i++){
        gel(temp1, i) = diviiround(gmul(gel(temp1, i), t), q);
    }
    c = gtopolyrev(temp1, -1);
    c = gmodulo(c, modulus);
    
    temp2 = f;
    temp2 = gmul(f, temp2);
    temp2 = lift(lift(temp2));
    temp2 = gmodulo(temp2, modulus);
    temp = lift(gmul(temp2, c));
    temp = lift(gmodulo(temp, q));
    temp = gtovecrev(temp);
    
    for(int i=1; i<=itos(n); i++){
        while(gcmp(gmul(two,gel(temp, i)),q)==1){
            gel(temp, i) = gsub(gel(temp, i), q);
        }
    }
    
    for(int i=1; i<=itos(n); i++){
        gel(temp, i) = diviiround(gmul(gel(temp, i), t), q);
    }
    temp = lift(gmodulo(temp, t));
    temp1 = gtopolyrev(temp, -1);
    temp1 = gmodulo(temp1, modulus);
    temp = lift(temp1);
    temp = gtovecrev(temp);
    GEN result = stoi(0);
    for(int i=0; i<lg(temp)-1; i++){
        result = gadd(gel(temp, i+1), result);
    }
    
    cout<<"Decryption result "<<GENtostr(result)<<std::endl;
    
    return temp;
    
}

// This function is used to homomorphically add 2 ciphertexts.
GEN homAdd(GEN input1, GEN input2){
    return gadd(input1, input2);
}

// This function is used to homomorphically multiply 2 ciphertexts. Currently, this function can only support one depth multiplications.
GEN homMul(GEN input1, GEN input2){
    return gmul(input1, input2);
}
