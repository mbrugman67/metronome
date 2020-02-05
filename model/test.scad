WIDTH = 14;
HEIGHT = 2;

F_WIDTH = 30;
F_HEIGHT = 5;
F_LENGTH = 80;

LEDS_PER_METER = 144;
LED_WIDTH = 5.2;
LED_LENGTH = 5.2;

X_OFFSET = -5;

module draw_led(X)
{
   color([0, 1, 1])
   translate([X, 0, HEIGHT / 2 - 1])
   cube([LED_LENGTH, LED_WIDTH, HEIGHT + 4], true);
}

difference()
{
   color([1, 0, 1])
   {
      translate([X_OFFSET, -F_WIDTH / 2 , 0])
      cube([F_LENGTH, F_WIDTH, F_HEIGHT]);
   }
   color([0, 0, 1])
   {
      translate([X_OFFSET - 1, -WIDTH / 2, HEIGHT])
      cube([F_LENGTH + 2, WIDTH, 10]);
   }
   
   spacing = 1000 / LEDS_PER_METER;
   for (x = [0 : 10])
   {
      draw_led(x * spacing);
   }
}