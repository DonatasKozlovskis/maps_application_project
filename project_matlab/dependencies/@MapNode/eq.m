function r = eq(obj1,obj2)
   % Plus Implement obj1 + obj2 for DocPolynom 
   obj1 = MapNode(obj1);
   obj2 = MapNode(obj2);

   r = isequaln( obj1, obj2);
end 