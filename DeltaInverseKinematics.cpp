/**********************************************************************************************
 * Arduino Delta Kinematics- Version 1.0
 * by William Bailes <williambailes@gmail.com> http://tinkersprojects.com/
 *
 * This Library is licensed under a GPLv3 License
 * 
 * made with infomation from the following documents
 * - R.L. Williams II, “The Delta Parallel Robot: Kinematics Solutions”, Internet Publication, www.ohio.edu/people/williar4/html/pdf/DeltaKin.pdf, January 2016. 
 **********************************************************************************************/

#include "DeltaInverseKinematics.h"
#include <math.h>

/******************* SETUP *******************/

DeltaInverseKinematics::DeltaInverseKinematics(double *B1temp,double *B2temp,double *B3temp,double ArmLength,double RodLength,double BassTri,double PlatformTri)
{
  B1angle = B1temp;
  B2angle = B2temp;
  B3angle = B3temp;

  L = ArmLength;
  l = RodLength;

  Sb = BassTri;
  Sp = PlatformTri;

	Ub = Sb * 0.57735026919;
	Wb = Sb * 0.28867513459;

	Up = Sp * 0.57735026919;
	Wp = Sp * 0.28867513459;

	a = Wb - Up;
	b = (Sp - 1.73205 * Wb) / 2;
	c = Wp - Wb / 2;
}
        
/******************* SET *******************/

void DeltaInverseKinematics::setOffsets(double X, double Y, double Z)
{
  offsetX = X;
  offsetY = Y;
  offsetZ = Z;
}
void DeltaInverseKinematics::setLimits(double upperB1, double upperB2, double upperB3, double lowerB1, double lowerB2, double lowerB3)
{
  LimitsUpperB1 = upperB1;
  LimitsUpperB2 = upperB2;
  LimitsUpperB3 = upperB3;
  LimitsLowerB1 = lowerB1;
  LimitsLowerB2 = lowerB2;
  LimitsLowerB3 = lowerB3;
}


void DeltaInverseKinematics::set(double x,double y,double z)
{
  x += offsetX;
  y += offsetY;
  z += offsetY;

  double B1a;
  double B2a;
  double B3a;
  double B1b;
  double B2b;
  double B3b;

  this->calulate(x,y,z,&B1a,&B2a,&B3a,&B1b,&B2b,&B3b);

  if(B1a >= LimitsUpperB1 && B1a <= LimitsLowerB1 )
  {
    *B1angle = B1a;
  }
  else if(B1b >= LimitsUpperB1 && B1b <= LimitsLowerB1 )
  {
    *B1angle = B1b;
  }
  
  if(B2a >= LimitsUpperB2 && B2a <= LimitsLowerB2 )
  {
    *B2angle = B2a;
  }
  else if(B2b >= LimitsUpperB2 && B2b <= LimitsLowerB2 )
  {
    *B2angle = B2b;
  }
  
  if(B3a >= LimitsUpperB3 && B3a <= LimitsLowerB3 )
  {
    *B3angle = B3a;
  }
  else if(B3b >= LimitsUpperB3 && B3b <= LimitsLowerB3 )
  {
    *B3angle = B3b;
  }
}

        
/******************* TEST *******************/

bool DeltaInverseKinematics::test(double x,double y,double z)
{
  double B1a;
  double B2a;
  double B3a;
  double B1b;
  double B2b;
  double B3b;

  this->calulate(x,y,z,&B1a,&B2a,&B3a,&B1b,&B2b,&B3b);

  if((B1a < LimitsUpperB1 && B1a > LimitsLowerB1 )&&(B1b < LimitsUpperB1 && B1b > LimitsLowerB1 ))
  {
    return false;
  }
  
  if((B2a < LimitsUpperB2 && B2a > LimitsLowerB2 )&&(B2b < LimitsUpperB2 && B2b > LimitsLowerB2 ))
  {
    return false;
  }
  
  if((B3a < LimitsUpperB3 && B3a > LimitsLowerB3 )&&(B3b < LimitsUpperB3 && B3b > LimitsLowerB3 ))
  {
    return false;
  }

  return true;
}


        
/******************* FUNCTIONS *******************/

