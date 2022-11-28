#include <iostream>
#include <cmath>
#include <limits>




using namespace std;

typedef std::numeric_limits< double > dbl;


double ex(double x, double eps=0.001){
    double sum = 1.0, cur = 1.0;
    int n = 1;

    //i+1 element = i * (x/n)
    while(cur > eps){
        cur *= x/n++;
        sum += cur;
    }
    cout << "Accuracy: " << fixed << eps <<  "\n" << "Step: " << n << endl;

    cout << "Function: " << sum << endl;
    return sum;
}

double ch(double x, double eps=0.001){
    double sum = 1.0, cur = 1.0;
    int n = 1;
    double x_sq = pow(x, 2);

    //i+1 element = i * x^2 / (2n-1) * 2n
    while(cur > eps){
        double two_n = 2 * n;
        cur *= x_sq;
        cur /= (two_n - 1) * two_n;

        sum += cur;
        n+=1;
    }
    cout << "Accuracy: " << fixed << eps << "\n" << "Step: " << n << endl;

    cout << "Function: " << sum  << endl;
    return sum;
}