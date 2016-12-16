#pragma once
#include <iostream>
using namespace std;
class shape
{
  public:
    virtual ~shape() {}
    virtual float get_square() const = 0;
    //    virtual void set_square(float a,float b,float c)=0;
};

class triangle: public shape
{

  public:
    triangle(): a(0), b(0), c(0)
    {
      //      cout << "constructor triangle" << endl;
    }
    triangle(const triangle &other)
    {
      a = other.a;
      b = other.b;
      c = other.c;
    }
    triangle &operator=(const triangle &other)
    {
      if (this != &other) {
        a = other.a;
        b = other.b;
        c = other.c;
      }

      return *this;
    }
    virtual float get_square() const override ;
    void set_square(float a, float b, float c);
    ~triangle()
    {
      //      cout << "destructor triangle" << endl;
    }
  private:
    float a, b, c;
};

class rectangle : public shape
{

  public:
    rectangle();
    virtual float get_square() const override;
    void set_square(float a, float b);
    ~rectangle()
    {
      //      cout << "destructor rectangle" << endl;
    }
  private:
    float a, b;
};

class circle : public shape
{
    const float PI;
  public:
    circle(): PI(3.14), r(0)
    {
      //      cout << "constructor circle" << endl;
    }
    virtual float get_square()const override;
    void set_square(float r);
    ~circle()
    {
      //      cout << "destructor circle" << endl;
    }

  private:
    float r;
};
