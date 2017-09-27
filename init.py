import YASHE
from YASHE import *

keys = key_gen()
keys = keys.generate_key(1024, 64, 256, 2**30, 8, 0.2)
ct = ciphertext(19, keys.pk)

