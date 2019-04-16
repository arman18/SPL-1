function h = condentropy(vec1,vec2,v4,v5)
  
  [p12, p1, p2] = estpab(vec1,vec2,v4,v5);
  h = estcondentropy(p12,p2);

end


