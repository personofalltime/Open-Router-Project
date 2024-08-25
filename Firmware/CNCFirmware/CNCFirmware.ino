#define MAX_BUFFER (64)
#define PI (3.14159265)
#define SMALLINCREMENT (0.001)
#define PERCENTAGERATE (1.0)

char buffer[MAX_BUFFER];
int bufferfill;


class Axis{
  int stepper1s = NULL;
  int stepper1d = NULL;
  int stepper2s = NULL;
  int stepper2d = NULL;

  int homepin = NULL;

  int steps_per_unit;

  int current_steps;

  public:
  
    Axis(int s1, int d1, int s2, int d2, int spu ){
      stepper1s = s1;
      stepper1d = d1;
      stepper2s = s2;
      stepper2d = d2;

      steps_per_unit = spu;

      current_steps = 0;
    }


    int* calc_line_steps(int val_mm){

      int diff = val_mm*steps_per_unit;
      int difference = (diff == current_steps) ? 0 : (diff-current_steps);

      int dir = (difference < 0) ? 0 : 1;
      int step = (difference < 0) ? -difference : difference;
      int out[2] = {dir, step};
      return out;
    } //POSSIBLY USELESS??

    void move(int steps[2]){  //Moves <double> mm, in <int> direction
      digitalWrite(stepper1d, steps[0]);
      int stepincrement = steps[1]/steps_per_unit;
      if(stepper2s == NULL){
        for(int i = 0; i < stepincrement; i++){
          digitalWrite(stepper1s, HIGH);
          delayMicroseconds(1000);
          digitalWrite(stepper1s, LOW);
          delayMicroseconds(1000);
          delayMicroseconds(PERCENTAGERATE*1000);

          if(steps[0] == 0){
            current_steps += 1;
          }
          else{
            current_steps -= 1;
          }
        }
      }
      else{
        digitalWrite(stepper2d, steps[0]);
        for(int i = 0; i < stepincrement; i++){
          digitalWrite(stepper1s, HIGH);
          delayMicroseconds(1000);
          digitalWrite(stepper1s, LOW);
          digitalWrite(stepper2s, HIGH);
          delayMicroseconds(1000);
          digitalWrite(stepper2s, LOW);
          delayMicroseconds(1000);
          delayMicroseconds(PERCENTAGERATE*1000);

          if(steps[0] == 0){
            current_steps += 1;
          }
          else{
            current_steps -= 1;
          }
        }
      }
    }

    void homeAxis(){
      while(digitalRead(homepin) != HIGH){
        int info[2] = {LOW, 0.01};
        move(info);
      }
      current_steps = 0;
    }

    int getSteps(){  return current_steps;  }

    void setSPU(int x){ steps_per_unit = x;  }
    long int getSPU(){ return steps_per_unit;  }

    void setS1s(int tmp){  stepper1s = tmp; }
    void setS1d(int tmp){  stepper1d = tmp; }


    void setS2s(int tmp){  stepper2s = tmp; }
    void setS2d(int tmp){  stepper2d = tmp; }
};

class CNC{
  public:
    Axis* X_Axis;
    Axis* Y_Axis;
    Axis* Z_Axis;

    CNC(Axis *X, Axis *Y, Axis *Z){
      X_Axis = X;
      Y_Axis = Y;
      Z_Axis = Z;
    }
};

