#include <iostream>

int main() {

    int count = 1;
    
    int result;
    
    float pi;
    
    std::string helloWorld;
    
    bool myBool;
    
    char bleh;
    
    double d;
    
    count = 10;
    
    pi = 3.140000;
    
    helloWorld = "Hello, World!";
    
    myBool = true;
    
    if ((count >= 10)) {
        count = (count + 1);
        bleh = 'S';
        std::cout << count << std::endl;
    }
    else {
        count = (count - 1);
        std::cout << count << std::endl;
    }
    
    result = 5;
    
    if ((result == 5)) {
        std::cout << "blah" << std::endl;
        std::cout << result << std::endl;
    }
    
    d = 0.500000;
    return 0;
}
