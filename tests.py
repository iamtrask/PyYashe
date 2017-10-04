import unittest
import YASHE

class base_test(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(base_test, self).__init__(*args, **kwargs)
        keys = YASHE.key_gen()
        self.keys = keys.generate_key(3329, 128, 256, 2**30, 8, 0.4)

class correctness_test(base_test):
    def __init__(self, *args, **kwargs):
        super(correctness_test, self).__init__(*args, **kwargs)
    
    def test_single_data(self):
        ct = YASHE.ciphertext(5, self.keys.pk)
        pt = YASHE.pari_GEN(5)
        self.assertEqual(ct.decrypt(self.keys.sk)[0], pt)

    def test_multiple_data(self):
        ct = YASHE.ciphertext([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], self.keys.pk)
        pt = YASHE.pari_GEN([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
        self.assertEqual(ct.decrypt(self.keys.sk).sub_array(0, 10), pt)

class addition_test(base_test):
    def __init__(self, *args, **kwargs):
        super(addition_test, self).__init__(*args, **kwargs)

    def test_single_data(self):
        ct_1 = YASHE.ciphertext(10, self.keys.pk)
        ct_2 = YASHE.ciphertext(5, self.keys.pk)
        pt = YASHE.pari_GEN(15)
        ct = ct_1 + ct_2
        self.assertEqual(ct.decrypt(self.keys.sk)[0], pt)

    def test_multiple_data(self):
        ct_1 = YASHE.ciphertext([1, 2, 3, 4, 5], self.keys.pk)
        ct_2 = YASHE.ciphertext([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], self.keys.pk)
        pt = YASHE.pari_GEN([2, 4, 6, 8, 10, 6, 7, 8, 9, 10])
        ct = ct_1 + ct_2
        self.assertEqual(ct.decrypt(self.keys.sk).sub_array(0, 10), pt)

class subtraction_test(base_test):
    def __init__(self, *args, **kwargs):
        super(subtraction_test, self).__init__(*args, **kwargs)
    
    def test_single_data(self):
        ct_1 = YASHE.ciphertext(10, self.keys.pk)
        ct_2 = YASHE.ciphertext(5, self.keys.pk)
        pt = YASHE.pari_GEN(5)
        ct = ct_1 - ct_2
        self.assertEqual(ct.decrypt(self.keys.sk)[0], pt)
    
    def test_multiple_data(self):
        ct_1 = YASHE.ciphertext([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], self.keys.pk)
        ct_2 = YASHE.ciphertext([1, 2, 3, 4, 5], self.keys.pk)
        pt = YASHE.pari_GEN([0, 0, 0, 0, 0, 6, 7, 8, 9, 10])
        ct = ct_1 - ct_2
        self.assertEqual(ct.decrypt(self.keys.sk).sub_array(0, 10), pt)

class multiplication_test(base_test):
    def __init__(self, *args, **kwargs):
        super(multiplication_test, self).__init__(*args, **kwargs)
    
    def test_single_data(self):
        ct_1 = YASHE.ciphertext(10, self.keys.pk)
        ct_2 = YASHE.ciphertext(5, self.keys.pk)
        pt = YASHE.pari_GEN(50)
        ct = ct_1 * ct_2
        self.assertEqual(ct.decrypt(self.keys.sk)[0], pt)
    
    def test_multiple_data(self):
        ct_1 = YASHE.ciphertext([1, 2, 3, 4, 5], self.keys.pk)
        ct_2 = YASHE.ciphertext([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], self.keys.pk)
        pt = YASHE.pari_GEN([1, 4, 10, 20, 35, 50, 65, 80, 95, 110, 114, 106, 85, 50])
        ct = ct_1 * ct_2
        self.assertEqual(ct.decrypt(self.keys.sk).sub_array(0, 14), pt)

    def test_multiple_multiplications(self):
        ct_1 = YASHE.ciphertext([1, 2, 3, 4, 5], self.keys.pk)
        ct_2 = YASHE.ciphertext([1, 2, 3, 4, 5, 6, 7, 8, 9, 10], self.keys.pk)
        pt = YASHE.pari_GEN([1, 8, 36, 120, 74, 12, 94, 8, 235, 232, 202, 24, 205, 244, 38, 240, 91, 152, 210, 40, 50, 144, 144, 236, 161, 52, 196])
        ct = ct_1 * ct_2
        ct = ct * ct
        self.assertEqual(ct.decrypt(self.keys.sk).sub_array(0, 27), pt)

class miscellaneous(base_test):
    def __init__(self, *args, **kwargs):
        super(miscellaneous, self).__init__(*args, **kwargs)

    def test_ciphertext_array(self):
        ciphertext_array = []
        for i in range(1, 10):
            ciphertext_array.append(YASHE.ciphertext(i, self.keys.pk))
        pt = YASHE.pari_GEN(5)
        self.assertEqual(ciphertext_array[4].decrypt(self.keys.sk)[0], pt)

    def test_nested_operations(self):
        ct_1 = YASHE.ciphertext(5, self.keys.pk)
        ct_2 = YASHE.ciphertext(10, self.keys.pk)
        pt = YASHE.pari_GEN(238)
        ct = (ct_1 * ct_2) * (ct_1 + ct_2)
        self.assertEqual(ct.decrypt(self.keys.sk)[0], pt)

if __name__ == '__main__':
    unittest.main()
