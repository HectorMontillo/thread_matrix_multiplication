#include <iostream>
#include <thread>
#include <string>
#include "timer.hh"
#include "Matrix.hh"
#include "ThreadPool.hh"

// Multiplicación serial
Matrix mult(const Matrix &a, const Matrix &b)
{
  Matrix result(a.size());

  for (int r = 0; r < a.size(); r++)
  {
    for (int c = 0; c < a.size(); c++)
    {
      for (int rm = 0; rm < b.size(); rm++)
      {
        result.at(r, c) += a.at(r, rm) * b.at(rm, c);
      }
    }
  }

  return result;
}

void computeCell(const Matrix &a, const Matrix &b, size_t ra, size_t cb, double &result)
{
  result = 0.0;
  for (size_t nc = 0; nc < b.size(); nc++)
  {
    result += a.at(ra, nc) * b.at(nc, cb);
  }
}

// Calcular celda por celda
Matrix mult2(const Matrix &a, const Matrix &b)
{
  Matrix result(a.size());
  vector<thread> threads;

  for (int r = 0; r < a.size(); r++)
  {
    for (int c = 0; c < a.size(); c++)
    {
      // for (int rm = 0; rm < b.size(); rm++) {
      //   result.at(r, c) += a.at(r, rm) * b.at(rm, c);
      // }
      double re;
      threads.push_back(thread(computeCell, cref(a), cref(b), r, c, ref(re)));
      result.at(r, c) = re;
    }
  }
  for (thread &t : threads)
  {
    t.join();
  }
  return result;
}

void computeCol(const Matrix &a, const Matrix &b, size_t col, Matrix &result)
{
  for (size_t r = 0; r < a.size(); r++)
  {
    for (size_t c = 0; c < a.size(); c++)
    {
      result.at(r, col) += a.at(r, c) * b.at(c, col);
    }
  }
}

//Calcular columna por columna
Matrix mult3(const Matrix &a, const Matrix &b)
{
  Matrix result(a.size());
  vector<thread> threads;

  for (size_t c = 0; c < b.size(); c++)
  {
    threads.push_back(thread(computeCol, cref(a), cref(b), c, ref(result)));
  }
  for (thread &t : threads)
  {
    t.join();
  }
  return result;
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

void bench()
{
  cout << "n,\t"
       << "seq,\t"
       << "tcel,\t"
       << "tcol,\t"
       << "tpool"
       << endl;
  for (int i = 10; i < 5000;)
  {
    Matrix m(i);
    m.fill();
    Matrix n(i);
    n.fill();

    cout << i;

    Timer t;
    Matrix r = mult(m, n);
    cout << ",\t" << t.elapsed();

    Timer x;
    Matrix ur = mult2(m, n);
    cout << ",\t" << x.elapsed();

    Timer u;
    Matrix uur = mult3(m, n);
    cout << ",\t" << u.elapsed();

    Timer v;
    Matrix uuur = mult5(m, n);
    cout << ",\t" << v.elapsed();

    cout << endl;
    i = i + 5;
  }
}

int main()
{
  bench();
}
