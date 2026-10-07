// Name: Abdullah Mubarok (009)
// Datum: --
// Klasse: 25G

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 
#include <cmath> 

class KNN_CLASSIFICATION{
    public:
        int n;
        std::vector<float> EUCLID_DIST;
        std::vector<float>* PTR_DATA_NEW;
        std::vector<std::string>* PTR_DATA_Y;
        std::vector<std::vector<float>>* PTR_DATA_X;
        std::vector<std::string> NEW_DATA_STR;
        KNN_CLASSIFICATION(std::vector<std::vector<float>>* DATA_X, std::vector<std::string>* DATA_Y, std::vector<float>* NEW_DATA, int total_n){
            n = total_n;
            PTR_DATA_X = DATA_X;
            PTR_DATA_Y = DATA_Y;
            PTR_DATA_NEW = NEW_DATA;
        }

        void euclideanDistance(){
            for(int i=0; i<PTR_DATA_X->size();i++){
                float row_vals = 0;
                for(int j=0; j<(*PTR_DATA_X)[i].size();j++){
                    float x_old = (*PTR_DATA_X)[i][j];
                    float x_new = (*PTR_DATA_NEW)[j];
                    float temp = x_new-x_old;
                    row_vals += temp*temp;
                }
                row_vals = std::sqrt(row_vals);
                EUCLID_DIST.push_back(row_vals);
            }
        }

        std::vector<int> sorting(){
            std::vector<int> indexs = {0,1,2,3,4,5};
            int len = EUCLID_DIST.size();
            for(int i=0; i<len;i++){
                for(int j=0; j<len-i-1;j++){
                    if(EUCLID_DIST[j+1] < EUCLID_DIST[j]){
                        float temp_float = EUCLID_DIST[j];
                        EUCLID_DIST[j] = EUCLID_DIST[j+1];
                        EUCLID_DIST[j+1] = temp_float;
                        
                        std::string temp_str = (*PTR_DATA_Y)[j];
                        (*PTR_DATA_Y)[j] = (*PTR_DATA_Y)[j+1];
                        (*PTR_DATA_Y)[j+1] = temp_str;

                        int temp_idx = indexs[j];
                        indexs[j] = indexs[j+1];
                        indexs[j+1] = temp_idx;
                    }
                }
            } return indexs;
        }

        std::vector<std::string> process(){
            euclideanDistance();
            std::vector<int> index_new = sorting();

            int acc_n = 0;
            int rej_n = 0;
            std::string NEW_Y;
            std::string NEW_X;
            std::string CLASS_ACC;
            std::string CLASS_REJ;
            
            for(int i=0; i<EUCLID_DIST.size();i++){
                std::cout << i+1 << ". Data from nearest (" << index_new[i] << "): " << EUCLID_DIST[i] << ". Class: " << (*PTR_DATA_Y)[i] << std::endl;
                
                if(i < n){
                    if ((*PTR_DATA_Y)[i] == "TERIMA" || (*PTR_DATA_Y)[i] == "BAIK"){
                        CLASS_ACC = (*PTR_DATA_Y)[i];
                        acc_n++;
                    } else { 
                        rej_n++; 
                        CLASS_REJ = (*PTR_DATA_Y)[i];
                    }
                }
            } 
            
            std::cout << " " << std::endl;  
            if (acc_n>rej_n){
                std::cout << "Accept that, the new data is on Class 1 (" << CLASS_ACC << ")" << std::endl;
                NEW_Y = CLASS_ACC+"\n";
            } else {
                std::cout << "Reject that, the new data is on Class 2 (" << CLASS_REJ << ")" << std::endl;
                NEW_Y = CLASS_REJ+"\n";
            } 
            
            // 1D lol, im to lazy to add output 2D
            for(int i=0; i<(*PTR_DATA_NEW).size(); i++){
                NEW_X += std::to_string((*PTR_DATA_NEW)[i]) + ',';
            }
            NEW_DATA_STR.push_back(NEW_X + NEW_Y);
            return NEW_DATA_STR;
        }
};

class KNN_REGRESI{
    public:
        std::vector<std::vector<float>>* PTR_X;
        std::vector<float>* PTR_Y;
        std::vector<float>* PTR_NEW_DATA;

        std::vector<std::vector<float>> X_norm;
        std::vector<float> X_test_norm;

        std::vector<float> beta = {0.0, 0.0, 0.0};
        std::vector<float> EUCLID_DIST;
        int N = 3;

        // DATA FOR PREAPAER
        float X1_MIN;
        float X1_MAX;
        float X2_MIN;
        float X2_MAX;

        std::vector<float> X1_cols;
        std::vector<float> X2_cols;

        KNN_REGRESI(std::vector<std::vector<float>>* DATA_X, std::vector<float>* DATA_Y, std::vector<float>* NEW_DATA){
            PTR_X = DATA_X;
            PTR_Y = DATA_Y;
            PTR_NEW_DATA = NEW_DATA;
        }

