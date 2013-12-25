function r = eq(obj1,obj2)
   % equality Implement obj1, obj2 for MapNode 
   obj1 = MapNode(obj1);
   obj2 = MapNode(obj2);
   % nan is treated as equal
   r = isequaln( obj1, obj2);
end 