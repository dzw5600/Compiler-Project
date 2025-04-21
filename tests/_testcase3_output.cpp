#include <iostream>

int main() {

    int i = 0;
    
    while ((i < 5)) {
        switch (i) {
          case 0:
            std::cout << '0' << std::endl;
            break;
          case 1:
            std::cout << "one" << std::endl;
            break;
          default:
            break;
        }
        i = (i + 1);
    }
    
    std::cout << "All done!" << std::endl;
    return 0;
}
