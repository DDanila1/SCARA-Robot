#include <AccelStepper.h>

#define STEP_PIN_1 3
#define DIR_PIN_1 4
#define STEP_PIN_2 5
#define DIR_PIN_2 6
#define ENABLE_PIN_1 7
#define ENABLE_PIN_2 8

AccelStepper stepper1(AccelStepper::DRIVER, STEP_PIN_1, DIR_PIN_1);
AccelStepper stepper2(AccelStepper::DRIVER, STEP_PIN_2, DIR_PIN_2);

const double L1 = 15;  //-----------------передаю константные значения длин звеньев
const double L2 = 15;

//-----------------------------------------ФУНКЦИЯ ОПРЕДЕЛЕНИЯ УГЛОВ
void FromStartToBoltFeederAngle(double x, double y, double& theta1, double& theta2)  //------передача в функцию аргументов
{
  double r = sqrt(x * x + y * y);  //--------r диагональ. Рассчитывается по теореме Пифагора

  if (r > L1 + L2)  //-----проверка на достижимость. Если диагональ будет больше чем сумма катетов, то значения углов станут 0(начальное положение манипулятора)
  {
    Serial.print("Error, point is out of the reach :(");
    theta1 = theta2 = 0;
    return;
  }

  else if (r < abs(L1 - L2))  //-----------если диагональ будет меньше чем модуль разности, то значения углов также станут 0
  {
    Serial.print("Error, point is too close :(");
    theta1 = theta2 = 0;
    return;
  }

  else {
    double cosTheta2 = cos((L1 * L1 + L2 * L2 - r * r) / (2 * L1 * L2));  //--------расчет косинуса угла№ 2
    if (cosTheta2 < -1 || cosTheta2 > 1)  //-------проверка на значение косинуса. В случае другого значения косинуса, не входящего в диапозон -1 до 1, значения улов станут 0
    {
      Serial.print("Error in angle calculating :(");
      theta1 = theta2 = 0;
      return;
    }

    theta2 = acos(cosTheta2);                                               //-------расчет арккосинуса
    theta1 = atan2(y, x) - atan2(L2 * sin(theta2), L1 + L2 * cos(theta2));  //-------расчет угла№ 1

    theta1 = theta1 * 180.0 / PI;  //-------------------перевод в градусы
    theta2 = theta2 * 180.0 / PI;
  }
}

void FromBoltFeederToObjectAngle(double obj_x, double obj_y, double& new_theta1, double& new_theta2) {
  double r = sqrt(obj_x * obj_x + obj_y * obj_y);

  if (r > L1 + L2) {
    new_theta1 = new_theta2 = 0;
    return;
  }

  else if (r < abs(L1 - L2)) {
    new_theta1 = new_theta2 = 0;
    return;
  }

  else {
    double new_cosTheta2 = cos((L1 * L1 + L2 * L2 - r * r) / (2 * L1 * L2));
    if (new_cosTheta2 < -1 || new_cosTheta2 > 1) {
      new_theta1 = new_theta2 = 0;
      return;
    }

    new_theta2 = acos(new_cosTheta2);
    new_theta1 = atan2(obj_y, obj_x) - atan2(L2 * sin(new_theta2), L1 + L2 * cos(new_theta2));
  }
}

int AngleToSteps(double angle)  //---------------------функция для перевода углов в шаги двигателя
{
  const int StepsPerRevolution = 200;  //--------константа шагов за полный оборот двигателя
  return (int)(angle / 360.0 * StepsPerRevolution);
}

void MoveFromStartToBoltFeeder(double steps_1, double steps_2) 
{
  stepper1.moveTo(steps_1);         //---------устанавливаем целевую позицию (до конвейера)
  stepper2.moveTo(steps_2);
  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) 
  {
    stepper1.run();  //----------------пока шаги мотора не равны 0 то выполняется движение
    stepper2.run();
  }

  digitalWrite(ENABLE_PIN_1, HIGH);  //----------после цикла движения выключаем первый мотор
  digitalWrite(ENABLE_PIN_2, HIGH);  //----------после цикла движения выключаем второй мотор
}

