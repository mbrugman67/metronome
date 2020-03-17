
LEDS_PER_METER = 144;
LED_WIDTH = 5.2;
LED_LENGTH = 5.2;

module draw_led(x, y, z, h)
{
   translate([x, y, z - 1])
   cube([LED_LENGTH, LED_WIDTH, h + 2]);
}

module lcd(y)
{
   color([1, 0, 0])
   translate([0, 0, y])
   cube([97, 60, 2], true); 
   
   color([.5, .5, .5])
   translate([0, 0, y + 5])
   cube([87, 42, 10], true); 
   
}

module front()
{
   difference()
   {
      color([0, 0, 1])
      {
         translate([-60, -45, 0])
         cube([120, 90, 4]);
      }
      
      // LCD cutout
      color([1, 0, 1])
      {
         translate([-44, -21.5, -1])
         cube([88, 43, 6]);
      }
      
      // LED cutouts
      color([1, 0, 1])
      {
         spacing = 1000 / LEDS_PER_METER;
         for (x = [0 : 11])
         {
            start = -12 * spacing / 2;
            draw_led(start + (x * spacing), -36, 0, 6);
         }
      }
      
      // LED relief
      color([1, 0, 1])
      {
         translate([-50, -40, -1])
         cube([100, 14, 3]);
      }
   }
   
   color([0, 0, 1])
   {
      translate([-44, -24.5, -5])
      cube([88, 3, 5]);
      
      translate([-44, 21.5, -5])
      cube([88, 3, 5]);
   }
}   

front();
lcd(-6);