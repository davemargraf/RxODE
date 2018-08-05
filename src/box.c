#include <stdio.h>
#include <stdarg.h>
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>
#define X0MIN 1e-5

double powerDi(double x, double lambda, int yj){
  if (lambda == 1) return x;
  if (yj == 0){
    if (lambda == 0.0) return exp(x);
    return pow(x*lambda,1.0/lambda)-1;
  } else {
    if (x >= 0){
      // log(x+1)= y; exp(y)-1=x
      if (lambda == 0.0) return expm1(x);
      return pow(x*lambda+1, 1/lambda)-1;
    } else {
      if (lambda == 2.0) return -expm1(-x);
      return 1 - pow(1 - (2-lambda)*x, 1/(2-lambda));
    }
  }
}

double powerD(double x, double lambda, int yj){
  if (lambda == 1.0) return x;
  if (yj == 0){
    double x0=x;
    if (x <= 0) x0= X0MIN;
    if (lambda == 0.0) return log(x0);
    return (pow(x0, lambda) - 1.0)/lambda;
  } else {
    if (x >= 0){
      if (lambda == 0.0) return log1p(x);
      return (pow(x + 1.0, lambda) - 1.0)/lambda;
    } else {
      if (lambda == 2.0) return -log1p(-x);
      double l2 = 2.0 - lambda;
      return (1.0 - pow(1.0 - x, l2))/l2;
    }
  }
}

double powerDD(double x, double lambda, int yj){
  if (lambda == 1) return 1;
  if (yj == 0){
    double x0 = x;
    if (x <= 0) return x0 = X0MIN;
    if (lambda == 0.0) return 1/x0;
    // pow(x,lambda)/lambda - 1/lambda
    return pow(x0, lambda-1);
  } else {
    if (x >= 0){
      if (lambda == 0.0) return 1/(x + 1.0);
      return pow(x + 1.0, lambda-1.0);
    } else {
      if (lambda == 2.0) return -1/(1.0 - x);
      return pow(1.0 - x, 1.0-lambda);
    }
  }
}

double powerDDD(double x, double lambda, int yj){
  if (lambda == 1) return 0;
  if (yj == 0){
    double x0 = x;
    if (x <= 0) return x0 = X0MIN;
    if (lambda == 0.0) return -1/(x0*x0);
    // pow(x,lambda)/lambda - 1/lambda
    return (lambda-1)*pow(x0, lambda-2);
  } else {
    if (x >= 0){
      if (lambda == 0.0) return -1/((x + 1.0)*(x + 1.0));
      return (lambda-1.0)*pow(x + 1.0, lambda-2.0);
    } else {
      if (lambda == 2.0) return -1/((1.0 - x)*(1.0 - x));
      return -(1.0-lambda)*pow(1.0 - x, -lambda);
    }
  }
}

double powerL(double x, double lambda, int yj){
  // logLik addition based on dTBS
  // yj is indicator for yeo-johson
  if (lambda == 1.0) return 0;
  if (yj == 0){
    double x0 = x;
    if (x <= 0) x0 = X0MIN;
    return (lambda - 1.0)*log(x0);
  } else {
    if (x >= 0) return (lambda - 1.0)*log1p(x);
    return (1.0-lambda)*log1p(-x);
  }
  // d = 0.0 for cox box
  // d = 1.0 fo  Yeo- Johnson
  // logLik approximation
  // y^(lambda)/lambda - 1/lambda
  // dh/dy = y^(lambda-1)
  // log(dh/dy) = (lambda-1)*log(y) + log(lambda) 
  //
  // (x + 1.0)^(lambda)/lambda - 1/lambda
  // dh/dy = (x+1.0)^(lambda-1)
  // log(dh/dy) = (lambda-1)*log(x+1.0)
  
  // For negative values yj becomes
  // (-x+1)^(2-lambda)/(2-lambda) - 1/(2-lambda)
  // dh/dy = (-x+1)^(1-lambda)
  // log(dh/dy) = (1-lambda)*log(-x+1)
}

double powerDL(double x, double lambda, int yj){
  // d(logLik/dlambda)
  if (!yj){
    double x0 = x;
    if (x <= 0) x0 = X0MIN;
    return log(x0);
  } else {
    if (x >= 0) return log1p(x);
    return -log1p(x);
  }
  // d = 0.0 for cox box
  // d = 1.0 fo  Yeo- Johnson
  // logLik approximation
  // y^(lambda)/lambda - 1/lambda
  // dh/dy = y^(lambda-1)
  // log(dh/dy) = (lambda-1)*log(y) + log(lambda) 
  //
  // (x + 1.0)^(lambda)/lambda - 1/lambda
  // dh/dy = (x+1.0)^(lambda-1)
  // log(dh/dy) = (lambda-1)*log(x+1.0)
  
  // For negative values yj becomes
  // (-x+1)^(2-lambda)/(2-lambda) - 1/(2-lambda)
  // dh/dy = (-x+1)^(1-lambda)
  // log(dh/dy) = (1-lambda)*log(-x+1)

}
