#include <string>

class Vigenere {
    private:
    static const std::string alphabeticChars;
    static const std::string signedChars[6];
    static const std::string notSignedChars;

    public:
    static std::string stripAscii (std::string text) {
        
        // retira espacos, pontuacao e quebra de linha
        size_t found = text.find_first_not_of(alphabeticChars);
        while (found != std::string::npos) {
            text.erase(found,1);
            found = text.find_first_not_of(alphabeticChars, found);
        }
        // retira acentuacao
        for (int i=0; i<6; i++) {
            size_t found = text.find_first_of(signedChars[i]);
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
        cipherText = stripAscii(cipherText);

        int i=0, j=0; char decipherChar;
        while (i < cipherText.size()) {
            decipherChar = ((cipherText[i]-key[j%key.size()]+26)%26)+97;
            message.append(1, decipherChar); j++; i++;
        }
        return message;
    }
};

const std::string Vigenere::alphabeticChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const std::string Vigenere::signedChars[6] = {"ÀÁÂÃàáâã","Çç","ÉÊéê","Íí","ÓÔÕóôõ","Úú"};
const std::string Vigenere::notSignedChars = "ACEIOU";