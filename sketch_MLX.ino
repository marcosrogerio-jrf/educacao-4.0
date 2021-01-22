
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Define o endereco I2C do display e qtde de colunas e linhas
LiquidCrystal_I2C lcd(0x27,16,2);

//Array que desenha o simbolo de grau
byte grau[8] = {B00110, B01001, B01001, B00110,
                B00000, B00000, B00000, B00000,};

double temp_amb;
double temp_obj;

//==========================
// INFRAVERMELHO
const int pinoLed = 9; //Pino digital utilizado pelo LED
const int pinoSensor = 8; //Pino digital utilizado pelo Sensor Infravermelho

//==========================
// RELE
const int pinoRele = 10; //Pino digital utilizado pelo RELE

void setup() {
  Serial.begin(9600);
  Serial.println("Sensor de temperatura MLX90614");
  
  //Inicialização Infravermelho
  pinMode(pinoSensor, INPUT); //Define o pino como entrada
  pinMode(pinoLed, OUTPUT); //Define o pino como saída
  digitalWrite(pinoLed, LOW); //LED inicia desligado

  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, LOW);

  //Inicializa o display LCD I2C
  lcd.init();
  lcd.setBacklight(HIGH);

  //Atribui a "1" o valor do array "grau", que desenha o simbolo de grau
  lcd.createChar(1, grau);

  //Inicializa o MLX90614
  mlx.begin();
}

void loop() {
  //Leitura da temperatura ambiente e do objeto
  //(para leitura dos valores em Fahrenheit, utilize
  //mlx.readAmbientTempF() e mlx.readObjectTempF() )
  lcd.clear();
  temp_amb = mlx.readAmbientTempC();
  temp_obj = mlx.readObjectTempC();

  //sensorProximidade();
  if(digitalRead(pinoSensor) == LOW){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      digitalWrite(pinoLed, HIGH); //ACENDE O 
      leituraDaTemperatura();
      digitalWrite(pinoRele, HIGH);
      delay(1000);
      digitalWrite(pinoRele, LOW);
  } else { //SENÃO, FAZ
    digitalWrite(pinoLed, LOW); //APAGA O LED
    lcd.setCursor(6, 0);
    lcd.print("IFMS");
    //lcd.setCursor(0, 1);
    //lcd.print("");
  }
  
  //Aguarda 1 segundo ate nova leitura
  delay(1000);
}

void sensorProximidade(){
  if(digitalRead(pinoSensor) == LOW){ //SE A LEITURA DO PINO FOR IGUAL A LOW, FAZ
      digitalWrite(pinoLed, HIGH); //ACENDE O LED
  } else { //SENÃO, FAZ
    digitalWrite(pinoLed, LOW); //APAGA O LED
  }
}

void leituraDaTemperatura(){
  lcd.setCursor(0, 0);
  lcd.print("Ambiente:");
  lcd.setCursor(10, 0);
  lcd.print(temp_amb);
  lcd.setCursor(15, 0);
  lcd.write(1);
  lcd.setCursor(0, 1);
  lcd.print("Aluno:");
  lcd.setCursor(10, 1);
  lcd.print(temp_obj);
  lcd.setCursor(15, 1);
  lcd.write(1);

  //Mostra as informacoes no Serial Monitor
  Serial.print("Ambiente = ");
  Serial.print(temp_amb);
  Serial.print("*CtObjeto = ");
  Serial.print(temp_obj); Serial.println("*C");
  delay(2000);
}
