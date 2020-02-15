


module switchBracket(x, y)
{
   color([1, .3, 0])
   {
      translate([x + 0, y + -7, 0])
      cube([40, 14, 4]);
   }
   
   color([1, 0, 1])
   {
      translate([x + 5, y + -7, 4])
      cube([5, 14, 6.5]);
      
      translate([x + 10, y + -7, 4])
      cube([1.7, 2, 6.5]);
      
      translate([x + 30, y + -7, 4])
      cube([5, 14, 6.5]);
      
      translate([x + 28.3, y + -7, 4])
      cube([1.7, 2, 6.5]);
      
   }
   
   color([0, 0, 1])
   {
      translate([x + 15, y - 2, 4])
      cylinder(5, 1.5, 1.5);
      
      translate([x + 25, y - 2, 4])
      cylinder(5, 1.5, 1.5);
   }
}

module button(x, y)
{
   color([0, 0, 1])
   {
      translate([-10, 0, 0])
      cylinder(3, 3.8, 3.8);
      
      translate([-10, 0, 0])
      cylinder(9, 1.8, 1.8);
   }
   
   difference()
   {
      color ([1, 0, 0])
      {
         translate([x + 0, y - 10, 0])
         cube([50, 20, 5]);
         
         translate([x + 0, y + 10, 0])
         cube([50, 5, 25]);
      }
      
      color ([1, .3, 0])
      {
         translate([x + 27, y + 7, 2.5])
         cylinder(15, 4, 4);
         
         translate([x + 27, y + 7, -1])
         cylinder(4, 2, 2);
      }

   }
}

//rotate([270, 0, 0])
switchBracket(0, -27);
button(0, 0);