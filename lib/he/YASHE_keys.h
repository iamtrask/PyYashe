#include "YASHE_utils.h"

class secret_key{
private:
    pari_GEN sk;
    
public:
    parameters* params;
    
    secret_key(){};
    
    secret_key(pari_GEN sk, parameters* params){
        this->sk = sk;
        this->params = params;
    }
    
    void initialize(pari_GEN sk, parameters* params){
        this->sk = sk;
        this->params = params;
    }
    
    pari_GEN decrypt(pari_GEN ct){
        pari_GEN m;
        pari_GEN temp = sk * ct;
        temp = custom_mod(temp, params->q);
        temp = temp * params->t;
        temp.value = gtovecrev(temp.value);
        for(int i = 0; i < params->n; i++)
            gel(temp.value, i + 1) = diviiround(gel(temp.value, i + 1), params->q.value);
        m = temp % params->t;
        m.value = lift(m.value);
        return m;
    }
    
    void serialize(){
        return;
    }
    
    ~secret_key(){};
};

class public_key{
public:
    pari_GEN pk;
    pari_GEN evk;
    parameters* params;
    
    public_key(){};
    
    public_key(pari_GEN pk, pari_GEN evk, parameters* params){
        this->pk = pk;
        this->evk = evk;
        this->params = params;
    }
    
    void initialize(pari_GEN pk, pari_GEN evk, parameters* params){
        this->pk = pk;
        this->evk = evk;
        this->params = params;
    }
    
    pari_GEN encrypt(pari_GEN m){
        pari_GEN ct;
        pari_GEN temp = m % params->t;
        temp.value = gtopolyrev(temp.value, -1);
        temp.value = lift(temp.value);
        pari_GEN q_by_t;
        q_by_t.value = gfloor((params->q/params->t).value);
        temp = q_by_t * temp;
        pari_GEN e = sample_error_polynomial(params, true);
        pari_GEN s = sample_error_polynomial(params, true);
        ct = temp + e + (pk * s);
        return ct;
    }
    
    pari_GEN relinearize(pari_GEN ct_mult){
        pari_GEN ct, temp;
        int log_q = params->Q/(int) log2(params->w) + 1;
        temp.value = base_w_decomposition(ct_mult.value, log_q, params->n, params->w);
        temp = temp % params->F;
        ct.value = lift(gmul(shallowtrans(temp.value), evk.value));
        ct = custom_mod(ct, params->q);
        return ct;
    }
    
    void serialize(){
        
        return;
    }
    
    ~public_key(){};
};

struct key_pair{
    secret_key sk;
    public_key pk;
};

class key_gen{
public:
    key_gen(){};
    
    key_pair generate_key(int n, int Q, int T, int w, double sigma_error, double sigma_key){
        key_pair keys;
        parameters* params = new parameters;
        params->t.value = stoi(T);
        params->sigma_key = sigma_key;
        params->sigma_error = sigma_error;
        params->n = n;
        params->Q = Q;
        params->w = w;
        GEN v = cgetg(n + 2, t_VEC);
        gel(v, 1) = stoi(1);
        for (int i = 2; i <= n; i++)
            gel(v, i) = gzero;
        gel(v, n + 1) = stoi(1);
        params->F.value = gtopolyrev(v, -1);
        params->q.value = gshift(stoi(1), params->Q);
        params->q.value = gnextprime(params->q.value);
        
        pari_GEN* f_and_f_inv = generate_secret_key(params);
        keys.sk.initialize(f_and_f_inv[0], params);
        pari_GEN h = generate_public_key(f_and_f_inv[1], params);
        pari_GEN gamma, e, s;
        int log_q = Q/(int)log2(w) + 1;
        e.value = cgetg(log_q + 1, t_COL);
        s.value = cgetg(log_q + 1, t_COL);
        gamma.value = powers_of_w(f_and_f_inv[0].value, params->q.value, log_q, n, w);
        for(int i = 0; i < log_q; i++){
            gel(e.value, i + 1) = sample_error_polynomial(params, true).value;
            gel(s.value, i + 1) = sample_error_polynomial(params, true).value;
        }
        gamma = gamma + e + (h*s);
        gamma = gamma % params->F;
        for(int i = 0; i < log_q; i++)
            gel(gamma.value, i + 1) = custom_mod(gel(gamma.value, i + 1), params->q.value);
        keys.pk.initialize(h, gamma, params);
        return keys;
    }
    
    /*key_pair deserialize(char* public_key, char* secret_key){
     key_pair keys;
     return keys;
     }*/
};

