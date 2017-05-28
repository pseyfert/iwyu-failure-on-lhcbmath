# where things go wrong
IWYU suggests:

```
/tmp/pseyfert/tmp.b0YL1Sk5C8/build > include-what-you-use -pedantic -std=c++14 -I../Kernel/LHCbMath  -o Kernel/LHCbMath/CMakeFiles/LHCbMathLib.dir/src/Choose.cpp.o -c ../Kernel/LHCbMath/src/Choose.cpp -I/afs/cern.ch/sw/lcg/external/llvm/3.9/x86_64-centos7/lib/clang/3.9.0/include/

../Kernel/LHCbMath/src/Choose.cpp should add these lines:

../Kernel/LHCbMath/src/Choose.cpp should remove these lines:
- #include "LHCbMath/Choose.h"  // lines 14-14

The full include-list for ../Kernel/LHCbMath/src/Choose.cpp:
#include <algorithm>            // for min
#include <cmath>                // for lgamma, log, log2, M_E, exp
#include <limits>               // for numeric_limits
#include "LHCbMath/LHCbMath.h"  // for Zero
#include "LHCbMath/Power.h"     // for pow
---

```

implementing this change leads to a compiler error:

```
/tmp/pseyfert/tmp.b0YL1Sk5C8/build > clang++ -pedantic -std=c++14 -I../Kernel/LHCbMath  -o Kernel/LHCbMath/CMakeFiles/LHCbMathLib.dir/src/Choose.cpp.o -c ../Kernel/LHCbMath/src/Choose.cpp -I/afs/cern.ch/sw/lcg/external/llvm/3.9/x86_64-centos7/lib/clang/3.9.0/include/
../Kernel/LHCbMath/src/Choose.cpp:72:20: error: use of undeclared identifier 'Math'
unsigned long long Math::choose(const unsigned short n,
                   ^
../Kernel/LHCbMath/src/Choose.cpp:82:8: error: use of undeclared identifier 'Math'
double Math::choose_double(const unsigned short n, const unsigned short k) {
       ^
../Kernel/LHCbMath/src/Choose.cpp:121:8: error: use of undeclared identifier 'Math'
double Math::gen_choose(const double a, const unsigned short k) {
       ^
../Kernel/LHCbMath/src/Choose.cpp:148:8: error: use of undeclared identifier 'Math'
double Math::choose_half(const int n, const unsigned short k) {
       ^
../Kernel/LHCbMath/src/Choose.cpp:176:8: error: use of undeclared identifier 'Math'
double Math::log_choose(const unsigned short n, const unsigned short k) {
       ^
5 errors generated.
```

this error makes sense. Choose.h declares (abbreviated):
```
namespace Math {
unsigned long long choose(const unsigned short n, const unsigned short k);
}
```

and Choose.cpp implements that:

```
#include "LHCbMath/Choose.h"
// something here
unsigned long long Math::choose(const unsigned short n,
                                const unsigned short k) {
  return _choose_(n, k);
}
```


# sad workaround

when removing the namespace declaration from Choose.h, iwyu still suggests removing the header, though it doesn't lead to a compiler error anymore like this:

this error makes sense. Choose.h declares (abbreviated):
```
unsigned long long choose(const unsigned short n, const unsigned short k);
```

and Choose.cpp implements that:

```
// no include here (forward declaration of `choose` not needed for compilation)
// something here
unsigned long long choose(const unsigned short n,
                          const unsigned short k) {
  return _choose_(n, k);
}
```
