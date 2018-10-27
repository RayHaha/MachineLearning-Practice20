// MLHW6.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

void Algorithm(double data[100][3], double gt[2], double u[100]);
void BubbleSort(double list[]);

int _tmain(int argc, _TCHAR* argv[])
{
	char theline [100] ;
	
	//read file
	fstream fin;
	fin.open("train.txt",ios::in);

	double data[100][3];
	double u[301][100];
	double alpha[300];
	double g[300][4];   // 0:s  1:i  2:theda  3:einu
	double ein[300];
	double err[300];

	// initial
	for(int i=0; i<301; i++){
		for(int j=0; j<100; j++){
			u[i][j] = 0;
		}

	}

	for(int i=0; i<100; i++){
		u[0][i] = (1.0/100.0);
	}
	
	
	for(int i=0; i<300; i++){
		alpha[i] = 0;
		ein[i] = 0;
		err[i]= 0;
		for(int j=0; j<4; j++){
			g[i][j] = 0;
		}

	}

	

	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}


	for(int t=0; t<300; t++){

		Algorithm(data,g[t],u[t]);  // 0:s  1:i  2:theda  3:einu
		

		double einu = 0;
		double totaleinu = 0;
		for(int i=0; i<100; i++){
			double temp = 0;
			if(g[t][1] == 0){
				temp = g[t][0]*(data[i][0]-g[t][2]);
			}else{
				temp = g[t][0]*(data[i][1]-g[t][2]);
			}
			int ans = 0;
			if(temp>=0){
				ans = 1;
			}else{
				ans = -1;
			}
			if(ans==data[i][2]){

			}else{
				einu = einu + u[t][i];
			}
		}
		for(int i=0; i<100; i++){
			totaleinu = totaleinu + u[t][i];
		}
		err[t] = (double)einu/(double)totaleinu;
		
		double scale = sqrt((1-err[t])/err[t]);

		for(int i=0; i<100; i++){
			double temp = 0;
			if(g[t][1] == 0){
				temp = g[t][0]*(data[i][0]-g[t][2]);
			}else{
				temp = g[t][0]*(data[i][1]-g[t][2]);
			}
			int ans = 0;
			if(temp>=0){
				ans = 1;
			}else{
				ans = -1;
			}
			if(ans==data[i][2]){
				u[t+1][i] = u[t][i]/scale;
			}else{
				u[t+1][i] = u[t][i]*scale;
			}
		}
		alpha[t] = log(scale);
	}
	
	double test[1000][3];

	fstream fs;
	fs.open("test.txt",ios::in);
	//分割字串存入二維陣列
	char *token2 = NULL;
	char *next_token2 = NULL;
	int c=0;
	while(fs.getline(theline,sizeof(theline),'\n')){
		int d = 0;
		token2 = strtok_s( theline, seps, &next_token2);
		while (token2 != NULL){
			test[c][d] = atof(token2);
            token2 = strtok_s( NULL, seps, &next_token2);
			d++;
		}
		c++;
	}

	for(int t=0; t<300; t++){
		for(int i=0; i<1000; i++){
			double temp = 0;
			if(g[t][1] == 0){
				temp = g[t][0]*(test[i][0]-g[t][2]);
			}else{
				temp = g[t][0]*(test[i][1]-g[t][2]);
			}
			int ans = 0;
			if(temp>=0){
				ans = 1;
			}else{
				ans = -1;
			}
			if(ans==test[i][2]){
				
			}else{
				ein[t]++;
			}
		}
		ein[t] = ein[t]/1000;
	}

	
	//open file
	char filename3[]="ein.txt";
	fstream fp;
	fp.open(filename3, ios::out);//開啟檔案
	if(!fp){//如果開啟檔案失敗，fp為0；成功，fp為非0
        cout<<"Fail to open file: "<<filename3<<endl;
    }
	for(int i=0; i<300; i++){
		fp<<ein[i]<<endl;//寫入字串
	}
	fp.close();//關閉檔案


	system("pause");
	return 0;
}

void Algorithm(double data[100][3], double gt[4], double u[100]){
	double x[2][100] ;
	for(int i=0; i<2; i++){
		for(int j=0; j<100; j++){
			x[i][j] = data[j][i];
		}
	}

	double best[4]={0,0,0,10000};  // 0:s  1:i  2:theda  3:einu

	for(int xi=0; xi<2; xi++){
		
		BubbleSort(x[xi]);
		for(int i=-1; i<99; i++){
			double median = 0;
			if(i==-1){
				median = x[xi][0] - 100; // 最左邊
			}else{
				median = (double)(x[xi][i]+x[xi][i+1])/(double)2;
			}

			double err = 0;
			double einu = 0;
			// s=1
			for(int j=0; j<100; j++){
				double temp = data[j][xi] - median;
				double sign = 0;
				if(temp>=0){
					sign = 1;
				}else{
					sign = -1;
				}

				if(sign == data[j][2]){

				}else{
					einu = einu + u[j];
				}
			}
			if(einu<best[3]){
				best[0] = 1;
				best[1] = xi;
				best[2] = median;
				best[3] = einu;
			}
			err = 0;
			einu = 0;
			// s=-1
			for(int j=0; j<100; j++){
				double temp = data[j][xi] - median;
				double sign = 0;
				if(temp>=0){
					sign = 1;
				}else{
					sign = -1;
				}

				if(sign*(-1) == data[j][2]){

				}else{
					einu = einu + u[j];
				}
			}
			if(einu<best[3]){
				best[0] = -1;
				best[1] = xi;
				best[2] = median;
				best[3] = einu;
			}
		}
	}
	
	for(int i=0; i<4; i++){
		gt[i] = best[i];
	}

}

void BubbleSort(double list[]){
	int size = sizeof(list);
	for(int i=0; i<size; i++){
		for(int j=0; j<size-i; j++){
			if(list[j]>list[j+1]){
				double temp = list[j];
				list[j] = list[j+1];
				list[j+1] = temp;
			}
		}
	}
}