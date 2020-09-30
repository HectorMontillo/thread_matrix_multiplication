#include <iostream>
#include <string>
#include "Matrix.hh"
#include "timer.hh"

// Multiplicación serial
Matrix mult(const Matrix &a, const Matrix &b)
{
  Matrix result(a.size());
  Timer t;
  for (int col = 0; col < b.size(); col++)
  {
    for (int r = 0; r < a.size(); r++)
    {
      for (int c = 0; c < a.size(); c++)
      {
        result.at(r, col) += a.at(r, c) * b.at(c, col);
      }
    }
  }
  cout << t.elapsed() << "\t";

  return result;
}

int main(int argc, char **argv)
{
  int i = stoi(argv[1]);
  Matrix m(i);
  m.fill();
  Matrix n(i);
  n.fill();
  Matrix r = mult(m, n);
}
