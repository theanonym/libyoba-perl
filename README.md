## Examples
```c++
// Interpreter instance
Perl perl;


// Scalar
Scalar scalar = perl.newScalar("string");
std::string value = scalar.toString();

Scalar integer = perl.newScalar(14);
IV value = integer.toIV(); // IV = long long


// Array
Array array = perl.newArray({ 1, 2, 3 });
array.unshift(perl.newScalar(0));
array.push(perl.newScalar(4));
Scalar first = array.shift(); // 0
Scalar last = array.pop(); // 4
Scalar second = array.get(1).makeCopy(); // 2
cout << array; // (1, 2, 3)

// Array iteration
for(Array::Iterator it = array.begin(); it != array.end(); ++it) {
   Scalar scalar = *it;
   cout << scalar << endl;
}

for(Scalar scalar : array)
   cout << scalar << endl;


// Hash
Hash hash = perl.newHash<IV>({ {"one", 1}, {"two", 2}, {"three", 3} });
Scalar one = hash.get("one");
hash.insert("four", perl.newScalar(4));

// Hash iteration
for(Hash::Iterator it = hash.begin(); it != hash.end(); ++it) {
   HashEntry entry = *it;
   std::string key = entry.getKey();
   Scalar scalar = entry.getValue();
}


// Eval
perl.eval(" print 'hello\n' ");
// Eval with return
Scalar result = perl.eval<Scalar>(" 2 + 2 ");
cout << result; // 4


// Call subroutine
perl.eval(" sub test { reverse @_ } ");
Array result = perl.call<Array, Array>("test", /* params */perl.newArray({ 1, 2 });
cout << result // (2, 1)


// Register c++ callback function
perl.registerScalarToScalar(
   "array_to_string",
   [&perl] (Scalar param) -> Scalar {
      Array array = param.unrefArray();
      return perl.newScalar(array.toString());
   });

// Perl
$array_ref = [1, 2, 3, 4];
$result_string = YobaPerl::call('array_to_string', $array_ref);
```

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