void DeltaInverseKinematics::calulate(double x,double y,double z, double *B1a,double *B2a,double *B3a,double *B1b,double *B2b,double *B3b)
{
	double E1 = 2 * L*(y + a);
	double F1 = 2 * z*L;
	double G1 = x * x + y * y + z * z + a * a + L * L + 2 * y*a - l * l;

	double E2 = - L * (1.73205*(x + b) + y + c);
	double F2 = 2 * z*L;
	double G2 = x * x + y * y + z * z + b * b + c * c + L * L + 2 * (x*b + y * c) - l * l;

	double E3 = L * (1.73205*(x - b) - y - c);
	double F3 = 2 * z*L;
	double G3 = x * x + y * y + z * z + b * b + c * c + L * L + 2 * (-x * b + y * c) - l * l;

	double T1 = (E1 * E1 + F1 * F1 - G1 * G1);
	double TH1a = 2 * atan2((-F1 + sqrt(T1)), (G1 - E1));
	double TH1b = 2 * atan2((-F1 - sqrt(T1)), (G1 - E1));
	*B1a = 1.57 + (TH1a >= 0) * TH1a + (TH1a < 0) * (6.28 + TH1a) ;
	*B1b = 1.57 + (TH1b >= 0) * TH1b + (TH1b < 0) * (6.28 + TH1b) ;

	double T2 = (E2 * E2 + F2 * F2 - G2 * G2);
	double TH2a = 2 * atan2((-F2 + sqrt(T2)), (G2 - E2));
	double TH2b = 2 * atan2((-F2 - sqrt(T2)), (G2 - E2));
	*B2a = 1.57 + (TH2a >= 0) * TH2a + (TH2a < 0) * (6.28 + TH2a);
	*B2b = 1.57 + (TH2b >= 0) * TH2b + (TH2b < 0) * (6.28 + TH2b);

	double T3 = (E3 * E3 + F3 * F3 - G3 * G3);
	double TH3a = 2 * atan2((-F3 + sqrt(T3)), (G3 - E3));
	double TH3b = 2 * atan2((-F3 - sqrt(T3)), (G3 - E3));
	*B3a = 1.57 + (TH3a >= 0) * TH3a + (TH3a < 0) * (6.28 + TH3a);
	*B3b = 1.57 + (TH3b >= 0) * TH3b + (TH3b < 0) * (6.28 + TH3b);
  
  TH1a += 1.57;
	TH1b += 1.57;
	TH2a += 1.57;
	TH2b += 1.57;
	TH3a += 1.57;
	TH3b += 1.57;

  while(TH1a<0){TH1a+=6.28;}
  while(TH1a>6.28){TH1a-=6.28;}
  while(TH1b<0){TH1b+=6.28;}
  while(TH1b>6.28){TH1b-=6.28;}

  while(TH2a<0){TH2a+=6.28;}
  while(TH2a>6.28){TH2a-=6.28;}
  while(TH2b<0){TH2b+=6.28;}
  while(TH2b>6.28){TH2b-=6.28;}

  while(TH3a<0){TH3a+=6.28;}
  while(TH3a>6.28){TH3a-=6.28;}
  while(TH3b<0){TH3b+=6.28;}
  while(TH3b>6.28){TH3b-=6.28;}

	*B1a = TH1a  ;
	*B1b = TH1b ;
	*B2a = TH2a ;
	*B2b = TH2b ;
	*B3a = TH3a ;
	*B3b = TH3b ;
}

double DeltaInverseKinematics::straightArms(double base,double platform,double armL1, double armL2)
{

  return 0;
}
