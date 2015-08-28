// g++ playingWithFnPtrs.cpp -o playingWithFnPtrs -std=c++11 -Wall -O3

#include <iostream>
#include <chrono>
using std::cout;
using std::endl;

typedef float num;
typedef num (*op)(num, num);
num _add(num a, num b) {return a + b;}
num _sub(num a, num b) {return a - b;}
num _mul(num a, num b) {return a * b;}
num _div(num a, num b) {return a / b;}

enum switch_op {so_add, so_sub, so_mul, so_div};
num everyFn(switch_op op, num a, num b) {
  switch (op) {
    case so_add: return a+b;
    case so_sub: return a-b;
    case so_mul: return a*b;
    case so_div: return a/b;
  }
  return 0;
}


int main(int argc, char const *argv[]) {
  long dataSize = 1e6;
  num data[dataSize];
  cout << "dataSize: " << dataSize << endl;
  for (int i = 0; i < dataSize; i++) {data[i] = 0;}
  
  //control
  {
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
      data[i] = _add(90, _sub(12, _div(8, _mul(2, 2))));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "control: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  
  
  for (int i = 0; i < dataSize; i++) {
    if (data[i] != 100) {
      cout << "incorrect data at " << i << ": " << data[i] << endl;;
      exit(1);
    }
    data[i] = 0;
  }
  
  //every fn ptr per iteration
  {
    //the condition is here to prevent the compiler from inlining the fn ptrs
    const op opAdd = argc > 1000 ? _sub : _add;
    const op opSub = argc > 1000 ? _mul : _sub;
    const op opMul = argc > 1000 ? _div : _mul;
    const op opDiv = argc > 1000 ? _add : _div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
      data[i] = opAdd(90, opSub(12, opDiv(8, opMul(2, 2))));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "every fn ptr per iteration: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  
  
  for (int i = 0; i < dataSize; i++) {
    if (data[i] != 100) {
      cout << "incorrect data at " << i << ": " << data[i] << endl;;
      exit(1);
    }
    data[i] = 0;
  }
  
  //switch per iteration
  {
    const switch_op a = argc > 1000 ? so_sub : so_add;
    const switch_op s = argc > 1000 ? so_mul : so_sub;
    const switch_op m = argc > 1000 ? so_div : so_mul;
    const switch_op d = argc > 1000 ? so_add : so_div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {
      data[i] = everyFn(a, 90, everyFn(s, 12, everyFn(d, 8, everyFn(m, 2, 2))));
    }
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "switch per iteration: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  
  
  for (int i = 0; i < dataSize; i++) {
    if (data[i] != 100) {
      cout << "incorrect data at " << i << ": " << data[i] << endl;
      exit(1);
    }
    data[i] = 0;
  }
  
  
  //one fn ptr per iteration
  {
    const op opAdd = argc > 1000 ? _sub : _add;
    const op opSub = argc > 1000 ? _mul : _sub;
    const op opMul = argc > 1000 ? _div : _mul;
    const op opDiv = argc > 1000 ? _add : _div;
  	auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < dataSize; i++) {data[i] = opMul(2, 2);}
    for (int i = 0; i < dataSize; i++) {data[i] = opDiv(8, data[i]);}
    for (int i = 0; i < dataSize; i++) {data[i] = opSub(12, data[i]);}
    for (int i = 0; i < dataSize; i++) {data[i] = opAdd(90, data[i]);}
  	auto end_time = std::chrono::high_resolution_clock::now();
    cout << "one fn ptr per iteration: " <<
    std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time
    ).count() << " microseconds" << endl;
  }
  
  
  for (int i = 0; i < dataSize; i++) {
    if (data[i] != 100) {
      cout << "incorrect data at " << i << ": " << data[i] << endl;;
      exit(1);
    }
  }
  return 0;
}


