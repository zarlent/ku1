#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
#include <locale.h>


#define W 60
#define H 20
#define Y_LIMIT 2000  


double f(double x);

double solve(double y, double a, double b, double eps);
double der2(double x, double eps);

double find_min(double a, double b, double step);
double find_max(double a, double b, double step);

void print_table(double a, double b, double step, double eps, FILE* out);
void table(double a, double b, double step, double eps);
int save_table_to_file(double a, double b, double step, double eps);

void plot(double x0, double x1);

int main() {
    setlocale(LC_ALL, "RUS");

    int choice;
    double x, a, b, step, y, eps;

    do {
        printf("\n=== Меню ===\n");
        printf("1. Значение в точке\n");
        printf("2. Таблица значений\n");
        printf("3. Минимум или максимум\n");
        printf("4. Найти x по Y\n");
        printf("5. Вторая производная\n");
        printf("6. Сохранить таблицу в файл\n");
        printf("7. График функции\n");
        printf("0. Выход\n> ");

        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("x = ");
            scanf("%lf", &x);
            printf("f(%.4f) = %.6f\n", x, f(x));
            break;

        case 2:
            printf("a b step eps: ");
            scanf("%lf %lf %lf %lf", &a, &b, &step, &eps);
            table(a, b, step, eps);
            break;

        case 3: {
            int mm;
            printf("a b step: ");
            scanf("%lf %lf %lf", &a, &b, &step);
            printf("1 - Минимум\n2 - Максимум\n> ");
            scanf("%d", &mm);

            if (mm == 1)
                printf("Минимум = %.6f\n", find_min(a, b, step));
            else
                printf("Максимум = %.6f\n", find_max(a, b, step));
            break;
        }

        case 4:
            printf("Y a b eps: ");
            scanf("%lf %lf %lf %lf", &y, &a, &b, &eps);
            printf("x ~= %.6f\n", solve(y, a, b, eps));
            break;

        case 5:
            printf("x eps: ");
            scanf("%lf %lf", &x, &eps);
            printf("f''(%.4f) = %.6f\n", x, der2(x, eps));
            break;

        case 6:
            printf("a b step eps: ");
            scanf("%lf %lf %lf %lf", &a, &b, &step, &eps);
            save_table_to_file(a, b, step, eps);
            break;

        case 7:
            printf("x0 x1: ");
            scanf("%lf %lf", &a, &b);
            plot(a, b);
            break;
        }

    } while (choice != 0);

    return 0;
}
double f(double x) {
    if (x < -1) {
        if (1 + x <= 0) return NAN;
        return (log(1 + x) - x) / (x * x);
    }
    else if (x < 4)
        return cos(3 * x) / pow(1 + x * x, 0.2);
    else
        return log(x * x + 2 * x + 2) *
        (4 * x * x * x - x * x + 3 * x - 2);
}


double solve(double y, double a, double b, double eps) {
    while (b - a > eps) {
        double m = (a + b) / 2;
        if (f(m) > y) b = m;
        else a = m;
    }
    return (a + b) / 2;
}

double der2(double x, double eps) {
    return (f(x + eps) - 2 * f(x) + f(x - eps)) / (eps * eps);
}

double find_min(double a, double b, double step) {
    double minv = 0;
    int first = 1;
    for (double x = a; x <= b; x += step) {
        double v = f(x);
        if (isnan(v)) continue;
        if (first || v < minv) {
            minv = v;
            first = 0;
        }
    }
    return minv;
}

double find_max(double a, double b, double step) {
    double maxv = 0;
    int first = 1;
    for (double x = a; x <= b; x += step) {
        double v = f(x);
        if (isnan(v)) continue;
        if (first || v > maxv) {
            maxv = v;
            first = 0;
        }
    }
    return maxv;
}

void print_table(double a, double b, double step, double eps, FILE* out) {
    fprintf(out, "----------------------------------------\n");
    fprintf(out, "|      x       |        f(x)           |\n");
    fprintf(out, "----------------------------------------\n");

    for (double t = a; t <= b + eps; t += step) {
        double v = f(t);
        if (isnan(v))
            fprintf(out, "| %12.5f |        ---             |\n", t);
        else
            fprintf(out, "| %12.5f | %21.6f |\n", t, v);
    }
    fprintf(out, "----------------------------------------\n");
}

void table(double a, double b, double step, double eps) {
    print_table(a, b, step, eps, stdout);
}

int save_table_to_file(double a, double b, double step, double eps) {
    char name[100];
    printf("Имя файла: ");
    scanf("%s", name);

    FILE* f = fopen(name, "w");
    if (!f) return -1;
    print_table(a, b, step, eps, f);
    fclose(f);
    return 1;
}


void plot(double x0, double x1)
{
    char scr[H][W];
    double y[W], x;
    double ymin = 0, ymax = 0;
    int first = 1;

    double hx = (x1 - x0) / (W - 1);

    for (int i = 0; i < W; i++) {
        x = x0 + i * hx;
        y[i] = f(x);

        if (isnan(y[i]) || isinf(y[i])) continue;

        
        if (fabs(y[i]) > Y_LIMIT) {
            y[i] = (y[i] > 0) ? Y_LIMIT : -Y_LIMIT;
        }

        if (first) {
            ymin = ymax = y[i];
            first = 0;
        }
        else {
            if (y[i] < ymin) ymin = y[i];
            if (y[i] > ymax) ymax = y[i];
        }
    }

    if (first) {
        printf("Функция не определена на интервале\n");
        return;
    }

    double hy = (ymax - ymin) / (H - 1);
    if (hy == 0) hy = 1;

    int yz = -1, xz = -1;

    if (0 >= ymin && 0 <= ymax)
        yz = (int)((ymax - 0) / hy + 0.5);

    if (0 >= x0 && 0 <= x1)
        xz = (int)((0 - x0) / hx + 0.5);

    
    for (int j = 0; j < H; j++)
        for (int i = 0; i < W; i++)
            scr[j][i] = ' ';

    
    if (yz >= 0 && yz < H)
        for (int i = 0; i < W; i++) scr[yz][i] = '-';

    if (xz >= 0 && xz < W)
        for (int j = 0; j < H; j++) scr[j][xz] = '|';

    if (yz >= 0 && yz < H && xz >= 0 && xz < W)
        scr[yz][xz] = '+';

    
    for (int i = 0; i < W; i++) {
        if (isnan(y[i]) || isinf(y[i])) continue;
        int j = (int)((ymax - y[i]) / hy + 0.5);
        if (j >= 0 && j < H)
            scr[j][i] = '*';
    }

   
    for (int j = 0; j < H; j++) {
        for (int i = 0; i < W; i++)
            putchar(scr[j][i]);
        putchar('\n');
    }
}
