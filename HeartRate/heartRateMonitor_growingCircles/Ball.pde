class Ball {
  float raio = 0;
  int opacity = 255;
  
  Ball(){
  }
  
  void display() {
    noStroke();
    fill(255,opacity);
    ellipse(width/2,height/2,raio,raio);
  }
  
  void grow() {
    opacity-=2;
    raio+=20;
  }
  
  float getRaio() {
    return raio;
  }
}
