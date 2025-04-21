#include <iostream>

int main() {

    int a = 5;
    
    int b;
    
    b = (a + 10);
    
    std::cout << a << std::endl;
    
    std::cout << b << std::endl;
    
    int c = 10;
    
    int mult = (c * b);
    
    std::cout << mult << std::endl;
    
    a = 4;
    
    int mod = (a % 4);
    
    std::cout << mod << std::endl;
    return 0;
}
