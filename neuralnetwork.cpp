#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <string>
#include <fstream>
#include<iostream>
#include <iomanip>

#define SEED               1 
#define TRAINING_NUMBER   10 
#define TEST_NUMBER      100 
#define INPUT_NUMBER      15 
#define HIDDEN_NUMBER     10 
#define OUTPUT_NUMBER     10 
#define LEARNING_RATE   0.01 
#define LEARNING_TIME   1000 

using namespace std;
string folderRatePath = "./learning_rate/";
string hiddenPath = "./learning_hidden/hidden_result";
string allPath = "./learning_all/all_result";
string fileExtension = ".txt";

/*
要素を変更して組み合わせを変更
*/
int hiddenNumber[4] = {8,9,10,11};
float learningLate[150] = {0.1};
int leaningTime[50] = {950};

void make_test(void);
void init_weight(void);
void learn_rate(ofstream& file,float learningLate);
void learn_hidden(ofstream& file,int hiddenNumber);
float learn_all(int hiddenNumber,float learningLate,int learningTime);
double get_test_error(void);

double input_weight[HIDDEN_NUMBER][INPUT_NUMBER];          
double hidden_weight[OUTPUT_NUMBER][HIDDEN_NUMBER];        

double test_output[TEST_NUMBER][OUTPUT_NUMBER];            

double training_output[TRAINING_NUMBER][OUTPUT_NUMBER] = { 
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
};

double test_input[TEST_NUMBER][INPUT_NUMBER];             

