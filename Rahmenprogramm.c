#include "Rahmenprogramm.h"

double f_c(double x, double y) {
    double res = exponent_c(y * ln_c(x));
    return res;
}

double ln_c(double x) {
    int n = 100;
    double y = (x - 1) / (x + 1);

    double sum = 0.0;
    for (double i = 0; i < n; i++) {
        double j = 2*i + 1;

        double mul = 1.0;
        while (j != 0) {
            mul = mul * y;

            j--;
        }
        
        sum += (1 / (2*i + 1)) * mul;
    }

    return 2 * sum;
}

double pow_int_c(double x, int y) { // y є Z, x є R
    double res = 1;
    int neg = 0;

    if(y < 0) {
        neg = 1;
        y = -y;
    }

    while(y > 0) {
        res *= x;
        y--;
    }

    if(neg ==1)
        return (1/res);
    else // neg == 0
        return res;
}

double invert_x_c(double x) {
    return 1 / x;
}

double exponent_c(double x) { 
    int n = 100;
    double sum = 1.0f;

    for (int i = n - 1; i > 0; --i ) 
        sum = 1 + x * sum / i; 

    return sum; 
} 

double simpson_c(double a, double a_step) {
    int otrezki = 10;
    double inner_step = (a_step - a)/otrezki;
			double ger_acc = 0;
			double unger_acc = 0;
			double x [otrezki];
			for (int i = 1; i<otrezki; i++) {
				x[i] = a + inner_step * i;
				if (i % 2 == 0)
					ger_acc += invert_x_c(x[i]);
				else
					unger_acc += invert_x_c(x[i]);
			}
			return (a_step - a) / (otrezki* 3) * (invert_x_c(a) + unger_acc * 4 + ger_acc * 2 + invert_x_c(a_step));
}
 
double ln_alternativ_c(double a) {
    if (a == 1)
        return 0;

    double result = 0;
    double step = (a - 1) / 100;

    for (double i = 1; i < a; i += step)
        result += simpson_c(i, i + step);

    return result;
}