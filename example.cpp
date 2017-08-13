#include "exampleHeaders.h"

int test(){
    pari_init(2000000000,2);
    GEN n, q, t;
    printf("Parameters n, q, and t are already initialized.\n");
    n = stoi(8192);
    GEN temp;
    temp = powii(stoi(2), stoi(180));
    q = nextprime(temp);
    t = stoi(1000000);
    
    params* par = new params;
    par->n = n;
    par->q = q;
    par->t = t;
    GEN inp1vec, inp2vec;
    inp1vec = cgetg(2, t_VEC);
    inp2vec = cgetg(2, t_VEC);
    
    pari_printf("(n, q, t) = (%s, %s, %s)\n", GENtostr(n), GENtostr(q), GENtostr(t));
    printf("Enter the input integers:\n");
    int input1, input2;
    scanf("%d %d", &input1, &input2);
    printf("Input integers: %d %d\n", input1, input2);
    gel(inp1vec, 1) = stoi(input1);
    gel(inp2vec, 1) = stoi(input2);
    
    printf("Enter the operation to perform (--|1 for Addition and 2 for Multiplication|--)\n");
    int flag;
    scanf("%d", &flag);
    if(flag-1){
        keyset* key = keygen(par);
        GEN c1 = encrypt(inp1vec, par, key);
        GEN c2 = encrypt(inp2vec, par, key);
        GEN c3 = homMul(c1, c2);
        decryptOneMul(c3, par, key);
    }
    else{
        keyset* key = keygen(par);
        GEN c1 = encrypt(inp1vec, par, key);
        GEN c2 = encrypt(inp2vec, par, key);
        GEN c3 = homAdd(c1, c2);
        decryptSimple(c3, par, key);
    }
    printf("Cleaning up the Pari stack. Ending program.\n");
    pari_close();
    return 1;
}