double training_input[TRAINING_NUMBER][INPUT_NUMBER] = {
	{0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0},
	{1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
	{1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1},
	{1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1},
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
	{1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1},
	{1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1}
};

int main()
{
	for(int i=1;i<sizeof(learningLate) / sizeof(float);i++){
		learningLate[i] = learningLate[i-1]+0.001;
	}
	for (int i=1;i<sizeof(leaningTime) / sizeof(float);i++){
		leaningTime[i] = leaningTime[i-1]+5;
	}
	cout<<"初期化終了"<<endl;
	//100で最小値を初期化
	string filePathTmp = allPath+fileExtension;
	ofstream file(filePathTmp);
	if (!file.is_open()) {
		cout << "Error opening file " << filePathTmp << endl;
		return 1;
	}
	float min_test_error = 100.0;
	for (int i=0;i<sizeof(hiddenNumber) / sizeof(int);i++){
		for(int j=0;j<sizeof(learningLate) / sizeof(float);j++){
			for(int k=0;k<sizeof(leaningTime)/sizeof(int);k++){
				srand(SEED);
				make_test();
				init_weight();
				float error = learn_all(hiddenNumber[i],learningLate[j],leaningTime[k]);
				if(error<0.48){
					file << fixed << setprecision(5)<<"HIDDEN_NUMBER "<<hiddenNumber[i]<<" LEARNING_RATE "<<learningLate[j]<<" LEARNING_TIME "<<leaningTime[k]<<" テストデータの２乗誤差の平均値 "<<error<<endl;
			    }
				if(error<min_test_error){
					min_test_error = error;
					if(error<0.48){
						cout<<"HIDDEN_NUMBER "<<hiddenNumber[i]<<" LEARNING_RATE "<<learningLate[j]<<" LEARNING_TIME "<<leaningTime[k]<<" テストデータの２乗誤差の平均値 "<<min_test_error<<endl;
						file << fixed << setprecision(5)<<"HIDDEN_NUMBER "<<hiddenNumber[i]<<" LEARNING_RATE "<<learningLate[j]<<" LEARNING_TIME "<<leaningTime[k]<<" テストデータの２乗誤差の平均値 "<<min_test_error<<endl;
			        }
				}
			}
		}
	}
	file.close();
	// for (int i=0;i<6;i++){
	// 	string filePathTmp = folderRatePath+to_string(learningLate[i])+fileExtension;
	// 	ofstream file(filePathTmp);
	// 	if (!file.is_open()) {
	// 		cout << "Error opening file " << filePathTmp << endl;
	// 		return 1;
	// 	}

	// 	srand(SEED);
	// 	make_test();
	// 	init_weight();
	// 	learn_rate(file, learningLate[i]);
	// 	// learn_hidden(file,2);
	// 	file.close();
	// }
	// string filePathTmp = hiddenPath+fileExtension;
	// ofstream file(filePathTmp);
	// if (!file.is_open()) {
	// cout << "Error opening file " << filePathTmp << endl;
	// 	return 1;
	// }
	// for (int i=0;i<11;i++){
	// 	srand(SEED);
	// 	make_test();
	// 	init_weight();
	// 	// learn_rate(file, learningLate[i]);
	// 	learn_hidden(file,hiddenNumber[i]);
	// }
	// file.close();
    return 0;
}

void make_test(void)
{
	int t, i, o, tn;
	for (t = 0; t < TEST_NUMBER; t++){
		tn = (int)(((double)rand() / (1.0 + RAND_MAX)) * OUTPUT_NUMBER);
		for (i = 0; i < INPUT_NUMBER; i++){
			if (((double)rand() / ((double)RAND_MAX + 1)) < 0.1){
				test_input[t][i] = 1 - training_input[tn][i];
			}
			else{
				test_input[t][i] = training_input[tn][i];
			}
		}
		for (o = 0; o < OUTPUT_NUMBER; o++){
			test_output[t][o] = training_output[tn][o];
		}
	}
}

void init_weight(void){

	int h, i, o;
	for (h = 0; h < HIDDEN_NUMBER; h++){
		for (i = 0; i < INPUT_NUMBER; i++){
			input_weight[h][i] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
		}
	}
	for (o = 0; o < OUTPUT_NUMBER; o++){
		for (h = 0; h < HIDDEN_NUMBER; h++){
			hidden_weight[o][h] = ((double)rand() / ((double)RAND_MAX + 1)) * 0.2 - 0.1;
		}
	}
}

void learn_rate(ofstream& file,float learningLate){

	int l, t, h, i, o;
	double error,test_error;
	double wix, whs;
	double sigmoid_wix[TRAINING_NUMBER][HIDDEN_NUMBER];
	double o_whs[TRAINING_NUMBER][OUTPUT_NUMBER];
	double delta_sigmoid_wix[TRAINING_NUMBER][HIDDEN_NUMBER];
	double delta_input_weight[HIDDEN_NUMBER][INPUT_NUMBER];
	double delta_hidden_weight[OUTPUT_NUMBER][HIDDEN_NUMBER];

	for (l = 0; l < LEARNING_TIME; l++){
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (h = 0; h < HIDDEN_NUMBER; h++){
				wix = 0;
				for (i = 0; i < INPUT_NUMBER; i++){
					wix += input_weight[h][i] * training_input[t][i];
				}
				sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
				delta_sigmoid_wix[t][h] = sigmoid_wix[t][h] * (1 - sigmoid_wix[t][h]);
			}
		}
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				whs = 0;
				for (h = 0; h < HIDDEN_NUMBER; h++){
					whs += hidden_weight[o][h] * sigmoid_wix[t][h];
				}
				o_whs[t][o] = training_output[t][o] - whs;
			}
		}
		error = 0;
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				error += pow(o_whs[t][o], 2);
			}
		}
		test_error = get_test_error();
		file << fixed << setprecision(5)<<l+1<<" "<< test_error << endl;

		for (h = 0; h < HIDDEN_NUMBER; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				delta_input_weight[h][i] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					for (o = 0; o < OUTPUT_NUMBER; o++){
						delta_input_weight[h][i] += -2 * delta_sigmoid_wix[t][h] * hidden_weight[o][h] * o_whs[t][o] * training_input[t][i];
					}
				}
			}
		}

		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < HIDDEN_NUMBER; h++){
				delta_hidden_weight[o][h] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					delta_hidden_weight[o][h] += -2 * o_whs[t][o] * sigmoid_wix[t][h];
				}
			}
		}

		for (h = 0; h < HIDDEN_NUMBER; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				input_weight[h][i] -= learningLate * delta_input_weight[h][i];
			}
		}

		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < HIDDEN_NUMBER; h++){
				hidden_weight[o][h] -= learningLate * delta_hidden_weight[o][h];
			}
		}
	}
}

void learn_hidden(ofstream& file,int hiddenNumber){

	int l, t, h, i, o;
	double error,test_error;
	double wix, whs;
	double sigmoid_wix[TRAINING_NUMBER][hiddenNumber];
	double o_whs[TRAINING_NUMBER][OUTPUT_NUMBER];
	double delta_sigmoid_wix[TRAINING_NUMBER][hiddenNumber];
	double delta_input_weight[hiddenNumber][INPUT_NUMBER];
	double delta_hidden_weight[OUTPUT_NUMBER][hiddenNumber];

	for (l = 0; l < LEARNING_TIME; l++){
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (h = 0; h < hiddenNumber; h++){
				wix = 0;
				for (i = 0; i < INPUT_NUMBER; i++){
					wix += input_weight[h][i] * training_input[t][i];
				}
				sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
				delta_sigmoid_wix[t][h] = sigmoid_wix[t][h] * (1 - sigmoid_wix[t][h]);
			}
		}
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				whs = 0;
				for (h = 0; h < hiddenNumber; h++){
					whs += hidden_weight[o][h] * sigmoid_wix[t][h];
				}
				o_whs[t][o] = training_output[t][o] - whs;
			}
		}
		error = 0;
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				error += pow(o_whs[t][o], 2);
			}
		}
		test_error = get_test_error();
		if(l==LEARNING_TIME-1){
			file << fixed << setprecision(5)<<hiddenNumber<<" "<< test_error << endl;
		}

		for (h = 0; h < hiddenNumber; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				delta_input_weight[h][i] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					for (o = 0; o < OUTPUT_NUMBER; o++){
						delta_input_weight[h][i] += -2 * delta_sigmoid_wix[t][h] * hidden_weight[o][h] * o_whs[t][o] * training_input[t][i];
					}
				}
			}
		}

		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < hiddenNumber; h++){
				delta_hidden_weight[o][h] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					delta_hidden_weight[o][h] += -2 * o_whs[t][o] * sigmoid_wix[t][h];
				}
			}
		}

		for (h = 0; h < hiddenNumber; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				input_weight[h][i] -= LEARNING_RATE * delta_input_weight[h][i];
			}
		}

		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < hiddenNumber; h++){
				hidden_weight[o][h] -= LEARNING_RATE * delta_hidden_weight[o][h];
			}
		}
	}
}

