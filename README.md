# Fresnel-Kirchhoff diffraction
## General diffraction theory

The phenomenon of diffraction is well described by the [Fresnel-Kirchhoff integral](https://en.m.wikipedia.org/wiki/Kirchhoff%27s_diffraction_formula):

$U(\rho) = \iint_{\Omega} A_{\Sigma}  \frac{e^{i k \rho}}{\rho} K(\varphi) d\sigma$

We can divide the phenomenon of diffraction for two main types - Fresnel diffraction and Fraunhofer diffraction via using Fresnel parameter:
$n = \frac{a^2}{\lambda z_0}$ 
where a is the characteristic size of the aperture, $\lambda$ is a wavelength and $z_0$ is distance to the screen.

- $n >= 1$ => we have can simplify Fresnel-Kirchhoff integral via Fraunhofer approximation
- $n << 1$ => we have can simplify Fresnel-Kirchhoff integral via Fresnel approximation

## Fraunhofer approximation
_Under the Fraunhofer approximation pepe pe pepe ._

### Fraunhofer Integral for a Rectangular Aperture
For a rectangular aperture in the plane with dimensions, the Fraunhofer diffraction integral at the observation point is given by:

$U(x_0, y_0, z_0) = U_0 \frac{e^{ikz_0}}{i \lambda z_0} \left(\int_{-a/2}^{a/2} e^{ik \frac{x x_0}{z_0}} dx \right) \cdot \left(\int_{-b/2}^{b/2} e^{ik \frac{y y_0}{z_0}} dy \right)$

This integral can easily be calculated analytically:

$U(x_0, y_0, z_0) = U_0 \frac{4}{k^2 \lambda} \left( sinc(\frac{kax_0}{2z_0}) \cdot sinc(\frac{kay_0}{2z_0}) \right)$

## Fresnel approximation
_Under the Fresnel approximation the integrand expression can be conveniently expanded in a series when the distance to the screen is much larger than the characteristic size of the diffraction pattern._

### Fresnel Integral for a Rectangular Aperture

For a rectangular aperture in the plane with dimensions, the Fresnel diffraction integral at the observation point is given by:

$U(x_0, y_0, z_0) = U_0 \frac{e^{i k z_0}}{i \lambda z_0} \int_{-a}^{a} \int_{-b}^{b} e^{i k \frac{(x - x_0)^2 + (y - y_0)^2}{2z_0}} dx dy$

Defining the dimensionless coordinates:

$X = \sqrt{\frac{k}{2 z_0}} (x - x_0), \quad Y = \sqrt{\frac{k}{2 z_0}} (y - y_0)$, 

the integral simplifies to:

$U(x_0, y_0, z_0) = U_0 \frac{e^{i k z_0}}{i \lambda z_0} \left[ \int_{-\alpha}^{\alpha} e^{i \pi X^2} dX \right] \left[ \int_{-\beta}^{\beta} e^{i \pi Y^2} dY \right]$,

where:

$\alpha = \sqrt{\frac{k}{2 z_0}} a $, $\quad \beta = \sqrt{\frac{k}{2 z_0}} b$.

This integral cannot be calculated analytically, we have to use numerical methods. The integral function is rapidly oscillating, and it is convenient to bring such integrals to the solution of a system of linear algebraic equations by the [Levin collocation method](https://www.iccs-meeting.org/archive/iccs2020/papers/121380029.pdf).

## Levin's Collocation Method for Integrating Rapidly Oscillating Functions

Levin's collocation method is a powerful numerical technique designed to efficiently integrate functions that exhibit rapid oscillations.

### Overview

The main idea behind Levin's method is to approximate the integral of a rapidly oscillating function using a series expansion and then applying collocation points to achieve high accuracy. This approach involves the following key steps:

1. **Function Representation**: The function ( f(x) ) is expressed in terms of a series expansion using Chebyshev polynomial.

2. **Collocation Points**: Select specific points (collocation points) within the integration interval where the approximation will match the original function.

3. **System of Equations**: Formulate a system of linear equations based on the collocation conditions. This system relates the coefficients in the series expansion to the values of the function at the collocation points.

4. **Solving for Coefficients**: Solve the resulting linear system to obtain the coefficients that best approximate the integral of the rapidly oscillating function. We are going to use [GMRES](https://en.wikipedia.org/wiki/Generalized_minimal_residual_method) to solve the system.

5. **Final Integration**: Use the obtained coefficients to compute the integral, often leading to significantly improved accuracy compared to standard methods.

### Deeper into details
