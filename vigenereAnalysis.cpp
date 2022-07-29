#include <iostream>
#include <fstream>
#include <cmath>

#include <utility>
#include <iterator>
#include <algorithm>
#include <vector>

#define MAX_TRIGRAMS 100

bool cmp(std::pair<int,int> i, std::pair<int,int> j){
    if( i.second == j.second ) return i.first > j.first;
    return i.second > j.second;
}

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

    // analiza uma mensagem cifrada
    static std::string analyze (std::string cipherText, int cipherLanguage = 0) {
        std::string key, message;

        std::pair<std::string, int> trigrams[MAX_TRIGRAMS];
        int nTrigrams = 0;
        int i, j, k;

	    for(i = 0; i<cipherText.size()-3; i++){
		    std::string tri = cipherText.substr(i,3);
		    for(j = i+3; j<cipherText.size(); j++){
			    if(tri == cipherText.substr(j,3) ){
				    trigrams[nTrigrams] = make_pair(tri, j-i);
				    nTrigrams++;
			    }
		    }
	    }
        
        std::vector<std::pair<int, int>> factors = { {0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0},{8,0},{9,0},{10,0},{11,0},{12,0},{13,0},{14,0},{15,0},{16,0},{17,0},{18,0},{19,0},{20,0} };
        //std::cout << "Trigrams: " << nTrigrams << "\n";
        while(nTrigrams > 0){
            nTrigrams--;
            //std::cout << trigrams[nTrigrams].second << ": " << trigrams[nTrigrams].first << "\n";
            
            for(int i = 1; i <= 20; i++){
		        if( (trigrams[nTrigrams].second)%i == 0) factors[i].second++;
	        }
        }

        factors.erase(std::remove_if(factors.begin(), factors.end(), [&](const std::pair<int,int>& elem) { return elem.first < 3; }), factors.end());
        sort(factors.begin(), factors.end(), cmp);

        int likelyKeySize = factors.begin()->first;

        std::cout << "Most likely cipher key size: " << likelyKeySize << "\n";
        
        float ingl[26] = { 8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074 };
        float port[26] = {14.630, 1.040, 3.880, 4.990, 12.570, 1.020, 1.300, 1.280, 6.180, 0.400, 0.020, 2.780, 4.740, 5.050, 10.730, 2.520, 1.200, 6.530, 7.810, 4.340, 4.630, 1.670, 0.010, 0.210, 0.010, 0.470};

        float *abcFreq;
        if(cipherLanguage == 0) abcFreq = ingl;
        else if (cipherLanguage == 1) abcFreq = port;
        else abcFreq = ingl;

        std::string likelyCipherKey = "";
        for(i = 0; i<likelyKeySize; i++){
            float keyCharFrequencies[26] = {};
            float chiDistance = 9999999.0f;
            int mostLikelyCharacter = 0;

            for(j = 0; j < 26; j++){
                float freq[26] = {};
                float freqchars = 0.0f;
                float tempChiDistance = 0.0f;

                //get character frequencies for affected characters of the cipher
                for(k = 0; k < cipherText.size(); k++) { 
                    if(k%likelyKeySize == i){
                        freq[(cipherText[k]-j+26)%26]++;
                        freqchars++;
                    }
                }
                //normalize array
                for(k = 0; k < 26; k++) freq[k] = (100.0f*freq[k])/freqchars;

                //calculate Chi distance to chosen language letter frequency histogram ((Xi - Yi)^2)/(Xi + Yi)
                for(k = 0; k < 26; k++){
                    //dont have to check for division by zero since language frequency arrays dont have null elements
                    tempChiDistance += (abcFreq[k] - freq[k])*(abcFreq[k] - freq[k])/(abcFreq[k] + freq[k]);
                }

                if(tempChiDistance < chiDistance){
                    mostLikelyCharacter = j;
                    chiDistance = tempChiDistance;
                }
                /*std::cout << "F[ " << freq[0];
                for(int k = 1; k < 26; k++) std::cout << ", " << freq[k];
                std::cout << "]\n";*/
            }
            likelyCipherKey += (cipherText[i]-((cipherText[i]-mostLikelyCharacter)%26)-39)%26+65;
        }
        if (cipherLanguage == 1) std::cout << "Most likely key based on the portuguese alphabet: " << likelyCipherKey << "\n";
        else abcFreq = std::cout << "Most likely key based on the english alphabet: " << likelyCipherKey << "\n";
        std::cout << "Attempting to decrypt ciphertext with infered key:\n";

        i=0, j=0; char decipherChar;
        while (i < cipherText.size()) {
            decipherChar = ((cipherText[i]-likelyCipherKey[j%likelyCipherKey.size()]+26)%26)+97;
            message.append(1, decipherChar); j++; i++;
        }
        return message;
    }
};

