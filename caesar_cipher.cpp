#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class CAESAR_CIPHER{
    public:
        int keyy;
        std::string* plaintext_ptr;
        std::vector<char> abjads = {'A', 'B', 'C', 'D', 'E', 'F', 
            'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
        std::vector<char> abjad_after_key;
        std::unordered_map<char, int> abjd_keyidx;
        std::string uppertext;
        std::string encryptedtext;

        CAESAR_CIPHER(std::string* plaintext, int key){
            plaintext_ptr = plaintext;
            abjad_after_key.resize(26);
            keyy = key;
        }

        void toUpper(){
            for(char letter: (*plaintext_ptr)){
                letter = (char)toupper(letter);
                uppertext += letter;
            }
        }
        
        void swicth(){
            int len_abjd = abjads.size();
            for(int i=0; i<len_abjd;i++){
                int idx_key = (i+keyy) % len_abjd;
                char curr_abjd = abjads[i];
                abjad_after_key[i] = abjads[idx_key];
                abjd_keyidx[curr_abjd] = i;
            }
        }
        
        void match(){
            for(char letter: (uppertext)){
                std::string chiper_char;
                if(isalpha(letter)){
                    int idx_key = abjd_keyidx[letter];
                    chiper_char = abjad_after_key[idx_key];
                } else {chiper_char = " ";}
                encryptedtext += chiper_char;
            }
        }

        void printClean(){
            std::cout << "Abjad setelah menggunakan Key " << keyy << ":" << std::endl;
            for(char x: abjad_after_key){
                std::cout << x;
            } std::cout << std::endl;
            std::cout << " "<< std::endl;
        }
        std::string process(){
            toUpper();
            swicth();
            match();
            printClean();
            return encryptedtext;
        }
    };

void HelloWorld(std::string stringtext){
    std::cout << stringtext << std::endl;
}
    
int main(){
    int key_switch = 7;
    std::string plaintext = "indonesia emas duaribuduapuluhempat";
    CAESAR_CIPHER CC(&plaintext, key_switch);
    std::string encryptedtext = CC.process();

    HelloWorld("Hasil dari caesar cipher: ");
    HelloWorld(encryptedtext);
    std::cin.get();
}

// g++ -o exes\CC caesar_cipher.cpp 