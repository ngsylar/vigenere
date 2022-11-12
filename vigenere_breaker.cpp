#include <algorithm>
#include <vector>
#include <utility>
#include <map>

#include <iostream> // remover

#include "vigenere.cpp"

class VigenereBreaker {
    private:
    static const float englishLetterFrequencies[26];
    static const float portugueseLetterFrequencies[26];

    static std::string cipherText;
    static std::vector<std::pair<std::string, int>> trigrams;
    static std::vector<std::pair<int, int>> possibleKeySizes;

    static int keySizeMin, keySizeMax;

    // obtem os trigramas da mensagem cifrada
    static void findTrigrams () {
        std::string trigram;
        for (int i=0; i < cipherText.size()-3; i++) {
            trigram = cipherText.substr(i,3);
            for (int j=i+3; j < cipherText.size(); j++)
                if (cipherText.substr(j,3) == trigram)
                    trigrams.push_back(std::make_pair(trigram, j-i));
        }
    }

    // calcula os provaveis tamanhos de chave
    static void calculatePossibleKeySizes () {
        std::map<int, int> keySizesFrequencies;
        for (int i=keySizeMin; i<=keySizeMax; i++)
            keySizesFrequencies[i] = 0;
        
        for (int trigramId=0; trigramId < trigrams.size(); trigramId++)
            for (int i=1; i<=20; i++)
                if (trigrams[trigramId].second%i == 0)
                    keySizesFrequencies[i]++;
        
        for (std::map<int, int>::iterator it=keySizesFrequencies.begin(); it != keySizesFrequencies.end(); it++)
            possibleKeySizes.push_back(std::make_pair(it->first, it->second));
        std::sort(possibleKeySizes.begin(), possibleKeySizes.end(), [](std::pair<int, int>& a, std::pair<int, int>& b) {return a.second > b.second;});
    }

    public:
    static void breakCipher (std::string cipherText, int keySizeMin=1, int keySizeMax=20) {
        VigenereBreaker::cipherText = Vigenere::stripAscii(cipherText);
        VigenereBreaker::keySizeMin = keySizeMin;
        VigenereBreaker::keySizeMax = keySizeMax;

        findTrigrams();
        calculatePossibleKeySizes();
        // std::cout << VigenereBreaker::cipherText << "\n";
        for (std::vector<std::pair<int, int>>::iterator it=possibleKeySizes.begin(); it!=possibleKeySizes.end(); it++) std::cout << it->first << "\t" << it->second << "\n"; // remover
        
        possibleKeySizes.clear();
        trigrams.clear();
        VigenereBreaker::keySizeMax = 0;
        VigenereBreaker::keySizeMin = 0;
        VigenereBreaker::cipherText = "";
    }
};

const float VigenereBreaker::englishLetterFrequencies[26] = {8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074};
const float VigenereBreaker::portugueseLetterFrequencies[26] = {14.630, 1.040, 3.880, 4.990, 12.570, 1.020, 1.300, 1.280, 6.180, 0.400, 0.020, 2.780, 4.740, 5.050, 10.730, 2.520, 1.200, 6.530, 7.810, 4.340, 4.630, 1.670, 0.010, 0.210, 0.010, 0.470};

std::string VigenereBreaker::cipherText = "";
std::vector<std::pair<std::string, int>> VigenereBreaker::trigrams = {};
std::vector<std::pair<int, int>> VigenereBreaker::possibleKeySizes = {};
int VigenereBreaker::keySizeMin = 0;
int VigenereBreaker::keySizeMax = 0;