// extrai texto de um arquivo
std::string extractFromFile (std::string fileName) {
    std::fstream FileWithMessage(fileName);
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
    std::fstream FileWithMessage(fileName);
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
int main (int argc, char *argv[]) {
    std::string option, fileName, key, text, product, save, productName;
    bool isRunning = true;
    int ctFileId=0, dtFileId=0, atFileId=0, language;

    std::cout << "Vigenere Cipher" << std::endl;
    if (argc >= 3) {
        key = argv[1];
        fileName = argv[2];
        text = extractFromFile(fileName);
        product = Vigenere::cipher(text, key);
        std::ofstream FileWithProduct("VigenereCipherText.txt");
        FileWithProduct.write(product.c_str(), product.size());
        FileWithProduct.close();
    } else while (isRunning) {
        std::cout << "\n1. Encrypt a file\n2. Write a message and encrypt\n3. Decrypt a file\n4. Write a cipher text and decrypt\n5. Analyze a cipher text\n6. Analyze a ciphered file\n7. Quit\nChoose an option: ";
        std::cin >> option;
        while ((option.size()!=1)||(option[0]<49)||(option[0]>55)) {
            std::cout << "Invalid option, choose again: ";
            std::cin >> option;
        }
        if (option[0] == 55) {isRunning = false; break;}
        switch (int(option[0])) {
            case 49:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                product = Vigenere::cipher(text, key);
                productName = "VigenereCipherText_";
                productName.append(1,char(ctFileId+48));
                break;
            case 50:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Write a message: ";
                //std::cin >> text;
                getline(std::cin, text); // ignora \n
                getline(std::cin, text); // pega a linha inteira
                product = Vigenere::cipher(text, key);
                productName = "VigenereCipherText_";
                productName.append(1,char(ctFileId+48));
                break;
            case 51:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;
                product = Vigenere::decipher(text, key);
                productName = "DecipheredText_";
                productName.append(1,char(dtFileId+48));
                break;
            case 52:
                std::cout << "Provide a key for the cipher: ";
                std::cin >> key;
                std::cout << "Write the cipher text: ";
                std::cin >> text;
                product = Vigenere::decipher(text, key);
                productName = "DecipheredText_";
                productName.append(1,char(dtFileId+48));
                break;
            case 53:
                std::cout << "Choose a language for the cipher:\n\t0. EN-US\n\t1. PT-BR\n";
                std::cin >> language;
                if(language == 0)
                    std::cout << "Write the cipher text (EN-US): ";
                else if (language == 1)
                    std::cout << "Write the cipher text (PT-BR): ";
                else
                    std::cout << "Unrecognized language, assuming EN-US.\n Write the cipher text (EN-US): ";
                std::cin >> text;

                if (language == 1) product = Vigenere::analyze(text, 1);
                else product = Vigenere::analyze(text);

                productName = "AnalyzedText_";
                productName.append(1,char(atFileId+48));
                break;
            case 54:
                std::cout << "Choose a language for the cipher:\n\t0. EN-US\n\t1. PT-BR\n";
                std::cin >> language;
                std::cout << "Enter file name: ";
                std::cin >> fileName;
                if (fileWasNotRead(fileName, &text))
                    std::cout << "Error: invalid file name." << std::endl;

                if (language == 1) product = Vigenere::analyze(text, 1);
                else product = Vigenere::analyze(text);

                productName = "AnalyzedText_";
                productName.append(1,char(atFileId+48));
                break;
            default: break;
        }
        std::cout << std::endl << product << std::endl;
        std::cout << std::endl << "Do you want to save the obtained text in a new file (y/n)? ";
        std::cin >> save;
        if ((save.size()>0)&&((save[0]==89)||(save[0]==121))) {
            std::ofstream FileWithProduct(productName+".txt");
            FileWithProduct.write(product.c_str(), product.size());
            FileWithProduct.close();
        }
    }
}