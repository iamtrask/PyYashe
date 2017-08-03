rm -rf build
swig -c++ -python -py3 example.i
python setup.py install
