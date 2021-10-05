class Button {
  PVector Pos = new PVector(0, 0);  //PVector is a builtin class
  float Width = 0;
  float Height = 0;
  color Color;
  String Text;
  Boolean Pressed = false;
  Boolean Clicked = false;
  
  // Constructor (x and y position, width and height, RGB color code
  Button(int xpos, int ypos, int w, int h, String t, int r, int g, int b) {
    Pos.x = xpos;
    Pos.y = ypos;
    Width = w;
    Height = h;
    Color = color(r, g, b);
    Text = t;
  }
  
  /* Function: update
  *  detects whether button has been clicked or not (must place in void Draw())
  */
  void update() {
    if (mousePressed == true && mouseButton == LEFT && Pressed == false) {
      println("mouse Pressed");
      Pressed = true;
      if (mouseX >= Pos.x && mouseX <= Pos.x+Width && mouseY >= Pos.y && mouseY <= Pos.y+Height) {
        Clicked = true;
      }
    } else {
      Clicked = false;
      Pressed = false;
    }
    //if (mousePressed != true) {
    //  Pressed = false;
    // }
  }
  
  /* Function: render
  *  Creates the visual button (must place in void Draw())
  */
  void render() {
    fill(Color);  // color of our rectangle button. Everything after this line will be this color unless changed
    rect(Pos.x, Pos.y, Width, Height);  // creates button object
    
    fill(0);  // color of text (black). Everything below this line will be black unless changed
    textAlign(CENTER, CENTER);  // puts the next in the center of our button
    text(Text, Pos.x+(Width/2), Pos.y+(Height/2));
  }
  
  /* Function: isClicked
  *  Returns the status of our 'Clicked' boolean value
  */
  boolean isClicked() {
    return Clicked;
  }
  
}
