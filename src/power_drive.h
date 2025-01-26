void setOutputPower(float percent);
float readOutputPower();
void PIDStep();

enum regulationMode {automatic, manual};

extern enum regulationMode regulationMode;
extern float regParamP;
extern float regParamI;
extern float regParamD;
extern float regParamIMin;
extern float regParamIMax;
extern int targetTemp;
