#include <iostream>
#include <vector>
#include <string>

class PLAYFAIR_CIPHER{
    public:
        std::string* plaintext_ptr;
        std::string* keyword_ptr;
        std::vector<std::vector<char>> matrix_cipher_5x5;
        std::vector<char> abjads = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
        std::string uppertext;
        std::string lowertext;
        
        std::vector<char> vector_non_word;
        std::vector<char> vector_word;
        std::string encryptedtext;

        PLAYFAIR_CIPHER(std::string* plaintext, std::string* keyword){
            plaintext_ptr = plaintext;
            keyword_ptr = keyword;

            std::vector<char> char_temp;
            char_temp.resize(5, 'X');
            for(int i=0;i<5;i++){
                matrix_cipher_5x5.push_back(char_temp);
            }
        }

        void toUpper(std::string* text){
            uppertext = "";
            for(char letter: (*text)){
                if(letter == 'J'){letter = 'I';}
                letter = (char)toupper(letter);
                uppertext += letter;
            }
        }
        
        void getSetWord(){
            std::string temp_char = ""; 
            for(int i=0; i<uppertext.size();i++){
                char curr_char = uppertext[i];
                if(temp_char.find(curr_char) == std::string::npos){
                    vector_word.push_back(curr_char);
                    temp_char += curr_char; 
                }
            }
        }
        
        void getSetNonWord(){
            for(char chr: abjads){
                if(uppertext.find(chr) == std::string::npos){
                    vector_non_word.push_back(chr);
                }
            }
        } 
        
        char getSingleChar(int idx, std::string state){
            char single_char;
            if(state == "word"){
                single_char = vector_word[idx];
            } else{
                single_char = vector_non_word[idx];
            }
            return single_char;
        }

        void inputTextToMatrix(){
            int x = 0; int z = 0;
            for(int i=0; i<matrix_cipher_5x5[0].size(); i++){
                for(int j=0; j<matrix_cipher_5x5[0].size(); j++){
                    if (x<vector_word.size()){
                        matrix_cipher_5x5[i][j] = getSingleChar(x, "word");
                        x++;
                    } else if (z<vector_non_word.size()){
                        matrix_cipher_5x5[i][j] = getSingleChar(z, "abjd");
                        z++;
                    }
                }
            }
        }
        
        void printMatrix(){
            for(int i=0; i<matrix_cipher_5x5[0].size();i++){
                for(int j=0; j<matrix_cipher_5x5[0].size();j++){
                    std::cout << matrix_cipher_5x5[i][j];
                } std::cout << std::endl;
            }
        }
                
        void toLower(std::string* text){
            for(char letter: (*text)){
                letter = (char)tolower(letter);
                lowertext += letter;
            }
        }

        void preprocessText(){
            int len_text = uppertext.size();
            for(int i=0;i<len_text;i+=2){
                char c1 = uppertext[i];
                char c2 = uppertext[i+1];
                if(c1 == c2){
                    // uppertext.insert(i+1, 1, 'X');
                    uppertext += uppertext[len_text-1];
                    len_text++;
                    
                    char c3 = uppertext[i+2];
                    uppertext[i+1] = 'X';
                    uppertext[i+2] = c2;
                    char temp_before = c3;
                    for(int j=i+3;j<len_text;j++){
                        char temp_curr = uppertext[j];
                        uppertext[j] = temp_before;
                        temp_before = temp_curr;
                    }
                } 
            }

            if(len_text % 2 == 1){
                uppertext += 'X';
                len_text += 1;
            } std::cout << uppertext << std::endl;
        }

        std::string checkText(){
            int len_text = uppertext.size();

            std::string enc_temp;
            for(int i=0;i<len_text;i+=2){
                char p1 = uppertext[i];
                char p2 = uppertext[i+1];
                std::vector<std::vector<int>> locs = processToMatrix(p1, p2);
                std::string new_ps = setLocs(locs);
                enc_temp += new_ps;
            }
            return enc_temp;
        }
        
        std::string setLocs(std::vector<std::vector<int>> locs){
            std::string new_ps = "";
            char new_p1 = matrix_cipher_5x5[locs[0][0]][locs[0][1]];
            char new_p2 = matrix_cipher_5x5[locs[1][0]][locs[1][1]];
            new_ps += new_p1;
            new_ps += new_p2;
            return new_ps;
        }

        std::vector<std::vector<int>> processToMatrix(char p1, char p2){
            int len_mtrx = matrix_cipher_5x5.size();
            int curr_row_p1; int curr_row_p2;
            int curr_col_p1; int curr_col_p2;
    
            for(int i=0;i<len_mtrx;i++){
                for(int j=0;j<len_mtrx;j++){
                    if(p1 == matrix_cipher_5x5[i][j]){
                        curr_row_p1 = i; curr_col_p1 = j;
                    }
                    if(p2 == matrix_cipher_5x5[i][j]){
                        curr_row_p2 = i; curr_col_p2 = j;
                    } 
                }
            }
            std::vector<std::vector<int>> locs = setRules(curr_row_p1, curr_row_p2, curr_col_p1, curr_col_p2);
            return locs;
        }

        std::vector<std::vector<int>> setRules(int currowp1, int currowp2, int curcolp1, int curcolp2){
            if (currowp1 == currowp2) {curcolp1 += 1; curcolp2 += 1;}
            else if (curcolp1 == curcolp2) {currowp1 += 1; currowp2 += 1;}
            else {
                int temp_idx_col = curcolp1;
                curcolp1 = curcolp2; 
                curcolp2 = temp_idx_col;
            }
            if (currowp1 >= matrix_cipher_5x5[0].size()){currowp1 = 0;}
            if (currowp2 >= matrix_cipher_5x5[0].size()){currowp2 = 0;}
            if (curcolp1 >= matrix_cipher_5x5[0].size()){curcolp1 = 0;}
            if (curcolp2 >= matrix_cipher_5x5[0].size()){curcolp2 = 0;}
          
            std::vector<std::vector<int>> locs = {{currowp1, curcolp1}, {currowp2, curcolp2}};
            return locs;
        }
        
        std::string process(){
            toUpper(keyword_ptr);
            getSetWord();
            getSetNonWord();
            inputTextToMatrix();
            printMatrix();
            toUpper(plaintext_ptr);
            preprocessText();

            encryptedtext = checkText();
            return encryptedtext;
        }
};

void HelloWorld(std::string stringtext){
    std::cout << stringtext << std::endl;
}
    
void NewLine(){
    std::cout << std::endl;
}

int main(){
    std::string plaintext = "kriptografi";
    std::string keywords = "sekuriti";
    PLAYFAIR_CIPHER PC(&plaintext, &keywords);
    std::string encryptedtext = PC.process();

    NewLine();
    HelloWorld("Key: "+keywords);
    HelloWorld("plaintext: "+plaintext);
    NewLine();
    HelloWorld("Hasil dari playfair cipher: "+encryptedtext);
    std::cin.get();
}

// g++ -o exes\PC playfair_cipher.cpp 

// Huruf kembar dalam satu pasangan dipisah dengan huruf pengisi, misalnya x.
// Satu baris: ganti dengan huruf di kanannya. Contoh ar menjadi RM.
// Satu kolom: ganti dengan huruf di bawahnya. Contoh mumenjadi CM.
// Selain itu: ambil huruf pada baris sendiri dan kolom pasangannya. Contoh hs menjadi BP.