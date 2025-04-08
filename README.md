# Fresnel-diffraction
## General diffraction theory

The phenomenon of diffraction is well described by the Fresnel-Kirchhoff integral:

$U(x_0, y_0, z_0) = \iint_{\Omega} A_{\Sigma}  e^{i k \frac{(x - x_0)^2 + (y - y_0)^2}{2z_0}} dx dy$

Under the Fraunhofer approximation (an approximation where diffraction in the far-field zone is considered), the integrand expression can be conveniently expanded in a series when the distance to the screen is much larger than the characteristic size of the diffraction pattern.

Fresnel Integral for a Rectangular Aperture

For a rectangular aperture in the plane  with dimensions , the Fresnel diffraction integral at the observation point  is given by:

$U(x_0, y_0, z_0) = \frac{e^{i k z_0}}{i \lambda z_0} \iint_{-a}^{a} \int_{-b}^{b} e^{i k \frac{(x - x_0)^2 + (y - y_0)^2}{2z_0}} dx dy$

### Dimensionless Variables

Defining the dimensionless coordinates:

$X = \sqrt{\frac{k}{2 z_0}} (x - x_0), \quad Y = \sqrt{\frac{k}{2 z_0}} (y - y_0)$, 

the integral simplifies to:

$U(x_0, y_0, z_0) = \frac{e^{i k z_0}}{i \lambda z_0} \left[ \int_{-\alpha}^{\alpha} e^{i \pi X^2} dX \right] \left[ \int_{-\beta}^{\beta} e^{i \pi Y^2} dY \right]$,

where:

$\alpha = \sqrt{\frac{k}{2 z_0}} a $, $\quad \beta = \sqrt{\frac{k}{2 z_0}} b$.

Connection to Fresnel Integrals

Each integral can be expressed using the Fresnel integrals:

\int_{-\alpha}^{\alpha} e^{i \pi X^2} dX = \sqrt{\frac{i}{2}} \left[ C(\sqrt{2\alpha}) + i S(\sqrt{2\alpha}) - C(-\sqrt{2\alpha}) - i S(-\sqrt{2\alpha}) \right].

The final result for the amplitude in the Fresnel approximation is:

U(x_0, y_0, z_0) \propto \left( C(\sqrt{2\alpha}) + i S(\sqrt{2\alpha}) - C(-\sqrt{2\alpha}) - i S(-\sqrt{2\alpha}) \right) \times \left( C(\sqrt{2\beta}) + i S(\sqrt{2\beta}) - C(-\sqrt{2\beta}) - i S(-\sqrt{2\beta}) \right).

