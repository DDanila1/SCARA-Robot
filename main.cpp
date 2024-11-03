#include <iostream> 
#include <cmath> 

int MoveFromStartToObject(double L1, double L2, double x, double y)
{
	double r = sqrt(x * x + y * y);

	if (r > L1 + L2)
	{
		std::cout << "Error, point is out of the reach :(" << std::endl;
		return 0;
	}
	else
	{
		double theta2 = acos(L1 * L1 + L2 * L2 - r * r / 2 * L1 * L2);
		double theta1 = atan2(y, x) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));
		return theta1, theta2;
	}
}

int main()
{
	double L1, L2, x, y;

	std::cout << "Enter L1, L2: " << std::endl;
	std::cin >> L1;
	std::cin >> L2;

	std::cout << "Enter x, y: " << std::endl;
	std::cin >> x;
	std::cin >> y;
	

}