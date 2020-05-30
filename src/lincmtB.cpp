//#undef NDEBUG
#include <R.h>
#include <stan/math.hpp>
#include "../inst/include/RxODE.h"
#ifdef ENABLE_NLS
#include <libintl.h>
#define _(String) dgettext ("RxODE", String)
/* replace pkg as appropriate */
#else
#define _(String) (String)
#endif

namespace stan {
  namespace math {

    using std::exp;
    using stan::math::exp;
    using std::sqrt;
    using stan::math::sqrt;
    using std::pow;
    using stan::math::pow;
    using std::acos;
    using stan::math::acos;
    using std::cos;
    using stan::math::cos;

    template<class T>
    Eigen::Matrix<T, Eigen::Dynamic, 2>
    micros2macros(const Eigen::Matrix<T, Eigen::Dynamic, 1>& p,
		  const int& ncmt,
		  const int& trans){
      Eigen::Matrix<T, Eigen::Dynamic, 2> g(ncmt,3);
      T btemp, ctemp, dtemp;
#define p1    p[0]
#define v1    p[1]
#define p2    p[2]
#define p3    p[3]
#define p4    p[4]
#define p5    p[5]
#define v     g(0, 0)
#define k     g(0, 1)
#define k12   g(1, 0)
#define k21   g(1, 1)
#define k13   g(2, 0)
#define k31   g(2, 1)
      switch (ncmt) {
      case 3: { // 3 compartment model 
	switch (trans){
	case 1: // cl v q vp
	  k = p1/v1; // k = CL/V
	  v = v1;
	  k12 = p2/v1; // k12 = Q/V
	  k21 = p2/p3; // k21 = Q/Vp
	  k13 = p4/v1; // k31 = Q2/V
	  k31 = p4/p5; // k31 = Q2/Vp2
	  break;
	case 2: // k=(*p1) v=(*v1) k12=(*p2) k21=(*p3) k13=(*p4) k31=(*p5)
	  k = p1;
	  v = v1;
	  k12 = p2;
	  k21 = p3;
	  k13 = p4;
	  k31 = p5;
	  break;
	case 11:
#undef beta
#define A (1/v1)
#define B (p3)
#define C (p5)
#define alpha (p1)
#define beta (p2)
#define gamma (p4)
	  v=1/(A+B+C);
	  btemp = -(alpha*C + alpha*B + gamma*A + gamma*B + beta*A + beta*C)*v;
	  ctemp = (alpha*beta*C + alpha*gamma*B + beta*gamma*A)*v;
	  dtemp = sqrt(btemp*btemp-4*ctemp);
	  k21 = 0.5*(-btemp+dtemp);
	  k31 = 0.5*(-btemp-dtemp);
	  k   = alpha*beta*gamma/k21/k31;
	  k12 = ((beta*gamma + alpha*beta + alpha*gamma) -
		 k21*(alpha+beta+gamma) - k * k31 + k21*k21)/(k31 - k21);
	  k13 = alpha + beta + gamma - (k + k12 + k21 + k31);
	  break;
	case 10:
#undef A
#define A v1
	  v=1/(A+B+C);
	  btemp = -(alpha*C + alpha*B + gamma*A + gamma*B + beta*A + beta*C)*v;
	  ctemp = (alpha*beta*C + alpha*gamma*B + beta*gamma*A)*v;
	  dtemp = sqrt(_as_zero(btemp*btemp-4*ctemp));
	  k21 = 0.5*(-btemp+dtemp);
	  k31 = 0.5*(-btemp-dtemp);
	  k   = alpha*beta*gamma/k21/k31;
	  k12 = ((beta*gamma + alpha*beta + alpha*gamma) -
		 k21*(alpha+beta+gamma) - k * k31 + k21*k21)/(k31 - k21);
	  k13 = alpha + beta + gamma - (k + k12 + k21 + k31);
#undef A
#undef B
#undef C
#undef alpha
#undef beta
#undef gamma
#define beta Rf_beta
	  break;
	}
      } break;
      case 2:{ // 2 compartment model
	switch (trans){
	case 1: // cl=(*p1) v=(*v1) q=(*p2) vp=(*p3)
	  k = p1/v1; // k = CL/V
	  v = v1;
	  k12 = p2/v1; // k12 = Q/V
	  k21 = p2/p3; // k21 = Q/Vp
	  break;
	case 2: // k=(*p1), (*v1)=v k12=(*p2) k21=(*p3)
	  k = p1;
	  v = v1;
	  k12 = p2;
	  k21 = p3;
	  break;
	case 3: // cl=(*p1) v=(*v1) q=(*p2) vss=(*p3)
	  k = p1/v1; // k = CL/V
	  v = v1;
	  k12 = p2/v1; // k12 = Q/V
	  k21 = p2/(p3-v1); // k21 = Q/(Vss-V)
	  break;
	case 4: // alpha=(*p1) beta=(*p2) k21=(*p3)
	  v = v1;
	  k21 = p3;
	  k = p1*p2/k21; // (*p1) = alpha (*p2) = beta
	  k12 = p1 + p2 - k21 - k;
	  break;
	case 5: // alpha=(*p1) beta=(*p2) aob=(*p3)
	  v=v1;
	  k21 = (p3*p2+p1)/(p3+1);
	  k = (p1*p2)/k21;
	  k12 = p1+ p2 - k21 - k;
	  break;
	case 11: // A2 V, alpha=(*p1), beta=(*p2), k21
#undef beta
#define A (1/v1)
#define B (p3)
#define alpha (p1)
#define beta (p2)
	  v   = 1/(A+B);
	  k21 = (A*beta + B*alpha)*v;
	  k   = alpha*beta/k21;
	  k12 = alpha+beta-k21-k;
	  break;
	case 10: // A=(*v1), alpha=(*p1), beta=(*p2), B=(*p3)
	  // Convert to A (right now A=(*v1) or A=1/(*v1))
#undef A
#define A (v1)
	  v   = 1/(A + B);
	  k21 = (A*beta + B*alpha)*v;
	  k   = alpha*beta/k21;
	  k12 = alpha + beta - k21 - k;
#undef A
#undef B
#undef alpha
#undef beta
#define beta Rf_beta
	  break;
	default:
	  REprintf(_("invalid trans (2 cmt trans %d)\n"), trans);
	  return NA_REAL;
	}
      } break;
      case 1:{ // One compartment model
	switch(trans){
	case 1: // cl v
	  k = p1/v1; // k = CL/V
	  v = v1;
	  break;
	case 2: // k V
	  k = p1;
	  v = v1;
	  break;
	case 11: // alpha V
	  k = p1;
	  v = v1;
	  break;
	case 10: // alpha A
	  k = p1;
	  v = 1/v1;
	  break;
	default:
	  return 0;
	}
      } break;
      }
#undef p1
#undef v1
#undef p2
#undef p3
#undef p4
#undef p4
      
#undef k
#undef v
#undef k12
#undef k21
#undef k13
#undef k31
      return g;
    }

#define v     g(0, 0)
#define k20   g(0, 1)
#define k23   g(1, 0)
#define k32   g(1, 1)
#define k24   g(2, 0)
#define k42   g(2, 1)
#define A1    A(0, 0)
#define A2    A(1, 0)
#define A3    A(2, 0)
#define A4    A(3, 0)
#define r1    rate(0, 0)
#define r2    rate(1, 0)
#define b1    bolus(0, 0)
#define b2    bolus(1, 0)
#define A1last Alast(0, 0)
#define A2last Alast(1, 0)
#define A3last Alast(2, 0)
#define A4last Alast(3, 0)
    // one compartment ka translations ncmt=1
#define tlag  params(2, 0)
#define F     params(3, 0)
#define rate1 params(4, 0)
#define dur1  params(5, 0)
#define ka    params(6, 0)
#define tlag2 params(7, 0)
#define f2    params(8, 0)
#define dur2  params(9, 0)    

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    oneCmtKaRateSSr1(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& rate) {
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(2, 1);
      A1 = r1/ka;
      A2 = r1/k20;
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    oneCmtKaRateSSr2(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& rate){
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(2, 1);
      A1 = 0;
      A2 = r2/k20;
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    oneCmtKaRateSStr1(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& rate,
		      T tinf, T tau){
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(2, 1);
      T eKa = exp(-ka*(tau-tinf))/(1.0-exp(-tau*ka));
      T eiKa = exp(-ka*tinf);
      T eiK = exp(-k20*tinf);
      T eK = exp(-k20*(tau-tinf))/(1.0-exp(-tau*k20));
      A1=eKa*(r1/ka - eiKa*r1/ka);
      A2=eK*(r1/k20 + eiKa*r1/(-k20 + ka) - eiK*r1*ka/(ka*k20 - k20*k20)) +
	ka*(eK - eKa)*(r1/ka - eiKa*r1/ka)/(-k20 + ka);
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    oneCmtKaRateSStr2(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& rate,
		      T tinf, T tau){
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(2, 1);
      T eiK = exp(-k20*tinf);
      T eK = exp(-k20*(tau-tinf))/(1.0-exp(-k20*tau));
      A1=0.0;
      A2=eK*(r2/k20 - eiK*r2*(-k20 + ka)/(ka*k20 - k20*k20));
      return A;
    }
    
    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    oneCmtKaRate(T t, Eigen::Matrix<T, Eigen::Dynamic, 2>& Alast,
		 Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		 Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		 Eigen::Matrix<T, Eigen::Dynamic, 2>& bolus,
		 Eigen::Matrix<T, Eigen::Dynamic, 2>& rate) {
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(2, 1);
      T eKa = exp(-ka*t);
      T e20 = exp(-k20*t);
      A1 = r1/ka-((r1-A1last*ka)*eKa)/ka + b1;
      T A21 = ((r1-A1last*ka)*eKa)/(ka-k20);
      T A22=(((ka-k20)*r2+ka*r1+(-A2last-A1last)*k20*ka+A2last*k20*k20)*e20)/(k20*ka-k20*k20);
      T A23 =(r2+r1)/k20;
      A2 = A21 - A22 + A23 + b2;
      return A;
    }
    // Undefine extras
#undef tlag
#undef F
#undef rate1
#undef dur1
#undef ka
#undef tlag2
#undef f2
#undef dur2
    // two compartment ka translations ncmt=1
#define tlag  params(4,  0)
#define F     params(5,  0)
#define rate1 params(6,  0)
#define dur1  params(7,  0)
#define ka    params(8,  0)
#define tlag2 params(9,  0)
#define f2    params(10, 0)
#define dur2  params(11, 0)    

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    twoCmtKaRateSSr1(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& rate) {
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(3, 1);
      T s = k23+k32+k20;
      //#Calculate roots
      T beta  = 0.5*(s - sqrt(s*s - 4*k32*k20));
      T alpha = k32*k20/beta;
      A1=r1/ka;
      A2=r1*k32/(beta*alpha);
      A3=r1*k23/(beta*alpha);
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    twoCmtKaRateSSr2(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		     Eigen::Matrix<T, Eigen::Dynamic, 2>& rate) {
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(3, 1);
      T s = k23+k32+k20;
      //#Calculate roots
      T beta  = 0.5*(s - sqrt(s*s - 4*k32*k20));
      T alpha = k32*k20/beta;
      A1=0;
      A2=r2*k32/(beta*alpha);
      A3=r2*k23/(beta*alpha);
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    twoCmtKaRateSStr1(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& rate,
		      T tinf, T tau){
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(3, 1);
      T s = k23+k32+k20;
      //#Calculate roots
      T beta  = 0.5*(s - sqrt(s*s - 4*k32*k20));
      T alpha = k32*k20/beta;

      T eA = exp(-alpha*(tau-tinf))/(1.0-exp(-alpha*tau));
      T eB = exp(-beta*(tau-tinf))/(1.0-exp(-beta*tau));

      T eiA = exp(-alpha*tinf);
      T eiB = exp(-beta*tinf);

      T alpha2 = alpha*alpha;
      T alpha3 = alpha2*alpha;

      T beta2 = beta*beta;
      T beta3 = beta2*beta;

      T ka2 = ka*ka;

      T eKa = exp(-ka*(tau-tinf))/(1.0-exp(-ka*tau));
      T eiKa = exp(-ka*tinf);

      A1=eKa*(r1/ka - eiKa*r1/ka);
      A2=(eA*(-alpha*(r1*k32/(beta*alpha) + eiKa*r1*(-k32 + ka)/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*(-alpha + k32)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*(-beta + k32)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k32*(r1*k23/(beta*alpha) - eiKa*r1*k23/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*k23/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*k23/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k32*(r1*k32/(beta*alpha) + eiKa*r1*(-k32 + ka)/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*(-alpha + k32)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*(-beta + k32)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))) - eB*(-beta*(r1*k32/(beta*alpha) + eiKa*r1*(-k32 + ka)/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*(-alpha + k32)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*(-beta + k32)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k32*(r1*k23/(beta*alpha) - eiKa*r1*k23/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*k23/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*k23/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k32*(r1*k32/(beta*alpha) + eiKa*r1*(-k32 + ka)/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*(-alpha + k32)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*(-beta + k32)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))))/(-alpha + beta) + ka*(eA*(-alpha + k32)/((-alpha + beta)*(-alpha + ka)) + eB*(-beta + k32)/((-beta + ka)*(alpha - beta)) + eKa*(k32 - ka)/((beta - ka)*(alpha - ka)))*(r1/ka - eiKa*r1/ka);
      A3=(eA*(-alpha*(r1*k23/(beta*alpha) - eiKa*r1*k23/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*k23/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*k23/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k23*(r1*k32/(beta*alpha) + eiKa*r1*(-k32 + ka)/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*(-alpha + k32)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*(-beta + k32)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + (k20 + k23)*(r1*k23/(beta*alpha) - eiKa*r1*k23/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*k23/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*k23/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))) - eB*(-beta*(r1*k23/(beta*alpha) - eiKa*r1*k23/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*k23/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*k23/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k23*(r1*k32/(beta*alpha) + eiKa*r1*(-k32 + ka)/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*(-alpha + k32)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*(-beta + k32)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + (k20 + k23)*(r1*k23/(beta*alpha) - eiKa*r1*k23/(beta*alpha + ka*(-alpha - beta) + ka2) - eiA*r1*ka*k23/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r1*ka*k23/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))))/(-alpha + beta) + ka*k23*(eA/((-alpha + beta)*(-alpha + ka)) + eB/((-beta + ka)*(alpha - beta)) + eKa/((beta - ka)*(alpha - ka)))*(r1/ka - eiKa*r1/ka);
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    twoCmtKaRateSStr2(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		      Eigen::Matrix<T, Eigen::Dynamic, 2>& rate,
		      T tinf, T tau) {
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(3, 1);
      T E2 = k20+k23;
      T E3 = k32;
      T s = k23+k32+k20;
      //#Calculate roots
      T beta  = 0.5*(s - sqrt(s*s - 4*k32*k20));
      T alpha = k32*k20/beta;

      T eA = exp(-alpha*(tau-tinf))/(1.0-exp(-alpha*tau));
      T eB = exp(-beta*(tau-tinf))/(1.0-exp(-beta*tau));

      T eiA = exp(-alpha*tinf);
      T eiB = exp(-beta*tinf);

      T alpha2 = alpha*alpha;
      T alpha3 = alpha2*alpha;

      T beta2 = beta*beta;
      T beta3 = beta2*beta;
      *A1=0.0;
      *A2=(eA*(E3*(r2*k32/(beta*alpha) - eiA*r2*(-k32*alpha + ka*(-alpha + k32) + alpha2)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k32*beta + ka*(-beta + k32) + beta2)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) - alpha*(r2*k32/(beta*alpha) - eiA*r2*(-k32*alpha + ka*(-alpha + k32) + alpha2)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k32*beta + ka*(-beta + k32) + beta2)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k32*(r2*k23/(beta*alpha) - eiA*r2*(-k23*alpha + ka*k23)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k23*beta + ka*k23)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))) - eB*(E3*(r2*k32/(beta*alpha) - eiA*r2*(-k32*alpha + ka*(-alpha + k32) + alpha2)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k32*beta + ka*(-beta + k32) + beta2)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) - beta*(r2*k32/(beta*alpha) - eiA*r2*(-k32*alpha + ka*(-alpha + k32) + alpha2)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k32*beta + ka*(-beta + k32) + beta2)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k32*(r2*k23/(beta*alpha) - eiA*r2*(-k23*alpha + ka*k23)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k23*beta + ka*k23)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))))/(-alpha + beta);
      *A3=(eA*(E2*(r2*k23/(beta*alpha) - eiA*r2*(-k23*alpha + ka*k23)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k23*beta + ka*k23)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) - alpha*(r2*k23/(beta*alpha) - eiA*r2*(-k23*alpha + ka*k23)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k23*beta + ka*k23)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k23*(r2*k32/(beta*alpha) - eiA*r2*(-k32*alpha + ka*(-alpha + k32) + alpha2)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k32*beta + ka*(-beta + k32) + beta2)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))) - eB*(E2*(r2*k23/(beta*alpha) - eiA*r2*(-k23*alpha + ka*k23)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k23*beta + ka*k23)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) - beta*(r2*k23/(beta*alpha) - eiA*r2*(-k23*alpha + ka*k23)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k23*beta + ka*k23)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3)) + k23*(r2*k32/(beta*alpha) - eiA*r2*(-k32*alpha + ka*(-alpha + k32) + alpha2)/(-beta*alpha2 + ka*(beta*alpha - alpha2) + alpha3) + eiB*r2*(-k32*beta + ka*(-beta + k32) + beta2)/(beta2*alpha + ka*(-beta*alpha + beta2) - beta3))))/(-alpha + beta);
      return A;
    }

    template <class T>
    Eigen::Matrix<double, Eigen::Dynamic, 1>
    twoCmtKaRate(T t, Eigen::Matrix<T, Eigen::Dynamic, 2>& Alast,
		 Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
		 Eigen::Matrix<T, Eigen::Dynamic, 2>& g,
		 Eigen::Matrix<T, Eigen::Dynamic, 2>& bolus,
		 Eigen::Matrix<T, Eigen::Dynamic, 2>& rate) {
      Eigen::Matrix<T, Eigen::Dynamic, 1> A(3, 1);
      T E2 =  k20+ k23;
      T s = k23+k32+k20;
      //#Calculate roots
      T beta  = 0.5*(s - sqrt(s*s - 4*k32*k20));
      T alpha = k32*k20/beta;

      T eKa = exp(-ka*t);
      T eA = exp(-alpha*t);
      T eB = exp(-beta*t);

      T ka2 = ka*ka;
  
      T alpha2 = alpha*alpha;
      T alpha3 = alpha2*alpha;
  
      T beta2 = beta*beta;
      T beta3 = beta2*beta;
  
      A1 = b1+r1/ka-((r1-A1last*ka)*eKa)/ka;
      A2 = b2+(((ka-k32)*r1-A1last*ka2+A1last*k32*ka)*eKa)/(ka2+(-beta-alpha)*ka+alpha*beta)+((((k32-beta)*ka-beta*k32+beta2)*r2+(k32-beta)*ka*r1+((-A3last-A2last-A1last)*beta*k32+(A2last+A1last)*beta2)*ka+(A3last+A2last)*beta2*k32-A2last*beta3)*eB)/((beta2-alpha*beta)*ka-beta3+alpha*beta2)-((((k32-alpha)*ka-alpha*k32+alpha2)*r2+(k32-alpha)*ka*r1+((-A3last-A2last-A1last)*alpha*k32+(A2last+A1last)*alpha2)*ka+(A3last+A2last)*alpha2*k32-A2last*alpha3)*eA)/((alpha*beta-alpha2)*ka-alpha2*beta+alpha3)+(k32*r2+k32*r1)/(alpha*beta);
      A3 = -((k23*r1-A1last*k23*ka)*eKa)/(ka2+(-beta-alpha)*ka+alpha*beta)+(((k23*ka-beta*k23)*r2+k23*ka*r1+((-A2last-A1last)*beta*k23+A3last*beta2-A3last*E2*beta)*ka+A2last*beta2*k23-A3last*beta3+A3last*E2*beta2)*eB)/((beta2-alpha*beta)*ka-beta3+alpha*beta2)-(((k23*ka-alpha*k23)*r2+k23*ka*r1+((-A2last-A1last)*alpha*k23+A3last*alpha2-A3last*E2*alpha)*ka+A2last*alpha2*k23-A3last*alpha3+A3last*E2*alpha2)*eA)/((alpha*beta-alpha2)*ka-alpha2*beta+alpha3)+(k23*r2+k23*r1)/(alpha*beta);
      return A;
    }
    
