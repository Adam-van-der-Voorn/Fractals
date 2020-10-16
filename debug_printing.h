#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl
#  define DEBUG true
#else
#  define PRINT(x) {}
#  define DEBUG false
#endif