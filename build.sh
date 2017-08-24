rm -rf build
#If you get C/C++ headers missing errors, then uncomment the following 2 lines.
#export CPLUS_INCLUDE_PATH=/System/Library/Frameworks/Python.framework/Headers
#export C_INCLUDE_PATH=/System/Library/Frameworks/Python.framework/Headers
swig -c++ -python example.i
python setup.py install
