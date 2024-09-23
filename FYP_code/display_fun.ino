
void display_write(String a,int s,int x,int y){
  display.clearDisplay();
  display.setTextSize(s);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(x,y);             // Start at top-left corner
  display.println(a);
  display.display();
  }

void alart_display(int a){
  if(a==2){
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);      
  display.println("Alart---");
  display.setCursor(0,21);      
  display.println("MQ-2");
  display.setTextSize(1);
  display.setCursor(0,23);      
  display.println("H2, LPG, CH4, CO, Alcohol, Smoke or Propane.");
  display.display();
    }
  else if(a==135){
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);      
  display.println("Alart---");
  display.setCursor(0,21);      
  display.println("MQ-135");
  display.setTextSize(1);
  display.setCursor(0,23);      
  display.println("ammonia, nitrogen oxides, benzene, alcohol and smoke.");
  display.display();
    }
  else if(a==6050){
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);      
  display.println("Accident ?");
  display.setCursor(0,21);
  display.setTextSize(1);       
  display.println("Emergency number-9876543210,100,112");
    }
  }
