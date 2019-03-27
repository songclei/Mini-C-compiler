f_main [0] [0]
  call f_getint
  s0 = a0
  call f_getint
  s1 = a0
  s2 = 1
  s3 = 0
  s4 = s0 || s1
  if s4 == x0 goto l0
  a0 = s2
  call f_putint
  a0 = 1
  return
  goto l1
l0:
  a0 = s3
  call f_putint
  a0 = 0
  return
l1:
end f_main

