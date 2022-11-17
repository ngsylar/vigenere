#include <string>
#include <vector>
#include <utility>

#define VSTRIP_DEFAULT          0b0000
#define VSTRIP_SWAP_ACCENTED    0b0001
#define VSTRIP_CONSIDER_ALL     0b0010  // editar: implementar essa tecnica - ignorar caracteres nao alfabeticos mas nao ignorar a classe da chave em que se encontram

class Vigenere {
    private:
    static const std::string alphabeticChars;
    static const std::string signedChars[6];
    static const std::string notSignedChars;

    static std::vector<std::pair<int, char>> nonAlphabeticText;

    static std::string reverseStrip (std::string text) {
        for (int i = nonAlphabeticText.size()-1; i>=0; i--)
            text = text.substr(0, nonAlphabeticText[i].first) + std::string(1, nonAlphabeticText[i].second) + text.substr(nonAlphabeticText[i].first);
        return text;
    }

    static void stripDefault (std::string& text, size_t& found) {
        // salva caracteres nao alfabeticos e letras acentuadas
        found = text.find_first_not_of(alphabeticChars);
        while (found != std::string::npos) {
            nonAlphabeticText.push_back(std::make_pair(found, text[found]));
            text.erase(found,1);
            found = text.find_first_not_of(alphabeticChars, found);
        }
    }

    static void stripSwapAccented (std::string& text, size_t& found) {
        // troca letras acentuadas por nao acentuadas
        for (int i=0; i<6; i++) {
            found = text.find_first_of(signedChars[i]);
            while (found != std::string::npos) {
                text[found] = notSignedChars[i];
                found = text.find_first_of(signedChars[i], found+1);
            }
        }
        // salva caracteres nao alfabeticos
        found = text.find_first_not_of(alphabeticChars);
        while (found != std::string::npos) {
            if (text[found] != -61)
                nonAlphabeticText.push_back(std::make_pair(found, text[found]));
            text.erase(found,1);
            found = text.find_first_not_of(alphabeticChars, found);
        }
    }

    public:
    static std::string stripAscii (std::string text, int flags=VSTRIP_DEFAULT) {
        size_t found;
        nonAlphabeticText.clear();
        
        if (flags == 0) stripDefault(text, found);
        else if (flags & 1) stripSwapAccented(text, found);

        // transforma letras em maiusculas
        for (int i=0; i<text.size(); i++)
            text[i] = toupper(text[i]);
            
        // for(int i=0;i<=nonAlphabeticText.size();i++) std::cout << nonAlphabeticText[i].first << "\t" << nonAlphabeticText[i].second;
        return text;
    }

    // cifra uma mensagem com base em uma chave
    static std::string cipher (std::string originalMessage, std::string originalKey, int flags=VSTRIP_DEFAULT) {
        std::string message, key, cipherText;
        key = stripAscii(originalKey);
        message = stripAscii(originalMessage, flags);

        int i=0, j=0; char cipherChar;
        while (i < message.size()) {
            cipherChar = ((message[i]+key[j%key.size()]-130)%26)+65;
            cipherText.append(1, cipherChar); j++; i++;
        }
        return reverseStrip(cipherText);
    }

    // decifra uma mensagem cifrada atraves de uma chave
    static std::string decipher (std::string cipherText, std::string originalKey, int flags=VSTRIP_DEFAULT) {
        std::string key, message;
        key = stripAscii(originalKey);
        cipherText = stripAscii(cipherText, flags);

        int i=0, j=0; char decipherChar;
        while (i < cipherText.size()) {
            decipherChar = ((cipherText[i]-key[j%key.size()]+26)%26)+97;
            message.append(1, decipherChar); j++; i++;
        }
        return reverseStrip(message);
    }
};

const std::string Vigenere::alphabeticChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::string Vigenere::signedChars[6] = {std::string(1,-128)+std::string(1,-127)+std::string(1,-126)+std::string(1,-125)+std::string(1,-96)+std::string(1,-95)+std::string(1,-94)+std::string(1,-93), std::string(1,-121)+std::string(1,-89), std::string(1,-119)+std::string(1,-118)+std::string(1,-87)+std::string(1,-86), std::string(1,-115)+std::string(1,-83), std::string(1,-109)+std::string(1,-108)+std::string(1,-107)+std::string(1,-77)+std::string(1,-76)+std::string(1,-75), std::string(1,-102)+std::string(1,-70)};
const std::string Vigenere::notSignedChars = "ACEIOU";

std::vector<std::pair<int, char>> Vigenere::nonAlphabeticText;