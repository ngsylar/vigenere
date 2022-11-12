// #include <iostream>
#include <fstream>

#include "vigenere_breaker.cpp"

// extrai texto de um arquivo
std::string extractFromFile (std::string fileName) {
    std::ifstream FileWithMessage(fileName);
    std::string fileLine, text;
    if (FileWithMessage.is_open()) {
        while (getline(FileWithMessage, fileLine))
            text.append(fileLine);
        FileWithMessage.close();
    }
    else text = fileName;
    return text;
}

// le um arquivo
bool fileWasNotRead (std::string fileName, std::string *text) {
    std::ifstream FileWithMessage(fileName);
    std::string fileLine;
    if (FileWithMessage.is_open()) {
        text->clear();
        while (getline(FileWithMessage, fileLine))
            text->append(fileLine);
        FileWithMessage.close();
        return false;
    }
    return true;
}

// ui
int main () {
    std::string desafio = "";

    VigenereBreaker::breakCipher(desafio);

    return 0;
    
    std::ofstream FileWithProduct;
    std::string option, fileName, key, text, product, save, productName;
    bool isRunning = true;

    std::cout << "Vigenere Cipher" << std::endl;
    while (isRunning) {
        std::cout << "\n1. Encrypt a file\n2. Write a message and encrypt\n3. Decrypt a file\n4. Write a cipher text and decrypt\n5. Quit\nChoose an option: ";
        std::cin >> option;
        while ((option.size()!=1)||(option[0]<49)||(option[0]>53)) {
            std::cout << "Invalid option, choose again: ";
            std::cin >> option;
        } if (option[0] == 53) {
            isRunning = false; break;
        }

        switch (int(option[0])) {
            case 49:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                else product = Vigenere::cipher(text, key);
                break;

            case 50:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Write a message: ";
                getline(std::cin, text);
                product = Vigenere::cipher(text, key);
                break;

            case 51:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                else product = Vigenere::decipher(text, key);
                break;

            case 52:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Write the cipher text: ";
                getline(std::cin, text);
                product = Vigenere::decipher(text, key);
                break;
            default: break;
        }

        std::cout << std::endl << product << std::endl;
        std::cout << std::endl << "Do you want to save the obtained text in a new file (y/n)? ";
        std::cin >> save;
        if ((save.size()>0)&&((save[0]==89)||(save[0]==121))) {
            std::cin >> productName;
            FileWithProduct = std::ofstream(productName);
            FileWithProduct.write(product.c_str(), product.size());
            FileWithProduct.close();
        }
    }
}