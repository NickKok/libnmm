/******************************************************************************/
/* rungeKutta4()
 * 
 * jjhaag AT dreamincode.net
 * 14 November 2007
 * 
 * 
 * 4-step (fourth order) Runge-Kutta numerical integrator for ordinary differential equations.
 * 
 * A common and powerful numerical integrator.  No adaptive step size; such 
 * functionality can be controlled using an external function.
 * 
 * Arguments:
 * 		x	-	the initial x-value
 * 		yi	-	vector of the initial y-values
 * 		dx	-	the step size for the integration
 * 		derivatives		-	a pointer to a function that returns the derivative 
 * 				of a function at a point (x,y), where y is an STL vector of 
 * 				elements.  Returns a vector of the same size as y.
 * 
 * If you're looking for details on the Runge-Kutta method(s), do a google 
 * search - that would take up pages here, and that's probably a little more 
 * than belongs in the header comments.
 * 
 * Dependencies: requires <vector> (plus whatever is required by the derivatives
 * function and the rest of your program).
 * 
 * 
 * 
 * Example usage - numerical integration dy/dt = cos(x) :
 * #include <vector>
 * #include <cmath>
 * std::vector<double> dydtcos(double x, vector<double> y) {
 *     std::vector<double> dydt(y)
 *     dydt.at(0)=cos(x);
 *     return dydt;
 * }
 * --------------
 * vector<double> yi(1,0.0);
 * double x=0.0, dx=0.10;
 * 
 * while (x<10.0) {
 *     yi=rungeKutta4(yi,x,dx,(*dydtcos));
 *     x+=dx;
 * }
 * 
 */

#ifndef EFX_RUNGEKUTTA_H
#define EFX_RUNGEKUTTA_H

#include <vector>

inline std::vector<double> rungeKutta4(
		double xi,
		double dx,
		std::vector<double> yi,
		std::vector<double> param,
		std::vector<double> input,
		std::vector<double> (*derivatives)(double x, std::vector<double> y, std::vector<double> param, std::vector<double> input) ) {
	
	//	total number of elements in the vector
	static int n;
    static std::vector<double> k1(10,0.0);
    static std::vector<double> k2(10,0.0);
    static std::vector<double> k3(10,0.0);
    static std::vector<double> k4(10,0.0);
    
    n=yi.size();
	
	//	first step
	k1=derivatives(xi, yi, param, input);
	for (int i=0; i<n; ++i) {
		k1.at(i)*=dx;
	}
	
	//	second step
	for (int i=0; i<n; ++i) {
		k2.at(i)=yi.at(i)+k1.at(i)/2.0;
	}
	k2=derivatives(xi+dx/2.0,k2, param, input);
	for (int i=0; i<n; ++i) {
		k2.at(i)*=dx;
	}
	
	//	third step
	for (int i=0; i<n; ++i) {
		k3.at(i)=yi.at(i)+k2.at(i)/2.0;
	}
	k3=derivatives(xi+dx/2.0,k3, param, input);
	for (int i=0; i<n; ++i) {
		k3.at(i)*=dx;
	}
	
	
	//	fourth step
	for (int i=0; i<n; ++i) {
		k4.at(i)=yi.at(i)+k3.at(i);
	}
	k4=derivatives(xi+dx,k4, param, input);
	for (int i=0; i<n; ++i) {
		k4.at(i)*=dx;
	}
	
	
	//	sum the weighted steps into yf and return the final y values
	for (int i=0; i<n; ++i) {
		yi.at(i)+=(k1.at(i)/6.0)+(k2.at(i)/3.0)+(k3.at(i)/3.0)+(k4.at(i)/6.0);
	}
	
	return yi;
}

inline std::vector<double> rungeKutta4(
	double xi,
	double dx,
	std::vector<double> yi,
	std::vector<double> input,
	std::vector<double> (*derivatives)(double x, std::vector<double> y, std::vector<double> param, std::vector<double> input) ) {
		static std::vector<double> param(1,0.0);
		return rungeKutta4(xi, dx, yi, param, input, derivatives);
}

#endif /* __EXP_RUNGEKUTTA_H__ */