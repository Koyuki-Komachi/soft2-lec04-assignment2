#include <stdio.h>  
#include <stdlib.h>  
#include <math.h> 
#include "func1.h" 

#define MAX_LINE 100  
#define MAX_LOCATIONS 50    
#define MT_FUJI_ALT 3.776     

int main(){  
    FILE *fp;  
    char line[MAX_LINE]; 
    Sample locations[MAX_LOCATIONS];
    int data_count = 0; 
    
    if ((fp = fopen("data.csv", "r")) == NULL){  
        fprintf(stderr, "cannot open data.csv\n");  
        exit(EXIT_FAILURE);  
    }  
    
    // data.csvを読み取って地名、標高、気温を構造体のメンバー変数に入れる  
    while (fscanf(fp, " %[^,],%lf,%lf",   
                locations[data_count].loc,  
                &locations[data_count].alt,  
                &locations[data_count].temp) == 3){  
        data_count++;  
    }  
    
    fclose(fp);  

    // 標高を降順にソート
    qsort(locations, data_count, sizeof(Sample), compare_altitude);

    printf("Observation data sorted by altitude (decending order):\n");
    print_locations(locations, data_count);
    
    // 最小二乗法によるパラメータの推定  
    double x[2] = {0.0, 0.0};  // 初期値: a = 0, b = 0  
    double g[2];               // 勾配  
    double learning_rate = 0.01;  // 学習率  
    double eps = 0.005;          // 収束判定閾値  
    int max_iter = 10000;        // 最大イテレーション回数  
    int iter;  
    
    // 最急降下法による最適化  
    for (iter = 1; iter <= max_iter; iter++) {  
        double E = f_value(x, locations, data_count);  
        f_gradient(x, locations, data_count, g);  
        
        // イテレーション経過の表示  
        printf("Iteration %d: E = %.4f, a = %.4f, b = %.4f\n",   
               iter, E, x[0], x[1]);  
        
        // パラメータの更新  
        double x_new[2];  
        x_new[0] = x[0] - learning_rate * g[0];  
        x_new[1] = x[1] - learning_rate * g[1];  
        
        // 収束判定  
        if (fabs(f_value(x_new, locations, data_count) - E) < eps) {  
            x[0] = x_new[0];  
            x[1] = x_new[1];  
            break;  
        }  
        
        x[0] = x_new[0];  
        x[1] = x_new[1];  
    }  
    
    printf("\nConverged after %d iterations.\n", iter);  
    printf("Final parameters: a = %.4f, b = %.4f\n", x[0], x[1]);  
    
    // 富士山頂の気温を推定  
    double fuji_temp = x[0] * MT_FUJI_ALT + x[1];  
    printf("Estimated temperature at Mt. Fuji (%.3f km): %.4f °C\n",   
           MT_FUJI_ALT, fuji_temp);  
    
    return 0;  
}