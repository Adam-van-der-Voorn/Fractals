#ifdef MY_DEBUG
#  include <iostream>
#  define PRINTLN(x) std::cout << x << std::endl
#  define PRINT(x) std::cout << x
#  define DEBUG true
#else
#  define PRINTLN(x) {}
#  define PRINT(x) {}
#  define DEBUG false
#endif