#include "tests.h"

/*
Getestet wurde auf einem System mit einem Intel® Core™ i3-7100U CPU, 2.40GHz × 4,
3,7 GiB Arbeitsspeicher, Ubuntu 18.04.4 LTS, 64 Bit, Linux-Kernel 4.15.0-111-generic,
Kompiliert wurde mit GCC 7.5.0 mit der Option -03
*/
void execute_tests(int testNumber) {
    if(testNumber == 1) {
        test1();
    } else if(testNumber == 2) {
        test2();
    } else if(testNumber == 3) {
        test3();
    } else if(testNumber == 4) {
        test4();
   } else {
        printf("Running all tests...\n");

        test1();
        test2();
        test3();
        test4();
   }
   
}

// Laufzeittest
int test1() {
    printf("Running test 1...\n");

    FILE *out_asm = fopen("resources/test1_asm.txt", "w"); 
    FILE *out_c = fopen("resources/test1_c.txt", "w"); 
    FILE *out_bib = fopen("resources/test1_bib.txt", "w"); 

    int n = countLines("resources/test1.txt");

    FILE *file  = fopen("resources/test1.txt", "r"); 

    if(f==NULL)
        return 0;
 
    double values[n];
    unsigned int i;      
    for(i = 0; i < n; ++i) {
        int retVal = fscanf(file, "%lf",&values[i]);
        printf("%lf\n",values[i]);
    }   

    double x = 42.5391;
    fprintf(out_asm, "          Tested x = %lf; 15 time each value\n\n", x);
    fprintf(out_c, "          Tested x = %lf; 15 time each value\n\n", x);
    fprintf(out_bib, "          Tested x = %lf; 15 time each value\n\n", x);

    for (i = 0; i < n; ++i) {
        double y = values[i];

        int j = 15;
        double sum_asm = 0;
        double sum_c = 0;
        double sum_bib = 0;

        printf("i = %d! Be patient, we invoke sleep(1) between measurement\n", i);

        while (j != 0) {
            double erg;
            double erg_c;
            double erg_pow;

            struct timespec start_v2;
            struct timespec end_v2;
            clock_gettime (CLOCK_MONOTONIC, &start_v2);
            erg = f(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_v2);
            double time_v2 = end_v2.tv_sec - start_v2.tv_sec + 1e-9 *(end_v2.tv_nsec - start_v2.tv_nsec);

            sleep(1);

            struct timespec start_c;
            struct timespec end_c;
            clock_gettime (CLOCK_MONOTONIC, &start_c);
            erg_c = f_c(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_c);
            double time_c = end_c.tv_sec - start_c.tv_sec + 1e-9*(end_c.tv_nsec - start_c.tv_nsec);

            sleep(1);

            struct timespec start_pow;
            struct timespec end_pow;
            clock_gettime (CLOCK_MONOTONIC, &start_pow);
            erg_pow = pow(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_pow);
            double time_pow = end_pow.tv_sec - start_pow.tv_sec + 1e-9*(end_pow.tv_nsec - start_pow.tv_nsec);

            sum_asm += time_v2;
            sum_c += time_c;
            sum_bib += time_pow;

            j--;
        }
        
        fprintf(out_asm, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_asm/16);
        fprintf(out_c, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_c/16);
        fprintf(out_bib, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_bib/16);
    }
    
    pclose(file);

    printf("Test 1... OK; Check \"test1_asm.txt, test1_c.txt, test1_bib\" for results\n");

    return 1;
}

// Abweichung-Test
// Es gibt keinen Sinn mehrere Ergebnisse zu testen, da der Wert sich nicht verändert
int test2() {
    printf("Running test 2...\n");

    FILE *out_test2 = fopen("resources/test2.txt", "a"); // append

    double x = 42.5391;
    double y = 4.3988;

    double erg = f(x, y);
    double erg_c = f_c(x, y);
    double erg_pow =  pow(x, y);
    
    fprintf(out_test2, "Anzahle der Taylor Schritte %d\n", 100);
    fprintf(out_test2, "Abweichung Assembler  : %.15e\n", ((double)abs(erg - erg_c)/erg_c)*100.0);
    fprintf(out_test2, "Abweichung C mit -O3  : %.15e\n\n", ((double)abs(erg - erg_pow)/erg_pow)*100.0);

    printf("Test 2... OK; Check \"test2.txt\" for results\n");

    return 1;
}

