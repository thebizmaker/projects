namespace py matrix

struct Matrix {
  1: required i32 row,
  2: required i32 col,
  3: required string worker,
  4: required string final_dims,
  5: required list<list<i32>> data
}

service MatrixService {
  string computeMatrix(1: Matrix matrix),
  void storeResult(1: string result, 2: string worker)
}