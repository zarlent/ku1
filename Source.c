#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <math.h>
#include <locale.h>

double f(double x);
double solve(double y, double a, double b, double eps);
double der2(double x);
void table(double a, double b, double step);
void minmax(double a, double b, double step,
    double* minx, double* minv,
    double* maxx, double* maxv);
void save_table_to_file(double a, double b, double step);

int main() {
    setlocale(LC_ALL, "RUS");

    int choice;
    double x, a, b, step, y;

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
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("step = ");
            scanf("%lf", &step);
            table(a, b, step);
            break;

        case 3: {
            int mm;
            double minx, minv, maxx, maxv;

            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("step = ");
            scanf("%lf", &step);

            printf("Что вывести?\n");
            printf("1 - Минимум\n");
            printf("2 - Максимум\n> ");
            scanf("%d", &mm);

            minmax(a, b, step, &minx, &minv, &maxx, &maxv);

            if (mm == 1)
                printf("\nМинимум: f(%.5f) = %.6f\n", minx, minv);
            else if (mm == 2)
                printf("\nМаксимум: f(%.5f) = %.6f\n", maxx, maxv);
            else
                printf("Неверный выбор\n");

            break;
        }

        case 4:
            printf("Y = ");
            scanf("%lf", &y);
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("x ~= %.6f\n", solve(y, a, b, 1e-6));
            break;

        case 5:
            printf("x = ");
            scanf("%lf", &x);
            printf("f''(%.4f) = %.6f\n", x, der2(x));
            break;

        case 6:
            printf("a = ");
            scanf("%lf", &a);
            printf("b = ");
            scanf("%lf", &b);
            printf("step = ");
            scanf("%lf", &step);

            if (step <= 0) {
                printf("Ошибка: шаг должен быть положительным\n");
                break;
            }

            save_table_to_file(a, b, step);
            break;
        }

    } while (choice != 0);

    return 0;
}

double f(double x) {
    if (x < -1)
        return (log(1 + x) - x) / (x * x);
    else if (x >= -1 && x < 4)
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

double der2(double x) {
    double h = 1e-5;
    return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}

void table(double a, double b, double step) {
    printf("\n----------------------------------------\n");
    printf("|      x       |        f(x)           |\n");
    printf("----------------------------------------\n");

    for (double t = a; t <= b; t += step)
        printf("| %12.5f | %21.6f |\n", t, f(t));

    printf("----------------------------------------\n");
}

void minmax(double a, double b, double step,
    double* minx, double* minv,
    double* maxx, double* maxv) {

    *minx = a;
    *maxx = a;
    *minv = f(a);
    *maxv = f(a);

    for (double t = a; t <= b; t += step) {
        double v = f(t);
        if (v < *minv) { *minv = v; *minx = t; }
        if (v > *maxv) { *maxv = v; *maxx = t; }
    }
}

void save_table_to_file(double a, double b, double step) {
    char filename[100];
    printf("Введите имя файла (например table.txt): ");
    scanf("%s", filename);

    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Ошибка открытия файла\n");
        return;
    }

    fprintf(file, "----------------------------------------\n");
    fprintf(file, "|      x       |        f(x)           |\n");
    fprintf(file, "----------------------------------------\n");

    for (double x = a; x <= b; x += step)
        fprintf(file, "| %12.5f | %21.6f |\n", x, f(x));

    fprintf(file, "----------------------------------------\n");

    fclose(file);
    printf("Таблица сохранена в файл \"%s\"\n", filename);
}
