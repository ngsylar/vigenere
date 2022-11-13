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
    static void computePossibleKeySizes () {
        std::map<int, int> keySizesWeights;
        for (int i=keySizeMin; i<=keySizeMax; i++)
            keySizesWeights[i] = 0;
        
        // calcula peso de cada tamanho de chave no intervalo keySizeMin:keySizeMax
        for (int trigramId=0; trigramId < trigrams.size(); trigramId++)
            for (int i=keySizeMin; i<=keySizeMax; i++)
                if (trigrams[trigramId].second%i == 0)
                    keySizesWeights[i]++;
        
        for ( // it->first é um possível tamanho de chave e it->second seu peso
            std::map<int, int>::iterator it=keySizesWeights.begin();
            it != keySizesWeights.end(); it++
        ) possibleKeySizes.push_back(std::make_pair(it->first, it->second));
        std::sort(
            possibleKeySizes.begin(), possibleKeySizes.end(),
            [](std::pair<int, int>& a, std::pair<int, int>& b)
            { return a.second > b.second; }
        );
    }

    // 
    static void computeLetterFrequencies () {
        int likelyKeySize = possibleKeySizes.front().first;
        std::string mostLikelyKey = "";
        for (int k=0; k < likelyKeySize; k++) {
            std::vector<float> cipherLetterFrequencies(26, 0.0f);
            int keyLetterGroupSize = 0;
            
            // calcula ocorrencia de cada letra no grupo do digito k da chave
            for (int i=0; i < cipherText.size(); i++)
                if (i%likelyKeySize == k) {
                    cipherLetterFrequencies[cipherText[i]-65]++;
                    keyLetterGroupSize++;
                }
            // normaliza o arranjo de frequencias resultante
            for (int i=0; i < 26; i++) {
                cipherLetterFrequencies[i] = cipherLetterFrequencies[i] / keyLetterGroupSize;
                cipherLetterFrequencies[i] *= 100.0f;
            }
            // calcula o caractere mais provavel para o digito k da chave
            float minChiDistance = 999999.0f;
            int mostLikelyCharacter = -1;
            for (int shift=0; shift < 26; shift++) {
                float chiDistance = 0.0f;

                // calcula distancia qui-quadrado do histograma de frequências
                for (int i=0; i < 26; i++) {
                    float chisub = englishLetterFrequencies[i] - cipherLetterFrequencies[i];
                    float chisum = englishLetterFrequencies[i] + cipherLetterFrequencies[i];
                    chiDistance += (chisub * chisub) / chisum;
                }
                // salva a menor distancia obtida para cada shift
                chiDistance *= 0.5f;
                if (chiDistance < minChiDistance) {
                    minChiDistance = chiDistance;
                    mostLikelyCharacter = shift;
                }
                // faz um shift a esquerda do histograma
                cipherLetterFrequencies.push_back(cipherLetterFrequencies.front());
                cipherLetterFrequencies.erase(cipherLetterFrequencies.begin());
                std::cout << shift << "\t" << chiDistance << "\n"; // remover
            }
            mostLikelyCharacter += 65;
            mostLikelyKey += (char)mostLikelyCharacter;
            // for (int i=0; i<26; i++) std::cout << i << " " << ctLetterFrequencies[i] << "\n"; return; // remover
        }
        std::cout << mostLikelyKey; // remover
    }

    public:
    static void breakCipher (std::string cipherText, int keySizeMin=1, int keySizeMax=20) {
        VigenereBreaker::cipherText = Vigenere::stripAscii(cipherText);
        VigenereBreaker::keySizeMin = keySizeMin;
        VigenereBreaker::keySizeMax = keySizeMax;

        findTrigrams();
        computePossibleKeySizes();
        computeLetterFrequencies();
        // std::cout << VigenereBreaker::cipherText << "\n"; // remover
        // for (std::vector<std::pair<int, int>>::iterator it=possibleKeySizes.begin(); it!=possibleKeySizes.end(); it++) std::cout << it->first << "\t" << it->second << "\n"; // remover
        
        // limpa todas as variaveis da classe
        possibleKeySizes.clear();
        trigrams.clear();
        VigenereBreaker::keySizeMax = 0;
        VigenereBreaker::keySizeMin = 0;
        VigenereBreaker::cipherText = "";
    }
};

const float VigenereBreaker::englishLetterFrequencies[26] = {8.167f, 1.492f, 2.782f, 4.253f, 12.702f, 2.228f, 2.015f, 6.094f, 6.966f, 0.153f, 0.772f, 4.025f, 2.406f, 6.749f, 7.507f, 1.929f, 0.095f, 5.987f, 6.327f, 9.056f, 2.758f, 0.978f, 2.360f, 0.150f, 1.974f, 0.074f};
const float VigenereBreaker::portugueseLetterFrequencies[26] = {14.630f, 1.040f, 3.880f, 4.990f, 12.570f, 1.020f, 1.300f, 1.280f, 6.180f, 0.400f, 0.020f, 2.780f, 4.740f, 5.050f, 10.730f, 2.520f, 1.200f, 6.530f, 7.810f, 4.340f, 4.630f, 1.670f, 0.010f, 0.210f, 0.010f, 0.470f};

std::string VigenereBreaker::cipherText = "";
std::vector<std::pair<std::string, int>> VigenereBreaker::trigrams = {};
std::vector<std::pair<int, int>> VigenereBreaker::possibleKeySizes = {};

int VigenereBreaker::keySizeMin = 0;
int VigenereBreaker::keySizeMax = 0;