// C Ausführungszeit je nach Taylor-Schritte
int test3() {
    printf("Running test 3...\n");

    FILE *out_asm = fopen("resources/test3_asm.txt", "a"); 
    FILE *out_c = fopen("resources/test3_c.txt", "a"); 
    FILE *out_bib = fopen("resources/test3_bib.txt", "a"); 
      
    double x = 42.5391;
    double y = 4.3988;
    int taylor_steps = 1300;

    double sum_asm = 0;
    double sum_c = 0;
    double sum_bib = 0;

    int j = 15;
    while (j != 0) {
        double erg;
        double erg_c;
        double erg_pow;

        struct timespec start_v2;
        struct timespec end_v2;
        clock_gettime (CLOCK_MONOTONIC, &start_v2);
        erg = f(x, y);
        clock_gettime (CLOCK_MONOTONIC, &end_v2);
        double time_v2 = end_v2.tv_sec - start_v2.tv_sec + 1e-9 *(end_v2.tv_nsec - start_v2.tv_nsec);

        struct timespec start_c;
        struct timespec end_c;
        clock_gettime (CLOCK_MONOTONIC, &start_c);
        erg_c = f_c(x, y);
        clock_gettime (CLOCK_MONOTONIC, &end_c);
        double time_c = end_c.tv_sec - start_c.tv_sec + 1e-9*(end_c.tv_nsec - start_c.tv_nsec);

        struct timespec start_pow;
        struct timespec end_pow;
        clock_gettime (CLOCK_MONOTONIC, &start_pow);
        erg_pow = pow(x, y);
        clock_gettime (CLOCK_MONOTONIC, &end_pow);
        double time_pow = end_pow.tv_sec - start_pow.tv_sec + 1e-9*(end_pow.tv_nsec - start_pow.tv_nsec);

        sum_asm += time_v2;
        sum_c += time_c;
        sum_bib += time_pow;

        j--;
    }
        
    fprintf(out_asm, "%d Schritte ====> Avg execution time: %.15lf\n", taylor_steps, sum_asm/16);
    fprintf(out_c, "%d Schritte  ====> Avg execution time: %.15lf\n", taylor_steps, sum_c/16);
    fprintf(out_bib, "====> Avg execution time: %.15lf\n", sum_bib/16);

    printf("Test 3... OK; Check \"test3_asm.txt, test3_c.txt, test3_bib\" for results\n");

    return 1;
}

int test4() {
    printf("Running test 4...\n");

    FILE *out_asm = fopen("resources/test4_asm.txt", "w"); 
    FILE *out_simp = fopen("resources/test4_simp.txt", "w"); 
    FILE *out_c = fopen("resources/test4_c.txt", "w"); 
    FILE *out_bib = fopen("resources/test4_bib.txt", "w"); 

    int n = countLines("resources/test4.txt");

    FILE *file  = fopen("resources/test4.txt", "r"); 

    if(f==NULL)
        return 0;
 
    double values[n];
    unsigned int i;      
    for(i = 0; i < n; ++i) {
        int retVal = fscanf(file, "%lf",&values[i]);
        printf("%lf\n",values[i]);
    }   

    double x = 11.5391;
    fprintf(out_asm, "          Tested x = %lf; 15 time each value\n\n", x);
    fprintf(out_c, "          Tested x = %lf; 15 time each value\n\n", x);
    fprintf(out_bib, "          Tested x = %lf; 15 time each value\n\n", x);
    fprintf(out_simp, "          Tested x = %lf; 15 time each value\n\n", x);

    for (i = 0; i < n; ++i) {
        double y = values[i];

        int j = 15;
        double sum_asm = 0;
        double sum_c = 0;
        double sum_bib = 0;
        double sum_simp = 0;

        printf("i = %d! Be patient, we invoke sleep(1) between measurement\n", i);

        while (j != 0) {
            double erg;
            double erg_c;
            double erg_pow;
            double erg_simp;

            struct timespec start_v2;
            struct timespec end_v2;
            clock_gettime (CLOCK_MONOTONIC, &start_v2);
            erg = f(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_v2);
            double time_v2 = end_v2.tv_sec - start_v2.tv_sec + 1e-9 *(end_v2.tv_nsec - start_v2.tv_nsec);

            sleep(1);

            struct timespec start_c;
            struct timespec end_c;
            clock_gettime (CLOCK_MONOTONIC, &start_c);
            erg_c = f_c(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_c);
            double time_c = end_c.tv_sec - start_c.tv_sec + 1e-9*(end_c.tv_nsec - start_c.tv_nsec);
            
            sleep(1);

            struct timespec start_pow;
            struct timespec end_pow;
            clock_gettime (CLOCK_MONOTONIC, &start_pow);
            erg_pow = pow(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_pow);
            double time_pow = end_pow.tv_sec - start_pow.tv_sec + 1e-9*(end_pow.tv_nsec - start_pow.tv_nsec);

            sleep(1);

            struct timespec start_simp;
            struct timespec end_simp;
            clock_gettime (CLOCK_MONOTONIC, &start_simp);
            erg_simp = f_alternativ(x, y);
            clock_gettime (CLOCK_MONOTONIC, &end_simp);
            double time_simp = end_simp.tv_sec - start_simp.tv_sec + 1e-9*(end_simp.tv_nsec - start_simp.tv_nsec);

            sum_asm += time_v2;
            sum_c += time_c;
            sum_bib += time_pow;
            sum_simp += time_simp;

            j--;
        }
        
        fprintf(out_asm, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_asm/16);
        fprintf(out_c, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_c/16);
        fprintf(out_bib, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_bib/16); 
        fprintf(out_simp, "Y Value: %lf ====> Avg execution time: %.15lf\n", y, sum_simp/16);
    }
    
    pclose(file);

    printf("Test 4... OK; Check \"test4_asm.txt, test4_c.txt, test4_bib, test4_simp.txt\" for results\n");

    return 1;
}

int countLines(char* name) {
    FILE *fp  = fopen(name, "r"); 

    if (fp == NULL) { 
        printf("Die Datei konnte nicht gefunden werden"); 
        return 0; 
    } 

    int count = 0;
    char c;

    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n')
            count = count + 1; 

    fclose(fp);

    return count;
}