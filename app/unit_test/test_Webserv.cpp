#include <string>
#include <cassert>
#include <stdexcept>
#include "../includes/Webserv.hpp"


// หาวิธี Compile เอง
void test_findCharInString()
{
    // Test case 1: Valid input, character found in string
    std::string str1 = "Hello, World!";
    const char* c1 = "o";
    int result1 = findCharInString(str1, c1);
    assert(result1 == 1);

    // Test case 2: Valid input, character not found in string
    std::string str2 = "Hello, World!";
    const char* c2 = "zo";
    int result2 = findCharInString(str2, c2);
    assert(result2 == 1);

    // Test case 3: Empty string
    std::string str3 = "";
    const char* c3 = "a";
    int result3 = findCharInString(str3, c3);
    assert(result3 == 0);

    // Test case 4: Null pointer
    std::string str4 = "Hello, World!";
    const char* c4 = NULL;
    try {
        findCharInString(str4, c4);
        // If the function does not throw an exception, the test fails
        assert(false);
    } catch (const std::invalid_argument& e) {
        // Exception thrown, test passes
    }
	std::cout << "All test cases passed for findCharInString" << std::endl;
}
