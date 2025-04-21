#include <iostream>

int main() {

    int count = 0;
    
    while ((count < 26)) {
        if ((((count % 3) == 0) && ((count % 5) == 0))) {
            std::cout << "FizzBuzz" << std::endl;
        }
        else         if (((count % 3) == 0)) {
            std::cout << "Fizz" << std::endl;
        }
        else         if (((count % 5) == 0)) {
            std::cout << "Buzz" << std::endl;
        }
        else {
            std::cout << count << std::endl;
        }
        count = (count + 1);
    }
    return 0;
}
