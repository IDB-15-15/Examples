#include "figures.h"
#include <math.h>

float triangle::get_square() const
{
//  cout << "get_square triangle" << endl;
  float p = (a + b + c) / 2;
  return sqrt(p * (p - a) * (p - b) * (p - c));
}

void triangle::set_square(float a, float b, float c)
{
//  cout << "set_square triangle" << endl;
  this->a = a;
  this->b = b;
  this->c = c;
}



rectangle::rectangle()
{
//  cout << "constructor rectangle" << endl;
  this->a = this->b=0;
}

float rectangle::get_square() const
{
//  cout << "get_square rectangle" << endl;
  return a * b;
}

void rectangle::set_square(float a, float b)
{
//  cout << "set_square rectangle" << endl;
  this->a = a;
  this->b = b;
}


float circle::get_square() const
{
//  cout << "get_square circle" << endl;
  return PI * r * r;
}

void circle::set_square(float r)
{
//  cout << "set_square circle" << endl;
  this->r = r;
}
