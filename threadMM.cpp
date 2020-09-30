#include <iostream>
#include <thread>
#include <string>
#include "timer.hh"
#include "Matrix.hh"
#include "ThreadPool.hh"

void computeCol(const Matrix &a, const Matrix &b, size_t col, Matrix &result)
{
  for (size_t r = 0; r < a.size(); r++)
  {
    for (size_t c = 0; c < a.size(); c++)
    {
      result.at(col, r) += a.at(r, c) * b.at(col, c);
    }
  }
}
Matrix mult5(const Matrix &a, const Matrix &b)
{
  assert(a.size() == b.size());
  Matrix result(a.size());
  {
    ThreadPool pool(4);
    for (size_t c = 0; c < b.size(); c++)
      pool.enqueue([&result, &a, &b, c]() { computeCol(a, b, c, result); });
  }
  return result;
}
int main(int argc, char **argv)
{
  int i = stoi(argv[1]);
  Matrix m(i);
  m.fill();
  Matrix n(i);
  n.fill();
  Matrix r = mult5(m, n);
}
