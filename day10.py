import re

f = open("input/day10.txt")
pos = f.readlines()

prog = re.compile("position=<\s*(-?\d+),\s*(-?\d+)> velocity=<\s*(-?\d+),\s*(-?\d+)>")

pos = [ [ int(x) for x in prog.search(p).groups()] for p in pos ]

prev_bounds = 100000000000
itters = 0

for x in xrange(100000):
  pos = [ [p[0]+p[2], p[1]+p[3], p[2], p[3]] for p in pos ]

  x_bounds = max([ p[0] for p in pos]) - min([ p[0] for p in pos])
  y_bounds = max([ p[1] for p in pos]) - min([ p[1] for p in pos])

  bounds = x_bounds * y_bounds

  print bounds
  print itters

  if bounds > prev_bounds:
    break

  prev_bounds = bounds
  itters+=1

pos = [ [p[0]-p[2], p[1]-p[3], p[2], p[3]] for p in pos ]

points = { (p[0],p[1]) for p in pos }

for y in range(min([p[1] for p in pos]), 1+max([p[1] for p in pos])):
  for x in range(min([p[0] for p in pos]), 1+max([p[0] for p in pos])):
    print '#' if (x,y) in points else '.',
  print

