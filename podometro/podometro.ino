


//first calibrate on powering up the module 
//a poscal function to calculate avg values of x , y , z and print them on serial
//turn on the LED on pin13 and turn it off after the calibration is completed


//in loop continually read the data and calculate the steps

#include <SoftwareSerial.h>
SoftwareSerial s(5,6); 

const int xpin=A2;
int ypin=A3;
int zpin=A4;

int powerpin=A0;

float threshhold=104.0;

float xval[100]={0};
float yval[100]={0};
float zval[100]={0};
float xavg;
  float yavg;
  float zavg;
  
int flag=0;
int steps = 0;
void setup()
{
Serial.begin(115200);
s.begin(9600);
pinMode(powerpin,OUTPUT);
digitalWrite(powerpin,HIGH);
pinMode(13,OUTPUT);
calibrate();
}



void loop()
{int acc=0;
  float totvect[100]={0};
float totave[100]={0};
 //float sum1,sum2,sum3=0;
  float xaccl[100]={0};
   float yaccl[100]={0};
    float zaccl[100]={0};
   // float x,y,z;
   
  
for (int i=0;i<100;i++)
{
xaccl[i]=float(analogRead(xpin));
delay(1);

yaccl[i]=float(analogRead(ypin));
delay(1);

zaccl[i]=float(analogRead(zpin));
delay(1);

//transform the data from the three axis 
 totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
 totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
 Serial.println(totave[i]);
 delay(200);

//if the new data from the axis it's bigger than the threshhold it adds a step to the variable 
if (totave[i]>threshhold && flag==0)
{
  steps=steps+1;
  flag=1;
 
}
 else if (totave[i] > threshhold && flag==1)
{
//do nothing 
}
  if (totave[i] <threshhold  && flag==1)
  {flag=0;}
  Serial.println('\n');
  Serial.print("steps=");
  Serial.println(steps); //used to check the values to get the correct threshhold
  s.write(steps);//send the steps to the nodeMCU through serial communication
  }

delay(1000);
 
}

void calibrate()//function used to calibrate the ACC

{

  digitalWrite(13,HIGH);
  
  float sum=0;
  float sum1=0;
  float sum2=0;
for (int i=0;i<100;i++)
{
xval[i]=float(analogRead(xpin));

sum=xval[i]+sum;
}
delay(100);
xavg=sum/100.0;

Serial.println(xavg);

for (int j=0;j<100;j++)
{
xval[j]=float(analogRead(xpin));

sum1=xval[j]+sum1;
}
yavg=sum1/100.0;

Serial.println(yavg);
delay(100);
for (int i=0;i<100;i++)
{
zval[i]=float(analogRead(zpin));

sum2=zval[i]+sum2;
}
zavg=sum2/100.0;
delay(100);
Serial.println(zavg);

digitalWrite(13,LOW);

}

  
//counting the pushups
