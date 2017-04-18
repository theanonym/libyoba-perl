## Documentation
See [Doxygen page](https://theanonym.github.io/libyoba-perl/annotated.html)

## Build

### Linux
Depends: `cmake`, `libperl-dev`.

```bash
git clone https://github.com/theanonym/libyoba-perl
cd libyoba-perl/build
cmake ..
make
tests/tests
```

## Usage
Include `<yobaperl/perl.hpp>` to your project and link with `libyoba-perl.so`.

Also, embed perl requires extra compiler flags:
* Compiler: `perl -MExtUtils::Embed -e ccopts`
* Linker: `perl -MExtUtils::Embed -e ldopts`
