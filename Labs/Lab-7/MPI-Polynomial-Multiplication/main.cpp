#include <iostream>
#include "Polynomial.h"
#include "PolynomialArithmetic.h"
#include <fstream>
#include <tuple>
#include <chrono>
#include "mpi.h"

#define POLYNOMIAL_PRINT_FLAG true

std::tuple<Polynomial, Polynomial> readPolynomials(std::string filename) {
    std::ifstream in(filename);
    std::deque<int> coefficientsOfTheFirstPolynomial, coefficientsOfTheSecondPolynomial;
    int polynomialSize, currentNumber;

    in >> polynomialSize;
    for (int i = 0; i < polynomialSize; i++) {
        in >> currentNumber;
        coefficientsOfTheFirstPolynomial.push_back(currentNumber);
    }

    in >> polynomialSize;
    for (int i = 0; i < polynomialSize; i++) {
        in >> currentNumber;
        coefficientsOfTheSecondPolynomial.push_back(currentNumber);
    }

    Polynomial firstPolynomial(coefficientsOfTheFirstPolynomial, polynomialSize - 1);
    Polynomial secondPolynomial(coefficientsOfTheSecondPolynomial, polynomialSize - 1);

    return std::make_tuple(firstPolynomial, secondPolynomial);
}

void regularMultiplication(Polynomial* firstPolynomial, Polynomial* secondPolynomial) {
    auto start = std::chrono::steady_clock::now();
    Polynomial regularPolynomialMultiplicationResult = PolynomialArithmetic::computeRegularPolynomialMultiplication(
            *firstPolynomial, *secondPolynomial);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Regular multiplication result:\n";
    regularPolynomialMultiplicationResult.print(POLYNOMIAL_PRINT_FLAG);
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";
}


void parallelizedRegularMultiplication(Polynomial* firstPolynomial, Polynomial* secondPolynomial) {
    auto start = std::chrono::steady_clock::now();
    Polynomial parallelRegularPolynomialMultiplicationResult = PolynomialArithmetic::computeParallelizedRegularPolynomialMultiplication(
            *firstPolynomial, *secondPolynomial);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Parallel regular multiplication result:\n";
    parallelRegularPolynomialMultiplicationResult.print(POLYNOMIAL_PRINT_FLAG);
    std::chrono::duration<double> elapsed_seconds = end-start;
    // 0.5 seconds were subtracted, because a thread sleep is used
    std::cout << "Elapsed time: " << elapsed_seconds.count() - 0.5  << " seconds\n";
}

void karatsubaMultiplication(Polynomial* firstPolynomial, Polynomial* secondPolynomial) {
    auto start = std::chrono::steady_clock::now();
    Polynomial karatsubaPolynomialMultiplicationResult = PolynomialArithmetic::computeKaratsubaPolynomialMultiplication(
            *firstPolynomial, *secondPolynomial);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Karatsuba multiplication result:\n";
    karatsubaPolynomialMultiplicationResult.print(POLYNOMIAL_PRINT_FLAG);
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";
}

void parallelizedKaratsubaMultiplication(Polynomial* firstPolynomial, Polynomial* secondPolynomial) {
    auto start = std::chrono::steady_clock::now();
    Polynomial parallelKaratsubaPolynomialMultiplicationResult = PolynomialArithmetic::computeParallelizedKaratsubaPolynomialMultiplication(
            *firstPolynomial, *secondPolynomial, 4);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Parallel Karatsuba multiplication result:\n";
    parallelKaratsubaPolynomialMultiplicationResult.print(POLYNOMIAL_PRINT_FLAG);
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Elapsed time: " << elapsed_seconds.count() << " seconds\n";
}

void runMasterAndWorker() {
}


int main(int argc, char* argv[]) {
    // CLion only reads files from the debug directory, hence the ../ expression used in the filepath
    auto [firstPolynomial, secondPolynomial] = readPolynomials("../polynomials.txt");

    firstPolynomial.print(POLYNOMIAL_PRINT_FLAG);
    secondPolynomial.print(POLYNOMIAL_PRINT_FLAG);

    int rank, size, len;
    char version[MPI_MAX_LIBRARY_VERSION_STRING];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_library_version(version, &len);
    printf("Hello, world, I am %d of %d, (%s, %d)\n",
           rank, size, version, len);
    MPI_Finalize();

    return 0;
}