        std::vector<float> get_Xcols(int index){
            std::vector<float> COLS;
            int len = (*PTR_X).size();
            for(int i=0; i<len;i++){
                COLS.push_back((*PTR_X)[i][index]);
            }
            return COLS;
        }

        float min(std::vector<float>* X_cols){
            float MIN = (*X_cols)[0];
            int len = (*X_cols).size();
            for(int i=0;i<len;i++){
                if((*X_cols)[i] < MIN){
                    MIN = (*X_cols)[i];
                }
            }
            return MIN;
        }
        
        float max(std::vector<float>* X_cols){
            float MAX = (*X_cols)[0];
            int len = (*X_cols).size();
            for(int i=0;i<len;i++){
                if((*X_cols)[i] > MAX){
                    MAX = (*X_cols)[i];
                }
            }
            return MAX;
        }
        
        float normalize(float val, float min_val, float max_val){
            float norm = (val - min_val) / (max_val - min_val);
            return norm;
        }

        void prepare_data(){
            X1_cols = get_Xcols(0);
            X2_cols = get_Xcols(1);

            X1_MIN = min(&X1_cols);
            X1_MAX = max(&X1_cols);
            X2_MIN = min(&X2_cols);
            X2_MAX = max(&X2_cols);

            // normalize all X old data
            for(int i=0;i<(*PTR_X).size();i++){
                float x1_norm = (*PTR_X)[i][0];
                float x2_norm = (*PTR_X)[i][1];
                X_norm.push_back({1.0, x1_norm, x2_norm});
            }
            
            // normalize all X new data
            X_test_norm = {
                normalize((*PTR_NEW_DATA)[0], X1_MIN, X1_MAX), 
                normalize((*PTR_NEW_DATA)[1], X2_MIN, X2_MAX)
            };
        }

        void euclideanDistance(){
            for(int i=0; i<X_norm.size();i++){
                float row_vals = 0;
                for(int j=0; j<X_norm[i].size();j++){
                    float x_old = X_norm[i][j];
                    float x_new = X_test_norm[j];
                    float temp = x_new-x_old;
                    row_vals += temp*temp;
                }
                row_vals = std::sqrt(row_vals);
                EUCLID_DIST.push_back(row_vals);
            }
        }

         std::vector<int> sorting(){
            std::vector<int> indexs = {0,1,2,3,4,5};
            int len = EUCLID_DIST.size();
            for(int i=0; i<len;i++){
                for(int j=0; j<len-i-1;j++){
                    if(EUCLID_DIST[j+1] < EUCLID_DIST[j]){
                        float temp_dist = EUCLID_DIST[j];
                        EUCLID_DIST[j] = EUCLID_DIST[j+1];
                        EUCLID_DIST[j+1] = temp_dist;
                        
                        float temp_float = (*PTR_Y)[j];
                        (*PTR_Y)[j] = (*PTR_Y)[j+1];
                        (*PTR_Y)[j+1] = temp_float;

                        int temp_idx = indexs[j];
                        indexs[j] = indexs[j+1];
                        indexs[j+1] = temp_idx;
                    }
                }
            } return indexs;
        }

        float getResultMean(){
            float temp_flt;
            for(int i=0; i<N;i++){
                temp_flt += (*PTR_Y)[i];
            }
            float predicts = temp_flt / N;
            return predicts;
        }

        float process(){
            prepare_data();
            euclideanDistance();
            std::vector<int> index_new = sorting();
            float new_pred = getResultMean();
            return new_pred;
        }
};

class CSV{
    public:
        std::string fileName;
        std::vector<std::vector<float>> VECTOR2D_FLOAT;
        std::vector<std::string> VECTOR2D_STR;

        CSV(std::string file){
            fileName = file;
        }
        
        void readFile(){
            std::ifstream FILE_I(fileName);
            if(FILE_I.is_open()){
                std::string row;
                std::string val;
                std::string header;
       
                // for header
                std::getline(FILE_I, row);
                std::stringstream header_ss(row);
                int z = 0; 
                while(std::getline(header_ss, val, ',')){
                    header += ' ' + val + ' ';
                    z++;
                }
                std::cout << "Header: " << header << std::endl;
                
                // for entire rows
                int i = 0;
                while(std::getline(FILE_I, row)){
                    std::stringstream row_ss(row);
                    std::string Ys;
                    std::vector<float> Xs;
                    std::string row_cout;
                    std::string val;
                    float valf;
                    
                    // for val percols
                    while(std::getline(row_ss, val, ',')){
                        try {
                            valf = std::stof(val);
                            Xs.push_back(valf);
                        }
                        catch (...){
                            VECTOR2D_STR.push_back(val);
                        }
                        row_cout += ' ' + val + ' ';
                    }  
                    std::cout << "Row   " << i << ": " << row_cout << std::endl;
                    VECTOR2D_FLOAT.push_back(Xs);
                    i++; 
                }
            }
        }
    
