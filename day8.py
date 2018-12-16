
f = open("input/day8.txt")

n = [ int(i) for i in f.readlines()[0].split(' ') ]

metadata = []
idx = 0

def read_node():
  global idx
  global n
  global metadata
  n_child = n[idx]
  n_md    = n[idx + 1]

  idx += 2

  node_val = 0

  if n_child > 0:

    child_values = list([ read_node() for c in xrange(n_child)])
    
    for m in xrange(n_md):
      metadata.append(n[idx])
      
      try:
        node_val += child_values[n[idx]-1]
      except: 
        pass

      idx += 1
    
  else:
    for m in xrange(n_md):
      node_val +=n[idx]
      idx += 1
  
  return node_val 

print read_node()

print sum(metadata),idx,len(n)
