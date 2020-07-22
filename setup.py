'''
MIT License
Copyright (c) 2020 Jimeno Fonseca
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''

import os

from setuptools import setup, Extension

with open("README.md", "r") as fh:
    long_description = fh.read()

here = os.path.abspath(os.path.dirname(__file__))
_version = {}
_version_path = os.path.join(here, 'model', '__init__.py')
with open(_version_path, 'r') as f:
    exec(f.read(), _version)

install_requires = ['serial',
                    'matplotlib',
                    'dash',
                    'dash-daq',
                    'dash-core-components',
                    'dash-html-components',
                    'plotly',
                    'pybluez',
                    ]

setup(
    name='wow-sg',
    version=_version['__version__'],
    packages=['wow-sg'],
    url='https://github.com/jimenofonseca/wow-sg',
    license='MIT',
    author='Jimeno Fonseca',
    author_email='fonseca.jimeno@gmail.com',
    description="wow-sg: Weather station on wheels for beginners",
    long_description=long_description,
    long_description_content_type='text/markdown',
    python_requires='>=3.6',
    install_requires=install_requires,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent"]
)