void MoveFromBoltFeederToObject(double sum_theta1, double sum_theta2) 
{
  digitalWrite(ENABLE_PIN_1, LOW);  //---------включаем первый мотор
  digitalWrite(ENABLE_PIN_2, LOW);  //---------включаем второй мотор

  stepper1.moveTo(sum_theta1);         //---------устанавливаем целевую позицию (от конвейера до детали)
  stepper2.moveTo(sum_theta2);
  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) 
  {
    stepper1.run();  //----------------пока шаги мотора не равны 0 то выполняется движение
    stepper2.run();
  }

  digitalWrite(ENABLE_PIN_1, HIGH);  //----------после цикла движения выключаем первый мотор
  digitalWrite(ENABLE_PIN_2, HIGH);  //----------после цикла движения выключаем второй мотор
}

void MoveFromObjectToBoltFeeder(double sum_theta1, double sum_theta2) 
{
  digitalWrite(ENABLE_PIN_1, LOW);  //---------включаем первый мотор
  digitalWrite(ENABLE_PIN_2, LOW);

  stepper1.moveTo(-sum_theta1);         //---------устанавливаем целевую позицию (от детали до конвейера)
  stepper2.moveTo(-sum_theta2);
  while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) 
  {
    stepper1.run();  //----------------пока шаги мотора не равны 0 то выполняется движение
    stepper2.run();
  }

  digitalWrite(ENABLE_PIN_1, HIGH);  //----------после цикла движения выключаем первый мотор
  digitalWrite(ENABLE_PIN_2, HIGH);  //----------после цикла движения выключаем второй мотор
}

void Lowering()
{
  //------------------------ФУНКЦИЯ ОПУСКАНИЯ
}

void Twisting()
{
  //------------------------ФУНКЦИЯ ЗАКРУЧИВАНИЯ
}

void Lifting()
{
  //------------------------ФУНКЦИЯ ПОДЪЕМА
}

void setup() {
  Serial.begin(9600);

  pinMode(ENABLE_PIN_1, OUTPUT);  //-----пин, в который подключен драйвер, используется как выходной
  pinMode(ENABLE_PIN_2, OUTPUT);
  digitalWrite(ENABLE_PIN_1, LOW);  //---------первый мотор включен
  digitalWrite(ENABLE_PIN_2, LOW);  //---------второй мотор включен

  stepper1.setMaxSpeed(100);
  stepper2.setMaxSpeed(100);
}

void loop() {
  double point_X = 25, point_Y = 15, obj_x = 23, obj_y = 31;  //-------УСТАНОВИТЬ ЗНАЧЕНИЯ КООРДИНАТ!!!
  double theta1, theta2, new_theta1, new_theta2;

  FromStartToBoltFeederAngle(point_X, point_Y, theta1, theta2);       //----------вызов функции расчета углов для движения к конвейеру подачи болтов
  FromBoltFeederToObjectAngle(obj_x, obj_y, new_theta1, new_theta2);  //----------вызов функции расчета углов для движения к объекту(заготовка)

  Serial.print("Theta1: ");  //--------вывод значений
  Serial.print(theta1);

  Serial.print("Theta2: ");
  Serial.print(theta2);

  Serial.print("new_theta1: ");  //--------вывод значений
  Serial.print(new_theta1);

  Serial.print("new_theta2: ");
  Serial.print(new_theta2);

  int steps_1 = AngleToSteps(theta1);  //----------расчет углов для движения к конвейеру в градусах
  int steps_2 = AngleToSteps(theta2);

  int new_steps_1 = AngleToSteps(new_theta1);  //----------расчет углов для движения к объекту(заготовка)
  int new_steps_2 = AngleToSteps(new_theta2);

  int sum_theta1 = steps_1 + new_steps_1;  //---------общий угол для движения от конвейера к объекту(заготовка)
  int sum_theta2 = steps_2 + new_steps_2;

  for (int i = 0; i < 10; i++)
  {
    MoveFromStartToBoltFeeder(steps_1, steps_2);
    MoveFromBoltFeederToObject(sum_theta1, sum_theta2);
    MoveFromObjectToBoltFeeder(sum_theta1, sum_theta2);
    Lowering();
    Twisting();
    Lifting();
  }

  delay(1000);  //-------задержка между циклами
}