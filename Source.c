#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
#include <locale.h>

double f(double x);
double solve(double y, double a, double b, double eps);
double der2(double x, double eps);

double find_min(double a, double b, double step);
double find_max(double a, double b, double step);

void print_table(double a, double b, double step, double eps, FILE* out);
void table(double a, double b, double step, double eps);
int save_table_to_file(double a, double b, double step, double eps);

int main() {
    setlocale(LC_ALL, "RUS");

    int choice;
    double x, a, b, step, y, eps;

    do {
        printf("\n=== Меню ===\n");
        printf("1. Значение в точке\n");
        printf("2. Таблица значений\n");
        printf("3. Минимум или максимум на отрезке\n");
        printf("4. Найти x по Y\n");
        printf("5. Вторая производная\n");
        printf("6. Сохранить таблицу в файл\n");
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
            else if (mm == 2)
                printf("Максимум = %.6f\n", find_max(a, b, step));
            else
                printf("Неверный выбор\n");

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

            if (save_table_to_file(a, b, step, eps) < 0)
                printf("Ошибка записи в файл\n");
            break;
        }

    } while (choice != 0);

    return 0;
}

double f(double x) {
    if (x < -1)
        return (log(1 + x) - x) / (x * x);
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
    double minv = f(a);
    for (double x = a; x <= b; x += step)
        if (f(x) < minv) minv = f(x);
    return minv;
}

double find_max(double a, double b, double step) {
    double maxv = f(a);
    for (double x = a; x <= b; x += step)
        if (f(x) > maxv) maxv = f(x);
    return maxv;
}

void print_table(double a, double b, double step, double eps, FILE* out) {
    fprintf(out, "----------------------------------------\n");
    fprintf(out, "|      x       |        f(x)           |\n");
    fprintf(out, "----------------------------------------\n");

    for (double t = a; t <= b + eps; t += step)
        fprintf(out, "| %12.5f | %21.6f |\n", t, f(t));

    fprintf(out, "----------------------------------------\n");
}

void table(double a, double b, double step, double eps) {
    print_table(a, b, step, eps, stdout);
}

int save_table_to_file(double a, double b, double step, double eps) {
    char filename[100];
    printf("Имя файла: ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "w");
    if (!file) {
        return -1; 
    }
    print_table(a, b, step, eps, file);
    fclose(file);

    printf("Таблица сохранена в \"%s\"\n", filename);
    return 1;  
}
