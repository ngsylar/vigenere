#include <string>

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