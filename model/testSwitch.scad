


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
      cube([5, 14, 5]);
      
      translate([x + 10, y + -7, 4])
      cube([1.7, 4, 5]);
      
      translate([x + 30, y + -7, 4])
      cube([5, 14, 5]);
      
      translate([x + 28.3, y + -7, 4])
      cube([1.7, 4, 5]);
      
   }
   
   color([0, 0, 1])
   {
      translate([x + 15, y + 2, 4])
      cylinder(5, 1, 1);
      
      translate([x + 25, y + 2, 4])
      cylinder(5, 1, 1);
   }
}

module button(x, y)
{
   color ([1, 0, 0])
   {
      translate([x + 0, y - 10, 0])
      cube([50, 20, 5]);
   }
}

rotate([270, 0, 0])
switchBracket(5, -20);
button(0, 0);