#undef v
#undef k20
#undef k23
#undef k32
#undef k24
#undef k42
#undef A1
#undef A2
#undef A3
#undef A3
#undef r1
#undef r2
#undef b1
#undef b2
    // undefine extras
#undef tlag
#undef F
#undef rate1
#undef dur1
#undef ka
#undef tlag2
#undef f2
#undef dur2
    
    template <class T>
    Eigen::Matrix<T, Eigen::Dynamic, 1>
    genericCmtInterface(Eigen::Matrix<T, Eigen::Dynamic, 1>& params,
			const double t,
			const int oral0,
			const int trans,
			const int ncmt,
			const int linCmt,
			rx_solving_options_ind *ind){
      Eigen::Matrix<T, Eigen::Dynamic, 2> par(ncmt, 3);
      par = micros2macros(params, ncmt, trans);
    }
  }
}

typedef Eigen::Matrix<double, Eigen::Dynamic, 1> MatrixPd;

extern "C" void sortIfNeeded(rx_solve *rx, rx_solving_options_ind *ind, unsigned int id,
			     int *linCmt,
			     double *d_tlag, double *d_tlag2,
			     double *d_F, double *d_F2,
			     double *d_rate1, double *d_dur1,
			     double *d_rate2, double *d_dur2);

extern "C" double getTime(int idx, rx_solving_options_ind *ind);

