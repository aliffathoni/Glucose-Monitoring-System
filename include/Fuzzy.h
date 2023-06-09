#ifndef FUZZY_h_
#define FUZZY_h_

float A, B;

float uRendah[] = {40, 50, 60};
float uNormal[] = {50, 80, 110};
float uTinggi[] = {100, 150, 200};

float Rendah = 0;
float Normal = 0.5;
float Tinggi = 1;

float minr[4];
float Rule[4];

float fuRendah(int glucose_value)
{
   if (glucose_value < uRendah[0])
    {
        return 0;
    }
    else if (glucose_value >= uRendah[0] && glucose_value <= uRendah[1])
    {
        return (glucose_value - uRendah[0]) / (uRendah[1] - uRendah[0]);
    }
    else if (glucose_value >= uRendah[1] && glucose_value <= uRendah[2])
    {
        return (uRendah[2] - glucose_value) / (uRendah[2] - uRendah[1]);
    }
    // else if (glucose_value > uRendah[2])
    else  {
        return 0;
    }
}

float fuNormal(int glucose_value)
{
   if (glucose_value <= uNormal[0])
    {
        return 0;
    }
    else if (glucose_value >= uNormal[0] && glucose_value <= uNormal[1])
    {
        return (glucose_value - uNormal[0]) / (uNormal[1] - uNormal[0]);
    }
    else if (glucose_value >= uNormal[1] && glucose_value <= uNormal[2])
    {
        return (uNormal[2] - glucose_value) / (uNormal[2] - uNormal[1]);
    }
    // else if (glucose_value > uNormal[2])
    else  {
        return 0;
    }
}

float fuTinggi(int glucose_value)
{
   if (glucose_value <= uTinggi[0])
    {
        return 0;
    }
    else if (glucose_value >= uTinggi[0] && glucose_value <= uTinggi[1])
    {
        return (glucose_value - uTinggi[0]) / (uTinggi[1] - uTinggi[0]);
    }
    else if (glucose_value >= uTinggi[1] && glucose_value <= uTinggi[2])
    {
        return (uTinggi[2] - glucose_value) / (uTinggi[2] - uTinggi[1]);
    }
    // else if (glucose_value >= uTinggi[2])
    else    {
        return 0;
    }
}

void rule(int glucose_value)
{
  // if glucose_value rendah then rendah
  minr[1] = fuRendah(glucose_value);
  Rule[1] = Rendah;
  // if glucose_value normal then normal
  minr[2] = fuNormal(glucose_value);
  Rule[2] = Normal;
  // if glucose_value tinggi then tinggi
  minr[3] = fuTinggi(glucose_value);
  Rule[3] = Tinggi;
}

float defuzzyfikasi(int glucose_value)
{
  rule(glucose_value);
  A = 0;
  B = 0;

  for (int i = 1; i <= 3; i++)
  {
    // printf("Rule ke %d = %f\n", i, Rule[i]);
    // printf("Min ke %d = %f\n", i, minr[i]);
    A += Rule[i] * minr[i];
    B += minr[i];
  }
  // printf("Hasil A : %f\n", A);
  // printf("Hasil B : %f\n", B);
  return A / B;
}

String fuzzy_glucose(int glucose_value)
{
  float keputusan = defuzzyfikasi(glucose_value);
  String msg;

  // Serial.print("Hasil Keputusan Defuzzyfikasi : ");
  // Serial.print(keputusan);
  if (keputusan >= 0 && keputusan < 0.5) {
    msg = "Rendah";
  } else if (keputusan >= 0.5 && keputusan < 1) {
    msg = "Normal";
  } else if (keputusan >= 1) {
    msg = "Tinggi";
  } else{
    msg = "Error";
  }
  // Serial.print("Status_glucose_value : ");
  // Serial.println(msg);
  return msg;
}

#endif