#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl
#else
#  define PRINT(x) {}
#endif