extern "C" int _locateTimeIndex(double obs_time,  rx_solving_options_ind *ind);

extern "C" double linCmtB(rx_solve *rx, unsigned int id, double t, int linCmt,
			  int i_cmt, int trans, int val,
			  double dd_p1, double dd_v1,
			  double dd_p2, double dd_p3,
			  double dd_p4, double dd_p5,
			  double dd_tlag, double dd_F, double dd_rate, double dd_dur,
			  // oral extra parameters
			  double dd_ka, double dd_tlag2, double dd_F2, double dd_rate2, double dd_dur2){
  
  // Get  Alast
  rx_solving_options_ind *ind = &(rx->subjects[id]);
  int evid, wh, cmt, wh100, whI, wh0;
  /* evid = ind->evid[ind->ix[ind->idx]]; */
  /* if (evid) REprintf("evid0[%d:%d]: %d; curTime: %f\n", id, ind->idx, evid, t); */
  int idx = ind->idx;
  sortIfNeeded(rx, ind, id, &linCmt, &dd_tlag, &dd_tlag2, &dd_F, &dd_F2,
	       &dd_rate, &dd_dur, &dd_rate2, &dd_dur2);
  rx_solving_options *op = rx->op;
  int oral0;
  oral0 = (dd_ka > 0) ? 1 : 0;
  double *A;
  double *Alast;
  double Alast0[4] = {0, 0, 0, 0};
  /* A = Alast0; Alast=Alast0; */
  double tlast;
  double it = getTime(ind->ix[idx], ind);
  double curTime=0.0;
  
  if (t != it) {
    // Try to get another idx by bisection
    /* REprintf("it pre: %f", it); */
    idx = _locateTimeIndex(t, ind);
    it = getTime(ind->ix[idx], ind);
    /* REprintf("it post: %f", it); */
  }
  /* REprintf("idx: %d; solved: %d; t: %f fabs: %f\n", idx, ind->solved[idx], t, fabs(t-it)); */
  int sameTime = fabs(t-it) < sqrt(DOUBLE_EPS);
  unsigned int ncmt = 1;
  if (dd_p4 > 0.){
    ncmt = 3;
  } else if (dd_p2 > 0.){
    ncmt = 2;
  }
  if (ind->solved[idx] && sameTime){
    // Pull from last solved value (cached)
    A = ind->linCmtAdvan+(op->nlin)*idx;
    // stop("fixme");
    if (val == 0){
      if (trans == 10) {
	return(A[oral0]*dd_v1);
      } else {
	return(A[oral0]/dd_v1);
      }    
    } else {
      return A[ncmt+oral0+val-1];
    }
  }
  MatrixPd params(2*ncmt + 4 + oral0*5, 1);
  params(0, 0) = dd_p1;
  params(1, 0) = dd_v1;
  if (ncmt >=2 ){
    params(2,0) = dd_p2;
    params(3,0) = dd_p3;
    if (ncmt >= 3){
      params(4,0) = dd_p4;
      params(5,0) = dd_p5;
    }
  }
  params(2*ncmt,     0) = dd_tlag;
  params(2*ncmt + 1, 0) = dd_F;
  params(2*ncmt + 2, 0) = dd_rate;
  params(2*ncmt + 3, 0) = dd_dur;
  if (oral0) {
    params(2*ncmt + 4, 0) = dd_ka;
    params(2*ncmt + 5, 0) = dd_tlag2;
    params(2*ncmt + 6, 0) = dd_F2;
    params(2*ncmt + 7, 0) = dd_rate2;
    params(2*ncmt + 8, 0) = dd_dur2;
  }
  
  return 0.0;
}
