#include "YASHE_keys.h"

class ciphertext{
public:
    pari_GEN value;
    public_key* pk;
    
    ciphertext(){};
    
    ~ciphertext(){};
    
    ciphertext(public_key* pk){
        //this->degree = 0;
        this->pk = pk;
    }
    
    ciphertext(int m, public_key* pk){
        pari_GEN pt(m);
        this->pk = pk;
        this->value = pk->encrypt(pt);
    }
    
    void initialize(int m, public_key* pk){
        pari_GEN pt(m);
        this->pk = pk;
        this->value = pk->encrypt(pt);
    }
    
    void initialize(public_key* pk){
        this->pk = pk;
    }
    
    void packing_method(pari_GEN m, public_key* pk){
        this->pk = pk;
        this->value = pk->encrypt(m);
    }
    
    ciphertext operator+(ciphertext &ct){
        ciphertext result;
        result.value = addition(this->value, ct.value, pk->params);
        result.pk = this->pk;
        return result;
    }
    
    ciphertext operator*(ciphertext &ct){
        ciphertext result;
        result.value = multiplication(this->value, ct.value, pk->params);
        result.value = pk->relinearize(result.value);
        result.pk = this->pk;
        return result;
    }
    
    ciphertext operator-(ciphertext &ct){
        ciphertext result;
        result.value = subtraction(this->value, ct.value, pk->params);
        result.pk = this->pk;
        return result;
    }
    
    pari_GEN decrypt(secret_key sk){
        pari_GEN m = sk.decrypt(this->value);
        return m;
    }
};
