#include <iostream>
#include <vector>
#include <string>
#include <cctype>

int luhnAlgorithm(std::vector<int> digitVector) {
    // Initialisation
    int finalSum;
    finalSum = 0;

    // Even index will be multiplied with 2, odd index will be multiplied with 1
    for (size_t i = 0; i < digitVector.size(); i++) { // 16
        if (i == 0 || (i % 2) == 0) {
            digitVector[i] = digitVector[i] * 2;
        } else { // (i % 2) != 0
            continue;
        }
    }

    // Display the multiple digits stored in the vector (Debug method)
    std::cout << "Multiple: " << std::endl;
    for (size_t i = 0; i < digitVector.size(); i++) { // 16
        std::cout << " " << digitVector[i] << " ";
    }
    std::cout << std::endl;

    // If digit is greater or equal to 10, both individual bit will be added together
    for (size_t i = 0; i < digitVector.size(); i++) { // 16
        if (digitVector[i] >= 10) {
            digitVector[i] = digitVector[i] - 9;
        } else {
            continue;
        }
    }

    // Display the sum digits stored in the vector (Debug method)
    std::cout << "Sum: " << std::endl;
    for (size_t i = 0; i < digitVector.size(); i++) { // 16
        std::cout << " " << digitVector[i] << " ";
    }
    std::cout << std::endl;

    // Sum all digit
    for (auto i : digitVector) {
        finalSum += i; // finalSum = finalSum + i
    }

    // Display the final sum digit (Debug method)
    std::cout << "Final sum: " << std::endl;
    std::cout << finalSum << std::endl;

    return (finalSum % 10);
}

int main() {
    std::string input; // Input
    std::vector<int> digitVector; // A vector which is used to store input
    int modSum; // Verification digit

    // Prompt the user for a 16-digit number
    std::cout << "Enter a 16-digit number which each digit ranges from 0 to 9: ";
    std::cin >> input; // User's input

    if (input.length() != 16) { // If input's length is smaller than 16 or more than 16, invalid
        std::cout << "Invalid input. Please enter a 16-digit number which each digit ranges from 0 to 9." << std::endl;
        return 1;
    }

    for (char c : input) { // If input string is not number, invalid
        if (!std::isdigit(c)) {
            std::cout << "Invalid input. Please enter a 16-digit number which each digit ranges from 0 to 9." << std::endl;
            return 1;
        } else { // std::isdigit(c)
            continue;
        }
    }

    // Convert each character to an integer and store in the vector
    for (size_t i = 0; i < input.length(); i++) { // 16
        int digit = input[i] - '0'; // Each character digit has unique ASCII value, subtracting '0' will result in the integer
        digitVector.push_back(digit);
    }

    // Display the digits stored in the vector (Debug method)
    std::cout << "Input: " << std::endl;
    for (size_t i = 0; i < digitVector.size(); i++) { // 16
        std::cout << " " << digitVector[i] << " ";
    }
    std::cout << std::endl;

    // Apply Luhn algorithm to validate 16-digit number
    modSum = luhnAlgorithm(digitVector);

    // Validate modSum
    if (modSum == 0) {
        std::cout << "Valid 16-digit number. Verified by Luhn Algorithm." << std::endl;
    } else { // modSum != 0
        std::cout << "Invalid 16-digit number. Verified by Luhn Algorithm." << std::endl;
    }

    return 0;
}
