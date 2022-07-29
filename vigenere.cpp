#include <iostream>
#include <fstream>

class Vigenere {
    public:
    static std::string stripAscii (std::string text) {
        std::string bannedChars = std::string(" \n,.:;!?()-\'\"").append(1,char(-61));
        std::string signedChars[6] = {"ÀÁÂÃàáâã","Çç","ÉÊéê","Íí","ÓÔÕóôõ","Úú"};
        std::string notSignedChars = "ACEIOU";
        
        // retira espacos, pontuacao e quebra de linha
        size_t found = text.find_first_of(bannedChars);
        while (found != std::string::npos) {
            text.erase(found,1);
            found = text.find_first_of(bannedChars, found);
        }
        // retira acentuacao
        for (int i=0; i<6; i++) {
            found = text.find_first_of(signedChars[i]);
            while (found != std::string::npos) {
                text[found] = notSignedChars[i];
                found = text.find_first_of(signedChars[i], found+1);
            }
        }
        // uppercase
        for (int i=0; i<text.size(); i++) {
            text[i] = toupper(text[i]);
        }
        return text;
    }

    // cifra uma mensagem com base em uma chave
    static std::string cipher (std::string originalMessage, std::string originalKey) {
        std::string message, key, cipherText;
        key = stripAscii(originalKey);
        message = stripAscii(originalMessage);

        int i=0, j=0; char cipherChar;
        while (i < message.size()) {
            cipherChar = ((message[i]+key[j%key.size()]-130)%26)+65;
            cipherText.append(1, cipherChar); j++; i++;
        }
        return cipherText;
    }

    // decifra uma mensagem cifrada atraves de uma chave
    static std::string decipher (std::string cipherText, std::string originalKey) {
        std::string key, message;
        key = stripAscii(originalKey);

        int i=0, j=0; char decipherChar;
        while (i < cipherText.size()) {
            decipherChar = ((cipherText[i]-key[j%key.size()]+26)%26)+97;
            message.append(1, decipherChar); j++; i++;
        }
        return message;
    }
};

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
    bool isRunning = true;

    std::cout << "Vigenere Cipher" << std::endl;
    while (isRunning) {
        std::cout << "\n1. Encrypt a file\n2. Write a message and encrypt\n3. Decrypt a file\n4. Write a cipher text and decrypt\n5. Quit\nChoose an option: ";
        std::cin >> option;
        while ((option.size()!=1)||(option[0]<49)||(option[0]>53)) {
            std::cout << "Invalid option, choose again: ";
            std::cin >> option;
        }
        if (option[0] == 53) {isRunning = false; break;}

        std::cout << "Provide a key for the cipher: ";
        std::cin >> key;
        switch (int(option[0])) {
            case 49:
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                product = Vigenere::cipher(text, key);
                break;

            case 50:
                std::cout << "Write a message: ";
                std::cin >> text;
                product = Vigenere::cipher(text, key);
                break;

            case 51:
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                product = Vigenere::decipher(text, key);
                break;

            case 52:
                std::cout << "Write the cipher text: ";
                std::cin >> text;
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