        void writeFile(std::vector<std::string>* someSentences){
            std::ofstream FILE_O(fileName);
            if(FILE_O.is_open()){
                for(int i=0; i<someSentences->size(); i++){
                    FILE_O << (*someSentences)[i];
                }
                FILE_O.close();
                std::cout << "File is created successfully as " << fileName << std::endl;
            }   else {
                std::cout << "Wrongly error idk why" << std::endl;
            }
        }

        std::vector<std::vector<float>>* extractDataX(){
            return &VECTOR2D_FLOAT;
        }

        std::vector<std::string>* extractDataY(){
            return &VECTOR2D_STR;
        }
};

std::vector<std::string> convert(std::vector<std::vector<int>>* X, std::vector<std::string>* Y, std::string header){
    std::vector<std::string> sentences;
    sentences.push_back(header);

    for(int i=0;i<X->size();i++){
        std::string row = "";
        for(int j=0;j<(*X)[i].size();j++){
            row += std::to_string((*X)[i][j]) + ',';
        }
        sentences.push_back(row+(*Y)[i]+"\n");
    }
    return sentences;
}

void processAll(std::vector<std::vector<int>>* X, std::vector<std::string>* Y, std::string header, std::string dataTrainName, std::string dataPredictName, std::vector<float>* NEW_DATA){
    // converts first
    std::vector<std::string> sentences1 = convert(X, Y, header);
    std::string file = dataTrainName;
    CSV FILE_CSV(file);
    
    // write and read csv
    FILE_CSV.writeFile(&sentences1);
    FILE_CSV.readFile();
    
    // extract data
    std::vector<std::vector<float>>* DATA_X = FILE_CSV.extractDataX();
    std::vector<std::string>* DATA_Y = FILE_CSV.extractDataY();
    
    std::cout << " " << std::endl;
    // assign KKN
    KNN_CLASSIFICATION KKN_MODEL(DATA_X, DATA_Y, NEW_DATA, 3);
    std::vector<std::string> NEW_PREDICT = KKN_MODEL.process();
    
    // print new data
    std::string newdtstr = "";
    for(int i=0; i<NEW_DATA->size();i++){
        newdtstr += std::to_string((float)(*NEW_DATA)[i]) + "  ";
    }
    std::cout << "New data: " << newdtstr << std::endl;
    std::string file_p = dataPredictName;
    CSV FILE_CSV_PREDICT(file_p);
    FILE_CSV_PREDICT.writeFile(&NEW_PREDICT);
}

int main(){
    // ubungen KKN1
    // initiate vars
    std::vector<std::vector<int>> X1 = {
        {8, 4},
        {4, 5},
        {4, 6},
        {7, 7},
        {5, 6},
        {6, 5}
    };
    std::vector<std::string> Y1 = {
        {"BAIK", "JELEK", "JELEK", "BAIK", "JELEK", "BAIK"}
    };
    std::string header1 = "X1,X2,Y\n";
    std::vector<float> NEW_DATA1 = {6, 7};
    processAll(&X1, &Y1, header1, "kkn1.csv", "kkn1_predict.csv", &NEW_DATA1);
    
    // ubungen KKN2
    // initiate vars
    std::vector<std::vector<int>> X2 = {
        {40,5,60},
        {50,8,40},
        {50,7,30},
        {70,4,60},
        {80,4,80},
        {60,6,60}
    };
    std::vector<std::string> Y2 = {
        {"TOLAK", "TERIMA", "TOLAK", "TERIMA", "TERIMA", "TERIMA"}
    };
    std::string header2 = "X1,X2,X3,Y\n";
    std::vector<float> NEW_DATA2 = {60, 7, 90};
    processAll(&X2, &Y2, header2, "kkn2.csv", "kkn2_predict.csv", &NEW_DATA2);
    
    // KNN_REGRESI 1
    // initiate vars
    std::vector<std::vector<float>> X3 = {
        {100, 5},
        {150, 10},
        {200, 15},
        {80, 2},
        {120, 8}
    };
    std::vector<float> Y3 = {
        1.2,1.8,2.3,0.9,1.4
    };
    std::vector<float> NEW_DATA3 = {110, 6};
    KNN_REGRESI KKN_MODEL_RGR(&X3, &Y3, &NEW_DATA3);
    float pred = KKN_MODEL_RGR.process(); 
    
    std::cout << "" << std::endl;
    std::cout << "Hasil prediksinnya: " << pred << std::endl;
    std::cin.get();
    return 0;
}

// i use cpp for excercise, to run this cpp file, first need to install all cpp requeieremnt to ur devices
// if u dont wanna install that, use online compiler cpp:
// 1. https://www.jdoodle.com/online-compiler-c++
// 2. https://www.onlinegdb.com/online_c++_compiler
// run this command below in terminal, ctrl + shift + `. then run the file.exe that and the csv and the answers is da
// g++ -o tes KA.cpp
// Noted: hhhh, im made for 1D predict output, i forget to turn input into 2D. in euclidean distance is for 1d