#include "FunctionExtraction.hpp"
#include "GaussianQuadrature.hpp"
#include "QuadratureFormula.hpp"

#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>

std::complex<double> f(const double x, const double x0, const double z0, const double lambda) {
    constexpr double pi = std::numbers::pi_v<double>;
    const double k = 2 * pi / lambda;

    return std::exp(std::complex<double>(0, pi * k * (x - x0) * (x - x0) / (2 * z0)));
}

int main() {
    const double lambda = 1e-6;
    const double alpha = 0.5e-3;
    const double betta = 0.5e-3;
    const double U0 = 1;
    const double z0 = 1;
    const double x0 = 0;
    const double y0 = 0;
    const int N = 50;
    const double size_image = 10 * lambda / (2 * alpha) * z0;
    const double step = size_image / N;
    const auto callable = [lambda, x0, z0](const double x) { return f(x, x0, z0, lambda); };

    const double h = 0.0000001;
    const auto I_x_1 = DefiniteIntegrals::integrate<DefiniteIntegrals::Quadrature<8> >(callable, {-alpha}, {alpha}, {h});
    const auto I_x_2 = DefiniteIntegrals::integrate<DefiniteIntegrals::Quadrature<8> >(callable, {-betta}, {betta}, {h/2});

    for (int i = -N; i <= N; ++i) {

    }

    std::cout << std::setprecision(15);
    std::cout << I_x_1 << std::endl;
    std::cout << I_x_2 << std::endl;
}
