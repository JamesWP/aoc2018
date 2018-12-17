
f = open("input/day17.txt.sample")
l = f.readlines()
l = [ [ part.split('=') for part in line.split(',') ] for line in l]


def points(line):
  s = line[0][0].strip()
  sv = int(line[0][1])

  v = line[1][0].strip()
  vv = [ int(x) for x in line[1][1].split('..')]
  vv = [ i for i in range(vv[0], vv[1]+1) ]

  x = [sv]*len(vv) if s == 'x' else vv
  y = vv if v == 'y' else [sv]*len(vv) 

  return zip(x,y)

points =  frozenset({ point for p in l for point in points(p) })
sources = { (500, 0) }
still = set()

ymin = min([ p[1] for p in points]) 
ymax = max([ p[1] for p in points]) 
xmin = min([ p[0] for p in points]) 
xmax = max([ p[0] for p in points]) 

print len(points)
print 
print 

def source_can_flow_down(source):
  n = (source[0], source[1]+1)
  if n in points or n in still:
    return False
  return True

def points_in_row(point):
  points_in_row = { p for p in points if p[1] == point[1] }
  points_to_left = { p for p in points_in_row if p[0] < point[0] }
  points_to_right = { p for p in points_in_row if p[0] > point[0] }
  
  return (points_to_left, points_to_right)

def bounded(source):
  (points_to_left, points_to_right) = points_in_row(source)

  if len(points_to_left) == 0 or len(points_to_right) == 0:
    return False 
 
  stillmin = max(points_to_left)[0] 
  stillmax = min(points_to_right)[0] 

  width = { (x, source[1]) for x in xrange(stillmin, stillmax+1) }

  #   .....#.......#.....
  #         |width|
  
  below_width = { (x,y+1) for (x,y) in width } 

  return all([ bw in still or bw in points for bw in below_width]) 

def new_still_blocks(source):
  (points_to_left, points_to_right) = points_in_row(source)
  
  stillmin = max(points_to_left)[0] 
  stillmax = min(points_to_right)[0] 

  nsb = { (x, source[1]) for x in xrange(stillmin, stillmax+1) }

 
  return nsb

def new_source_blocks(source):

  points_in_row = { (x,source[1]) for x in xrange(xmin,xmax+1) }
  possible_sources = { (x,y) for (x,y) in points_in_row if (x,y+1) not in still and (x,y+1) not in points} 

  bounds = points_in_row.intersection(points)

  left_bound = None
  right_bound = None

  try:
    left_bound = max({ x for (x,y) in bounds if x < source[0]})
  except ValueError:
    pass

  try:
    right_bound = min({ x for (x,y) in bounds if x > source[0]})
  except ValueError:
    pass

  left_source = None
  right_source = None

  try:
    left_source = max({ (x,y) for (x,y) in possible_sources if x < source[0] and (left_bound is None or x > left_bound)})
  except ValueError:
    pass
  
  try:
    right_source = min({ (x,y) for (x,y) in possible_sources if x > source[0] and (right_bound is None or x < right_bound)})
  except ValueError:
    pass

  return { p for p in [left_source, right_source] if p is not None}

def pour(source):
  if source_can_flow_down(source):
    source = (source[0], source[1]+1)
    if source[1] > max([ p[1] for p in points]):
      return None
    return pour(source)
  if bounded(source):
    return ('still', new_still_blocks(source))
  # flowing right or left or both
  return ('source', new_source_blocks(source))

s = (500,0)

for x in xrange(9):
  (t, b) = pour(s)
  if t == 'still':
    still = still.union(b)
  if t == 'source':
    sources = sources.union(b)
    s = next(iter(b))

for y in xrange(-1 + ymin, ymax +2):
  for x in xrange(-1 + xmin, xmax+2):
    p = (x,y)
    if p in sources:
      print '+',
    elif p in points:
      print '#',
    elif p in still:
      print '~',
    else:
      print '.',
  print 
