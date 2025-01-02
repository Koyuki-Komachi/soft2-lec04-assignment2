#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_LINE 100
#define MAX_LOCATIONS 50
#define MAX_NAME 32
#define MT_FUJI_ALT 3.776

typedef struct {
    char loc[MAX_NAME];
    double alt;
    double temp;
} Sample;

Sample locations[MAX_LOCATIONS];
int data_count = 0;

int f_dimension(){
    return 2;
}

double f_value(const double x[]){
    double a = x[0], b = x[1];
    double E = 0.0;

    for (int i = 0; i < data_count; ++i){
        double diff = locations[i].temp - (a * locations[i].alt + b);
        E += diff * diff;
    }
    return E;
}

void f_gradient(const double x[], double g[]){
    double a = x[0], b = x[1];
    g[0] = 0.0;
    g[1] = 0.0;
    
    for (int i = 0; i < data_count; ++i){
        double diff = locations[i].temp - (a * locations[i].alt + b);
        g[0] += -2.0 * locations[i].alt * diff;
        g[1] += -2.0 * diff;
    }
}

int main(){
    FILE *fp;
    char line[MAX_LINE];

    if ((fp = fopen("data.csv", "r")) == NULL){
        fprintf(stderr, "cannot open data.csv.\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(fp, "%[^,],%lf, %lf",
                locations[data_count].loc,
                &locations[data_count].alt,
                &locations[data_count].temp) == 3){
                    data_count++;
                }

    fclose(fp);

    double x[2] = {0.0, 0.0};
    double g[2];
    double learning_rate = 0.01;
    double eps = 0.005;
    int max_iter = 10000;
    int iter;

    for (iter = 1; iter <= max_iter; ++iter){
        double E = f_value(x);
        f_gradient(x, g);

        // イテレーション経過の表示
        printf("iteration %d: E = %.4f, a = %.4f, b = %.4f\n", 
                iter, E, x[0], x[1]);

        // パラメータの更新
        double x_new[2];
        x_new[0] = x[0] - learning_rate * g[0];
        x_new[1] = x[1] - learning_rate * g[1];

        // 収束判定
        if (fabs(f_value(x_new) - E) < eps){
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
    printf("Estimated temperature at Mt.Fuji (%.3f km): %.4f\n",
            MT_FUJI_ALT, fuji_temp);
}