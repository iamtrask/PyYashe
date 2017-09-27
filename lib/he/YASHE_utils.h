/* File : YASHE_utils.h */

// TODO : Replace the continuous gaussian dist function with the knuth yao implementation of discrete gauss sampler. Check if main YASHE paper suggests using discrete gaussian dist.

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define PARI_OLD_NAMES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pari/pari.h>
//#include <pari.h>
#include <time.h>
#include <string>
#include <sys/time.h>
#include <vector>

struct timeval tv;

pari_sp ltop, lbot;

void print(GEN x){
    printf("%s\n", GENtostr(x));
}

class pari_GEN{
public:
    GEN value;
    
    pari_GEN(){};
    
    pari_GEN(int x){
        value = stoi(x);
        return;
    }
    
    void initialize(GEN x){
        value = x;
        return;
    }
    
    pari_GEN operator+(const pari_GEN GEN_2){
        pari_GEN result;
        result.value = gadd(this->value, GEN_2.value);
        return result;
    }
    
    pari_GEN operator*(const pari_GEN GEN_2){
        pari_GEN result;
        result.value = gmul(this->value, GEN_2.value);
        return result;
    }
    
    pari_GEN operator/(const pari_GEN GEN_2){
        pari_GEN result;
        result.value = gdiv(this->value, GEN_2.value);
        return result;
    }
    
    pari_GEN operator-(const pari_GEN GEN_2){
        pari_GEN result;
        result.value = gsub(this->value, GEN_2.value);
        return result;
    }
    
    pari_GEN operator%(const pari_GEN GEN_2){
        pari_GEN result;
        result.value = gmodulo(this->value, GEN_2.value);
        return result;
    }
    
    bool operator==(const pari_GEN GEN_2){
        if(!gequal(this->value, GEN_2.value))
            return false;
        else
            return true;
    }
};

struct parameters{
    int n, Q, T, w;
    double sigma_error, sigma_key;
    pari_GEN q, t, F;
};

double Uniform(void) {
    return ((double) rand() + 1.0) / ((double) RAND_MAX + 2.0);
}

double Normal(void) {
    return sqrt(-log(Uniform())*2.0) * sin(2.0 * M_PI * Uniform());
}

double Gauss(double mu, double sigma) {
    double z = sqrt(-2.0 * log(Uniform())) * sin(2.0 * M_PI * Uniform());
    return mu + sigma*z;
}

pari_GEN Sample(int n, double sigma) {
    pari_GEN ret;
    ret.value = cgetg(n + 1, t_VEC);
    double z;
    int i;
    
    for (i = 1; i <= n; i++) {
        z = Gauss(0, sigma);
        z = fabs(round(z)); /*absolute value of Gaussian distribution */
        ret.value[i] = (long) stoi((long) z);
    }
    
    return ret;
}

pari_GEN generate_random(int bit_length){
    gettimeofday(&tv, NULL);
    setrand(stoi(tv.tv_usec + tv.tv_sec*1000000));
    pari_GEN r;
    r.value = randomi(gshift(gen_1, bit_length));
    return r;
}

pari_GEN sample_error_polynomial(parameters* params, bool error){
    ltop = avma;
    pari_GEN tmp;
    if(error)
        tmp = Sample(params->n, params->sigma_error);
    else
        tmp = Sample(params->n, params->sigma_key);
    tmp.value = gmodulo(tmp.value, params->q.value);
    tmp.value = gtopolyrev(tmp.value, -1);
    tmp.value = gmodulo(tmp.value, params->F.value);
    lbot = avma;
    return tmp;
}

pari_GEN custom_mod(pari_GEN x, pari_GEN q){
    pari_GEN two(2);
    pari_GEN q_half, y = x;
    q_half.value = gfloor((q / two).value);
    y = y % q;
    y.value = lift(gtovecrev(lift(y.value)));
    int n = lg(y.value) - 1;
    for(int i = 0; i < n; i++)
        if(gcmp(gel(y.value, i + 1), q_half.value) > 0)
            gel(y.value, i + 1) = gsub(gel(y.value, i + 1), q.value);
    y.value = gtopolyrev(y.value, -1);
    return y;
}

GEN custom_mod(GEN x, GEN q){
    GEN two = stoi(2);
    GEN q_half, y = x;
    q_half = gfloor(gdiv(q, two));
    y = gmodulo(y, q);
    y = lift(gtovecrev(lift(y)));
    int n = lg(y) - 1;
    for(int i = 0; i < n; i++)
        if(gcmp(gel(y, i + 1), q_half) > 0)
            gel(y, i + 1) = gsub(gel(y, i + 1), q);
    y = gtopolyrev(y, -1);
    return y;
}

