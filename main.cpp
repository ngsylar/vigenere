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
    std::ofstream FileWithProduct;
    std::string option, fileName, key, text, product, save, productName;
    int language;
    bool isRunning = true;

    std::cout << "Vigenere Cipher" << std::endl;
    while (isRunning) {
        std::cout << "\n1. Encrypt a file\n2. Decrypt a file\n3. Break a file\'s encryption\n4. Quit\nChoose an option: ";
        std::cin >> option;
        while ((option.size()!=1)||(option[0]<49)||(option[0]>52)) {
            std::cout << "Invalid option, choose again: ";
            std::cin >> option;
        } if (option[0] == 52) {
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
                std::cout << "Enter the key to decrypt: ";
                std::cin >> key;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                else product = Vigenere::decipher(text, key);
                break;
            
            case 51:
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                else {
                    std::cout << "\n1. English\n2. Portuguese\nChoose a language: ";
                    std::cin >> language;
                    product = VigenereBreaker::breakCipher(text, language-1, 5);
                    VigenereBreaker::clearVariables();
                } break;

            default: break;
        }

        std::cout << std::endl << product << std::endl;
        std::cout << std::endl << "Do you want to save the obtained text in a new file (y/n)? ";
        std::cin >> save;
        if ((save.size()>0)&&((save[0]==89)||(save[0]==121))) {
            std::cout << "Save file as: ";
            std::cin >> productName;
            FileWithProduct = std::ofstream(productName);
            FileWithProduct.write(product.c_str(), product.size());
            FileWithProduct.close();
            std::cout << "The file has been saved.";
        }
    }
}