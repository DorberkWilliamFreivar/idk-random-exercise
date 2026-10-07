#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

class RAIL_FENCE{
    public:
        int depth_keyy;
        std::string* plainteks_ptr;
        std::vector<std::string> rows;
        std::string encryptedtext;
        int diff_space = 1;

        RAIL_FENCE(std::string* plainteks, int depth_key){
            plainteks_ptr = plainteks;
            depth_keyy = depth_key;
        }

        void makeDepths(){
            if (depth_keyy <= 1){
                rows.push_back(*plainteks_ptr);
                return;
            }

            std::vector<std::string> fences(depth_keyy, "");
            int cur_row = 0;
            bool go_down = false;
            for(char chr: (*plainteks_ptr)){
                fences[cur_row] += chr;
                if(cur_row == 0 || cur_row == depth_keyy - 1){
                    go_down = !go_down;
                }
                if(go_down == true){
                    cur_row += 1;
                } else {cur_row += -1;}
            }
            rows = fences;
        }

        std::string printRows(){
            int len_rows = rows.size();
            std::string full_text;
            for(int i=0;i<len_rows;i++){
                full_text += rows[i];
                std::cout << rows[i] << std::endl;
            } return full_text;
        }

        std::string process(){
            makeDepths();
            encryptedtext = printRows();

            return encryptedtext;
        }
};

void HelloWorld(std::string stringtext){
    std::cout << stringtext << std::endl;
}
    
int main(){
    int depth_key = 3;
    std::string plaintext = "integritasdata";
    RAIL_FENCE RF(&plaintext, depth_key);
    std::string encryptedtext = RF.process();

    HelloWorld("Hasil dari rail fence: ");
    HelloWorld(encryptedtext);
    std::cin.get();
}

// g++ -o exes\RF rail_fence.cpp 