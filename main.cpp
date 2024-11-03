#include <iostream> 
#include <cmath> 

void MoveFromStartToObject(double L1, double L2, double x, double y, double& theta1, double& theta2)//------передача в функцию аргументов
{
	double r = sqrt(x * x + y * y); //--------r диагональ. Рассчитывается по теореме Пифагора

	if (r > L1 + L2)//-----проверка на достижимость. Если диагональ будет больше чем сумма катетов, то значения углов станут 0(начальное положение манипулятора)
	{
		std::cout << "Error, point is out of the reach :(" << std::endl;
		theta1 = theta2 = 0;
		return;
	}

	else if (r < std::abs(L1 - L2))//-----------если диагональ будет меньше чем модуль разности, то значения углов также станут 0
	{
		std::cout << "Error, point is too close :(" << std::endl;
		theta1 = theta2 = 0;
		return;
	}

	else
	{
		double cosTheta2 = cos(L1 * L1 + L2 * L2 - r * r / 2 * L1 * L2); //--------расчет косинуса угла№ 2

		if (cosTheta2 < -1 || cosTheta2 > 1)//-------проверка на значение косинуса. В случае другого значения косинуса, не входящего в диапозон -1 до 1, значения улов станут 0
		{
			std::cout << "Error in angle calculating :(" << std::endl;
			theta1 = theta2 = 0;
			return;
		}

		theta2 = acos(cosTheta2);//------расчет арккосинуса
		theta1 = atan2(y, x) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));//-------расчет угла№ 1
	}
}

int main()
{
	double L1, L2, x, y, theta1, theta2; //---------инициализация переменных

	std::cout << "Enter L1, L2: " << std::endl; //-------ввод длин звеньев манипулятора
	std::cin >> L1;
	std::cin >> L2;

	std::cout << "Enter x, y: " << std::endl; //-------ввод координат
	std::cin >> x;
	std::cin >> y;

	MoveFromStartToObject(L1, L2, x, y, theta1, theta2); //-------вызов функции

	std::cout << "Angle Theta1:\t" << theta1 << std::endl; //--------вывод полученных значений углов
	std::cout << "Angle Theta2:\t" << theta2 << std::endl;
	return 0;
}

