#include <iostream>
#include <string>
#include "timer.hh"
#include "Matrix.hh"
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <wait.h>

unsigned int sizeof_dm(int rows, int cols, size_t sizeElement)
{
  size_t size = rows * (sizeof(void *) + (cols * sizeElement));
  return size;
}

void create_index(void **m, int rows, int cols, size_t sizeElement)
{
  int i;
  size_t sizeRow = cols * sizeElement;
  m[0] = m + rows;
  for (i = 1; i < rows; i++)
  {
    m[i] = (m[i - 1] + sizeRow);
  }
}

void print_matriz(int **matrix, int Rows, int Cols)
{
  printf("\n");
  for (int i = 0; i < Rows; i++)
  {
    for (int j = 0; j < Cols; j++)
      printf("%i\t", matrix[i][j]);
    printf("\n");
  }
}

void computeSubMatrix(const Matrix &a, const Matrix &b, size_t i, size_t j, int **result)
{
  //Matrix result(a.size());
  for (size_t col = i; col < j; col++)
  {
    for (size_t r = 0; r < a.size(); r++)
    {
      for (size_t c = 0; c < a.size(); c++)
      {
        result[r][col] += a.at(r, c) * b.at(c, col);
      }
    }
  }
}

void mult6(const Matrix &a, const Matrix &b)
{
  int **matrix;
  pid_t *procs;
  int shmId;
  int shmPro;

  int part = a.size() / 4;

  size_t sizeMatrix = sizeof_dm(a.size(), a.size(), sizeof(int));
  shmId = shmget(IPC_PRIVATE, sizeMatrix, IPC_CREAT | 0600);

  shmPro = shmget(IPC_PRIVATE, 4 * sizeof(pid_t), IPC_CREAT | 0600);
  matrix = (int **)shmat(shmId, NULL, 0);
  procs = (pid_t *)shmat(shmPro, NULL, 0);

  //procs[0] = (pid_t)-1;
  //cout << procs[0] << " " << procs[1] << " " << procs[2] << " " << procs[3] << endl;

  create_index((void **)matrix, a.size(), a.size(), sizeof(int));

  pid_t pid = fork();
  //pid_t process_1, process_2, process_3, process_4;
  if (pid)
  {
    pid_t process_1 = pid;
    //calculo de un fragmento
    //cout << "Proceso 1" << process_1 << endl;
    pid = fork();
  }
  if (pid)
  {
    pid_t process_2 = pid;
    //cout << "Proceso 2" << process_2 << endl;
    pid = fork();
  }
  if (pid)
  {
    pid_t process_3 = pid;
    //cout << "Proceso 3" << getpid() << endl;
    pid = fork();
  }
  if (pid)
  {
    pid_t process_4 = pid;
    //cout << "Proceso 4" << getpid() << endl;
  }
  if (pid == 0)
  {
    //cout << "Proceso Hijo (1)" << getpid() << endl;
    int index;
    for (int i = 0; i < 4; i++)
    {
      if (procs[i] == 0)
      {
        procs[i] = 1;
        index = i;
        break;
      }
    }
    //cout << "Proceso Hijo (2)" << getppid() << endl;
    computeSubMatrix(a, b, part * index, part * (index + 1), matrix);

    shmdt(matrix);
    shmctl(shmId, IPC_RMID, 0);
  }
  else
  {
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    //print_matriz(matrix, a.size(), b.size());
    //cout << "Finalizado correctamente" << endl;
  }
}

int main(int argc, char **argv)
{
  int i = stoi(argv[1]);
  Matrix m(i);
  m.fill();
  Matrix n(i);
  n.fill();
  mult6(m, n);

  //cout << "Todo listo: " << i << endl;

  //cout << "Todo listo: " << i << endl;
  return 0;
}