float learn_all(int hiddenNumber,float learningLate,int learningTime){

	int l, t, h, i, o;
	double error,test_error;
	double wix, whs;
	double sigmoid_wix[TRAINING_NUMBER][hiddenNumber];
	double o_whs[TRAINING_NUMBER][OUTPUT_NUMBER];
	double delta_sigmoid_wix[TRAINING_NUMBER][hiddenNumber];
	double delta_input_weight[hiddenNumber][INPUT_NUMBER];
	double delta_hidden_weight[OUTPUT_NUMBER][hiddenNumber];

	for (l = 0; l < learningTime; l++){
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (h = 0; h < hiddenNumber; h++){
				wix = 0;
				for (i = 0; i < INPUT_NUMBER; i++){
					wix += input_weight[h][i] * training_input[t][i];
				}
				sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
				delta_sigmoid_wix[t][h] = sigmoid_wix[t][h] * (1 - sigmoid_wix[t][h]);
			}
		}
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				whs = 0;
				for (h = 0; h < hiddenNumber; h++){
					whs += hidden_weight[o][h] * sigmoid_wix[t][h];
				}
				o_whs[t][o] = training_output[t][o] - whs;
			}
		}
		error = 0;
		for (t = 0; t < TRAINING_NUMBER; t++){
			for (o = 0; o < OUTPUT_NUMBER; o++){
				error += pow(o_whs[t][o], 2);
			}
		}
		test_error = get_test_error();
		for (h = 0; h < hiddenNumber; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				delta_input_weight[h][i] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					for (o = 0; o < OUTPUT_NUMBER; o++){
						delta_input_weight[h][i] += -2 * delta_sigmoid_wix[t][h] * hidden_weight[o][h] * o_whs[t][o] * training_input[t][i];
					}
				}
			}
		}
		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < hiddenNumber; h++){
				delta_hidden_weight[o][h] = 0;
				for (t = 0; t < TRAINING_NUMBER; t++){
					delta_hidden_weight[o][h] += -2 * o_whs[t][o] * sigmoid_wix[t][h];
				}
			}
		}
		for (h = 0; h < hiddenNumber; h++){
			for (i = 0; i < INPUT_NUMBER; i++){
				input_weight[h][i] -= learningLate * delta_input_weight[h][i];
			}
		}
		for (o = 0; o < OUTPUT_NUMBER; o++){
			for (h = 0; h < hiddenNumber; h++){
				hidden_weight[o][h] -= learningLate * delta_hidden_weight[o][h];
			}
		}
	}
	//学習終了時にエラーを返す
	return test_error;
}


double get_test_error(void){

	double test_error;
	double wix, whs;
	double o_whs[TEST_NUMBER][OUTPUT_NUMBER];
	double sigmoid_wix[TEST_NUMBER][HIDDEN_NUMBER];

	for (int t = 0; t < TEST_NUMBER; t++){
		for (int h = 0; h < HIDDEN_NUMBER; h++){
			wix = 0;
			for (int i = 0; i < INPUT_NUMBER; i++){
				wix += input_weight[h][i] * test_input[t][i];
			}
			sigmoid_wix[t][h] = 1 / (1 + exp(-wix));
		}
	}

	for (int t = 0; t < TEST_NUMBER; t++){
		for (int o = 0; o < OUTPUT_NUMBER; o++){
			whs = 0;
			for (int h = 0; h < HIDDEN_NUMBER; h++){
				whs += hidden_weight[o][h] * sigmoid_wix[t][h];
			}
			o_whs[t][o] = test_output[t][o] - whs;
		}
	}

	test_error = 0;
	for (int t = 0; t < TEST_NUMBER; t++){
		for (int o = 0; o < OUTPUT_NUMBER; o++){
			test_error += pow(o_whs[t][o], 2);
		}
	}

	return test_error / TEST_NUMBER;
}