pari_GEN* generate_secret_key(parameters* params){
    pari_GEN* f_and_f_inv = new pari_GEN(2);
    pari_GEN tmp;
    GEN f_inv;
    int flag = 0;
    do{
        tmp = sample_error_polynomial(params, false);
        tmp.value = gmul(tmp.value, params->t.value);
        tmp = pari_GEN(1) + tmp;
        pari_CATCH(e_INV){
            //pari_CATCH_reset();
            flag = 1;
        }
        pari_TRY{
            f_inv = ginvmod(tmp.value, params->F.value);
        }
        pari_ENDCATCH
    }while(flag);
    f_and_f_inv[0] = tmp;
    f_and_f_inv[1].value = f_inv;
    return f_and_f_inv;
}

pari_GEN generate_public_key(pari_GEN f_inv, parameters* params){
    pari_GEN pk;
    pari_GEN g;
    g = sample_error_polynomial(params, false);
    pk = g * params->t * f_inv;
    return pk;
}

pari_GEN addition(pari_GEN ct_1, pari_GEN ct_2, parameters* params){
    pari_GEN ct;
    ct = ct_1 + ct_2;
    return ct;
}

pari_GEN subtraction(pari_GEN ct_1, pari_GEN ct_2, parameters* params){
    pari_GEN ct;
    ct = ct_1 - ct_2;
    return ct;
}

pari_GEN multiplication(pari_GEN ct_1, pari_GEN ct_2, parameters* params){
    pari_GEN ct_mult;
    pari_GEN c_1 = custom_mod(ct_1, params->q);
    pari_GEN c_2 = custom_mod(ct_2, params->q);
    ct_mult = c_1 * c_2;
    ct_mult = ct_mult % params->F;
    ct_mult.value = gtovecrev(lift(ct_mult.value));
    for(int i = 0; i < params->n; i++)
        gel(ct_mult.value, i + 1) = diviiround(gmul(params->t.value, gel(ct_mult.value, i + 1)), params->q.value);
    ct_mult.value = gtopolyrev(ct_mult.value, -1);
    ct_mult = custom_mod(ct_mult, params->q);
    ct_mult = ct_mult % params->F;
    return ct_mult;
}

GEN decimal_to_base_w(GEN x, int vector_size, int W){
    GEN vector_m = cgetg(vector_size + 1, t_COL);
    GEN w = stoi(W);
    GEN w_by_2 = gdiv(w, gen_2);
    if(gcmp(x, gen_0) == 0){
        for(int i = 0; i < vector_size; i++)
            gel(vector_m, i + 1) = gen_0;
        return vector_m;
    }
    std::vector<int> digits;
    GEN m = Q_abs(x), temp;
    while(!gequal(m, gen_0)){
        temp = lift(gmod(m, w));
        //if(gcmp(temp, w_by_2) > 0)
        //    temp = gsub(temp, w);
        digits.push_back(itos(temp));
        m = gfloor(gdiv(m, w));
    }
    int i = 1;
    std::vector<int>::iterator it;
    for(it = digits.begin(); it != digits.end(); it++)
        gel(vector_m, i++) = stoi(*it);
    digits.clear();
    for(i; i < vector_size + 1; i++)
        gel(vector_m, i) = gen_0;
    if(signe(x) == -1)
        vector_m = gneg(vector_m);
    return vector_m;
}

GEN base_w_decomposition(GEN x, int log_q, int n, int w){
    GEN u = cgetg(log_q + 1, t_COL);
    GEN temp = cgetg(n + 1, t_VEC);
    GEN vec = gtovecrev(lift(lift(x)));
    int k = lg(vec) - 1;
    for(int i = 0; i < k; i++)
        gel(temp, i + 1) = gel(vec, i + 1);
    for(int i = k; i < n; i++)
        gel(temp, i + 1) = gen_0;
    GEN tmp = cgetg(n + 1, t_MAT);
    for(int i = 0; i < n; i++){
        //print(gel(temp, i + 1));
        gel(tmp, i + 1) = cgetg(log_q + 1, t_COL);
        gel(tmp, i + 1) = decimal_to_base_w(gel(temp, i + 1), log_q, w);
    }
    tmp = shallowtrans(tmp);
    for(int i = 0; i < log_q; i++)
        gel(u, i + 1) = gtopolyrev(gel(tmp, i + 1), -1);
    return u;
}

GEN powers_of_w(GEN x, GEN q, int log_q, int n, int W){
    GEN u = cgetg(log_q + 1, t_COL);
    GEN w = stoi(W);
    GEN tmp = gen_1;
    for(int i = 0; i < log_q; i++){
        //gel(u, i + 1) = custom_mod(gmul(tmp, x), q);
        gel(u, i + 1) = gmodulo(gmul(tmp, x), q);
        tmp = gmul(tmp, w);
    }
    return u;
}
