#include <iostream> 
#include <cmath> 

const double L1 = 15;
const double L2 = 15;

void MoveFromStartToBoltFeeder(double x, double y, double& theta1, double& theta2)//------передача в функцию аргументов
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

void MoveFromBoltFeederToObject(double obj_x, double obj_y, double& new_theta1, double& new_theta2)
{
	double r = sqrt(obj_x * obj_x + obj_y * obj_y);

	if (r > L1 + L2)
	{
		std::cout << "Error, point is out of the reach :(" << std::endl;
		new_theta1 = new_theta2 = 0;
		return;
	}

	else if (r < std::abs(L1 - L2))
	{
		std::cout << "Error, point is too close :(" << std::endl;
		new_theta1 = new_theta2 = 0;
		return;
	}

	else
	{
		double new_cosTheta2 = cos(L1 * L1 + L2 * L2 - r * r / 2 * L1 * L2);

		if (new_cosTheta2 < -1 || new_cosTheta2 > 1)
		{
			std::cout << "Error in angle calculating :(" << std::endl;
			new_theta1 = new_theta2 = 0;
			return;
		}

		new_theta2 = acos(new_cosTheta2);
		new_theta1 = atan2(obj_y, obj_x) - atan2(L2 * sin(new_theta2), L1 + L2 * cos(new_theta2));
	}
}

int AngleToSteps(double angle)//---------------------функция для перевода углов в шаги двигателя
{
	const int StepsPerRevolution = 200;//--------константа шагов за полный оборот двигателя
	return (int)(angle / 360.0 * StepsPerRevolution);
}

int main()
{
	double x, y, theta1, theta2; //---------инициализация переменных
	double obj_x, obj_y, new_theta1, new_theta2;

	//std::cout << "Enter L1, L2: " << std::endl; //-------ввод длин звеньев манипулятора
	//std::cin >> L1;
	//std::cin >> L2;

	std::cout << "MoveFromStartToBoltFeeder. Enter x, y: " << std::endl; //-------ввод координат
	std::cin >> x;
	std::cin >> y;

	std::cout << "MoveFromBoltFeederToObject. Enter obj_x, obj_y: " << std::endl; //-------ввод координат
	std::cin >> obj_x;
	std::cin >> obj_y;

	MoveFromStartToBoltFeeder(x, y, theta1, theta2); //-------вызов функции

	std::cout << "Angle Theta1:\t" << theta1 << std::endl; //--------вывод полученных значений углов
	std::cout << "Angle Theta2:\t" << theta2 << std::endl;

	std::cout << "----------------------------------------------------------------------" << std::endl;//--------разделитель

	MoveFromBoltFeederToObject(obj_x, obj_y, new_theta1, new_theta2);//-------вызов функции

	std::cout << "Angle new_Theta1:\t" << new_theta1 << std::endl; //--------вывод полученных значений углов
	std::cout << "Angle new_Theta2:\t" << new_theta2 << std::endl;

	return 0;
}