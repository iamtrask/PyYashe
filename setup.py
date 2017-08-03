from distutils.core import setup, Extension


example_module = Extension('_example',
                           sources=['example.cxx','example_wrap.cxx'],
                        # include_dirs = ['/usr/local/opt/openblas/include/'],
                        # extra_compile_args = ["-std=c++11", "-mmacosx-version-min=10.7", "-pthread", "-O3", "-march=native", "-Wall", "-funroll-loops", "-Wno-unused-result","-I/usr/local/opt/openblas/include/"],
                        extra_compile_args = ["-std=c++11", "-mmacosx-version-min=10.7", "-pthread", "-O3", "-march=native", "-Wall", "-funroll-loops", "-Wno-unused-result", "-lpari"],
                           )

setup (name = 'example',
       version = '0.2',
       author      = "SWIG Docs",
       description = """Simple swig linguamind from docs""",
       ext_modules = [example_module],
       py_modules = ["example"],
       )
