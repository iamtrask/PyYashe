# PyYASHE
An Experimental Library for Developing a High Performance YASHE Implementation

The scheme was first proposed by Bos et al. in - Improved Security for a Ring-Based Fully Homomorphic Encryption Scheme (https://eprint.iacr.org/2013/075.pdf). This paper by Microsoft Research also uses this scheme to implement secure prediction over pre-trained-encrypted neural networks - CryptoNets: Applying Neural Networks to Encrypted Data with High Throughput and Accuracy (https://www.microsoft.com/en-us/research/wp-content/uploads/2016/04/CryptonetsTechReport.pdf).

This implementation only implements the YASHE scheme presently. YASHE' implementation will be added soon.

## Running

1. Install [Docker](https://www.docker.com/)

   * If you're on macOS and use [Homebrew](https://brew.sh), run `brew cask install docker; open -a docker`

2. Run these commands:

``` shell
make build
make run
```
