# Gambit ![Windows](https://github.com/Vandise/gambit/workflows/Windows/badge.svg) ![Ubuntu](https://github.com/Vandise/gambit/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Vandise/gambit/workflows/MacOS/badge.svg)

Gambit is a dynamically and statically typed, multi-paradigm, imperative (procedural) and functional programming language that compiles to JavaScript.

## Installation

Installation requires `git` and `autoconf` on linux distributions and OSX. Windows will require `MingW`. To generate the test suite, the `ruby` language will be required.

```
$ > git clone https://github.com/Vandise/gambit
$ > cd gambit/
$ > autoconf
$ > ./configure
$ > make
$ > sh scripts/generate_tests
$ > make tests
$ > ./bin/test_suite
``

## License

MIT License

Copyright (c) 2019 Benjamin J. Anderson

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.