void setup() {

  Serial.begin(4800);

  Axis *Xaxis = new Axis(0, 0, 0, 0, 0);
  Axis *Yaxis = new Axis(0, 0, 0, 0, 0);
  Axis *Zaxis = new Axis(0, 0, 0, 0, 0);

  CNC *Router = new CNC(Xaxis, Yaxis, Zaxis);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void interpolatetolocation(double Xmm, double Ymm, double Zmm, CNC* router){ //Passes in the absolute co-ordinates of X, Y, and Z to which the toolhead needs to be moved
  
  double X = Xmm - (router->X_Axis->getSteps()*router->X_Axis->getSPU());
  double Y = Ymm - (router->Y_Axis->getSteps()*router->Y_Axis->getSPU());
  double Z = Zmm - (router->Z_Axis->getSteps()*router->Z_Axis->getSPU());

  int Xdir = (X < 0) ? 0 : 1;
  int Ydir = (Y < 0) ? 0 : 1;
  int Zdir = (Z < 0) ? 0 : 1;

  if(Z == 0){ //Bresenham's Line algorithm
    
    double tmpx = abs(X) * 1000;
    double tmpy = abs(Y) * 1000;
  
    int dx = (int)tmpx;
    int dy = (int)tmpy;

    int count = 0;
    
    for(int i = 0; i < dx; i++){
      count += dy;
      int tmp[] = {Xdir, SMALLINCREMENT};
      router->X_Axis->move(tmp);
      if(count > dx){
        count -= dx;
        int tmp[] = {Ydir, SMALLINCREMENT};
        router->Y_Axis->move(tmp);
      }
    }
  }
  else{ 
    double tmpx = abs(X) * 1000;
    double tmpy = abs(Y) * 1000;
    double tmpz = abs(Z) * 1000;
  
    int dx = (int)tmpx;
    int dy = (int)tmpy;
    int dz = (int)tmpz;

    int count = 0;
    int countz = 0;
    
    for(int i = 0; i < dx; i++){
      count += dy;
      countz += dz;
      int tmp[] = {Xdir, SMALLINCREMENT};
      router->X_Axis->move(*tmp);
      if(count > dx){
        count -= dx;
        int tmp[] = {Ydir, SMALLINCREMENT};
        router->Y_Axis->move(*tmp);
      }
      if(countz > dx){
        countz -= dx;
        int tmp[] = {Xdir, SMALLINCREMENT};
        router->Z_Axis->move(*tmp);
      }
    }
  }
}

void arcCW(double startX, double startY, double centreX, double centreY, double radius, double angle, double Z, CNC* router){
  double circ = (2*radius*PI)*(angle/360);
  int divisor = circ / SMALLINCREMENT;
  
  double initialangle = asin( (startY - centreY) /radius);
  double tmpx;
  double tmpy;

  for(int j = 1; j <= divisor; j++){ //Handles the sitch for each of the four quadrants
    double newangle = j * (angle/divisor);
    if(((newangle+initialangle) <= 90) && ((newangle+initialangle) > 0)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX - tmpx, centreY + tmpy, Z, router);
    }
    else if(((newangle+initialangle) > 90) && ((newangle+initialangle) <= 180)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX + tmpx, centreY + tmpy, Z , router);
    }
    else if(((newangle+initialangle) > -90) && ((newangle+initialangle) < 0)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX - tmpx, centreY - tmpy , Z , router);
    }
    else if(((newangle+initialangle) <= -90) && ((newangle+initialangle) >= -180)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX + tmpx, centreY - tmpy , Z , router);
    }
  }
}

void arcCCW(double startX, double startY, double centreX, double centreY, double radius, double angle, double Z, CNC* router){
  double circ = (2*radius*PI)*(angle/360);
  int divisor = circ / SMALLINCREMENT;
  
  double initialangle = asin( (startY - centreY) /radius);
  double tmpx;
  double tmpy;

  for(int j = 1; j <= divisor; j++){ //Handles the sitch for each of the four quadrants
    double newangle = j * (angle/divisor) * -1;
    if(((newangle+initialangle) <= 90) && ((newangle+initialangle) > 0)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX - tmpx, centreY + tmpy, Z, router);
    }
    else if(((newangle+initialangle) > 90) && ((newangle+initialangle) <= 180)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX + tmpx, centreY + tmpy, Z , router);
    }
    else if(((newangle+initialangle) > -90) && ((newangle+initialangle) < 0)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX - tmpx, centreY - tmpy , Z , router);
    }
    else if(((newangle+initialangle) <= -90) && ((newangle+initialangle) >= -180)){
      tmpx = radius*cos(newangle+initialangle);
      tmpy = radius*sin(newangle+initialangle);

      interpolatetolocation(centreX + tmpx, centreY - tmpy , Z , router);
    }
  }
}

int getNumData(char * check){
  char* number = check;
  int code = 0;
  for(int m = 1; m < sizeof(number); m++){
    code*=10;
    code+=number[m];
  }
  return code;
}

void processGcode(char buffercontents[64]){

  char * value = strtok(buffercontents, " ");
  if(value[0] == 'G'){
    int code = getNumData(value);
    switch(code){
      //all gcode commands
      //movement commands run by repeatedly using the strtok function and getnumdata, to find X, Y, and Z values respectively. They then run the respective function with the arguments.

    }


  }
  else if(value == 'M'){

  }
}