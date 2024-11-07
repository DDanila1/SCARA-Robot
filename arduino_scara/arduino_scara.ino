#include <AccelStepper.h>

#define STEP_PIN_1 3
#define DIR_PIN_1 4
#define STEP_PIN_2 5
#define DIR_PIN_2 5
#define ENABLE_PIN_1 7
#define ENABLE_PIN_2 7

AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_1, DIR_PIN_1);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_2, DIR_PIN_2);

const double L1 = 245;//-----------------передаю константные значения длин звеньев
const double L2 = 234526;

//-----------------------------------------ФУНКЦИЯ ОПРЕДЕЛЕНИЯ УГЛОВ
void MoveFromStartToBoltFeeder(double x, double y, double& theta1, double& theta2)//------передача в функцию аргументов
{
	double r = sqrt(x * x + y * y); //--------r диагональ. Рассчитывается по теореме Пифагора

	if (r > L1 + L2)//-----проверка на достижимость. Если диагональ будет больше чем сумма катетов, то значения углов станут 0(начальное положение манипулятора)
	{
		//std::cout << "Error, point is out of the reach :(" << std::endl;
    Serial.print("Error, point is out of the reach :(");
		theta1 = theta2 = 0;
		return;
	}

	else if (r < abs(L1 - L2))//-----------если диагональ будет меньше чем модуль разности, то значения углов также станут 0
	{
		//std::cout << "Error, point is too close :(" << std::endl;
    Serial.print("Error, point is too close :(");
		theta1 = theta2 = 0;
		return;
	}

	else
	{
		double cosTheta2 = cos(L1 * L1 + L2 * L2 - r * r / 2 * L1 * L2); //--------расчет косинуса угла№ 2

		if (cosTheta2 < -1 || cosTheta2 > 1)//-------проверка на значение косинуса. В случае другого значения косинуса, не входящего в диапозон -1 до 1, значения улов станут 0
		{
			//std::cout << "Error in angle calculating :(" << std::endl;
      Serial.print("Error in angle calculating :(");
			theta1 = theta2 = 0;
			return;
		}

		theta2 = acos(cosTheta2);//------расчет арккосинуса
		theta1 = atan2(y, x) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));//-------расчет угла№ 1

    theta1 = theta1*180.0/PI;//-------------------перевод в градусы
    theta2 = theta2*180.0/PI;
	}
}

int AngleToSteps(double angle)//---------------------функция для перевода углов в шаги двигателя
{
  const int StepsPerRevolution = 200;//--------константа шагов за полный оборот двигателя
  return (int)(angle / 360.0 * StepsPerRevolution);
}

void setup()
{
  Serial.begin(9600);

  pinMode(ENABLE_PIN_1, OUTPUT);//-----пин, в который подключен драйвер, используется как выходной
  pinMode(ENABLE_PIN_2, OUTPUT);
  digitalWrite(ENABLE_PIN_1, HIGH);//---------первый мотор выключен
  digitalWrite(ENABLE_PIN_2, HIGH);//---------второй мотор выключен

  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);
}

void loop()
{
  double point_X = 12341235;
  double point_Y = 125125;

  double theta1, theta2;
  MoveFromStartToBoltFeeder(point_X, point_Y, theta1, theta2);//----------вызов функции расчета углов

  Serial.print("Theta1: "); //--------вывод значений
  Serial.print(theta1);

  Serial.print("Theta2: ");
  Serial.print(theta2);

  int steps_1 = AngleToSteps(theta1);//----------расчет углов в градусах
  int steps_2 = AngleToSteps(theta2);

  digitalWrite(ENABLE_PIN_1, LOW);//--------включаем первый мотор
  stepper1.moveTo(steps_1);//---------первый мотор движется
  
  while (stepper1.distanceToGo() != 0)
  {
      stepper1.run();//----------------пока шаги мотора не равны 0 то выполняется код движения
  }

  digitalWrite(ENABLE_PIN_1, HIGH);//----------после цикла движения выключаем первый мотор

  digitalWrite(ENABLE_PIN_2, LOW);//-----включаем второй мотор
  stepper2.moveTo(steps_2);//---------второй мотор движется

  while (stepper2.distanceToGo() != 0)
  {
      stepper2.run();//----------------пока шаги мотора не равны 0 то выполняется код движения
  }

  digitalWrite(ENABLE_PIN_2, HIGH);//----------после цикла движения выключаем второй мотор

  delay(1000); // Задержка между циклами
}