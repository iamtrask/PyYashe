# PyYASHE [![Build Status](https://travis-ci.org/OpenMined/PyYashe.svg?branch=master)](https://travis-ci.org/OpenMined/PyYashe)

❗
This library has been deprecated due to changes in strategy and roadmap. To actively contribute based on our current roadmap, checkout [OpenMined](https://github.com/OpenMined/OpenMined), [PySyft](https://github.com/OpenMined/PySyft), or [join our slack](https://openmined.slack.com/messages/team_pysyft)
---

An Experimental Library for Developing a High Performance YASHE' Implementation

The scheme was first proposed by Bos et al. in - Improved Security for a Ring-Based Fully Homomorphic Encryption Scheme (https://eprint.iacr.org/2013/075.pdf). This paper by Microsoft Research also uses this scheme to implement secure prediction over pre-trained-encrypted neural networks - CryptoNets: Applying Neural Networks to Encrypted Data with High Throughput and Accuracy (https://www.microsoft.com/en-us/research/wp-content/uploads/2016/04/CryptonetsTechReport.pdf).

This is a Levelled Fully Homomorphic implementation of the YASHE' scheme.

## Running

1. Install [Docker](https://www.docker.com/)

   * If you're on macOS and use [Homebrew](https://brew.sh), run `brew cask install docker; open -a docker`

2. Run these commands:

``` shell
make build
make run
```

## Development

1. Install [Docker](https://www.docker.com/)

    * If you're on macOS and use [Homebrew](https://brew.sh), run `brew cask install docker; open -a docker`
   
2. Run `make build`

3. Edit source code as you wish

4. Run `make dev`

## API

### Functions:

#### pari_init(pari_size, max_prime)  

   `pari_init()` is the function that needs to be called before dealing with this API. On importing the module, the module executes this statement automatically. `pari_size` defines the size of stack we'll be using, and `max_prime` defines the pre-computed prime table. By default, `pari_size` is set to 2000000000, and `max_prime` is set to 2.

   _Arguments_: `pari_size (int)`, `max_prime (int)`

#### pari_close()  

   `pari_close()` function has to be called at the end of each program to clear the memory used.


### Classes:

#### pari_GEN

  This class abstracts the `GEN` variable in C++, making it available through a Python interface. The class is compatible with +, \*, /, -, `__getitem__` , %, and `print`.
  * Class Data:
    1. value (`GEN`)

  * `__init__(self, x)`

    The constructor converts `x` to a `GEN` variable.

    _Arguments_: `x(int)`

#### parameters
  * Class Data:
    1. n, Q, w (int)
    2. sigma_error, sigma_key (double)
    3. q, t, F (pari_GEN)

#### secret_key
  * Class Data:
    1. sk (`pari_GEN`)
    2. params (`parameters`)

  * `__init__(self, sk = None, parmas = None)   `
    The constructor initiates class data.

    _Arguments_: `sk (pari_GEN)`, `params (parameters)`

  * `decrypt(self, ct)`   
    `decrypt()` method returns the plaintext (`pari_GEN`) encrypted in ciphertext ct.

    _Arguments_: `ct (pari_GEN)`

  * `serialize(self)`   
    TO BE IMPLEMENTED

#### public_key
  * Class Data:
    1. pk (`pari_GEN`)
    2. evk (`pari_GEN`)
    2. params (`parameters*`)

  * `__init__(self, pk = None, evk = None, params = None)`   
    The constructor initiates the class data.

    _Arguments_: `pk (pari_GEN)`, `evk (pari_GEN)`, `params (parameters*)`

  * `encrypt(self, pt)`   
    `encrypt()` method returns the ciphertext (`pari_GEN`) which encrypts plaintext pt.

    _Arguments_: `pt (pari_GEN)`
    
  * `relinearize(self, ct)`   
    `relinearize()` method returns a ciphertext of dimension 1, and which can be decrypted by secret_key sk. It is called on the resulting ciphertext after each multiplication.
    
    _Arguments_: `ct (pari_GEN)`

  * `serialize(self) `  
    TO BE IMPLEMENTED

#### key_pair
  * Class Data:
    1. sk (`secret_key`)
    2. pk (`public_key`)

#### key_gen
  * `generate_key(self, n, Q, T, sigma)`   

    `generate_key()` method returns the keys, which is of type `key_pair`.

    _Arguments_: `n (int)`, `Q (int)`, `T (int)`, `w (int)`, `sigma_error (double)`, `sigma_key (double)`


  * `deserialize(self)`   
    TO BE IMPLEMENTED

#### ciphertext   

  The class is compatible with '+', '\*', and '-' operators. It also supports batching.

  * Class Data:
    1. value (`pari_GEN`)
    2. pk (`public_key*`)

  * `__init__(self, plaintext = None, pk)`   

    The plaintext variable is either an `int` variable, or it is an `int_list` variable. The size of list has to be less than the degree of polynomials `(paramter n)` in the polynomial ring.

    _Arguments_: `plaintext (int or int_list), pk (public_key*)`

  * `decrypt(self, sk)`

    `decrypt()` method returns the decrypted ciphertext which is `pari_GEN` variable.

    _Arguments_: `sk (secret_key*)`

--------

### TODO
- [ ] Serialization and Deserialization support. <br/>
