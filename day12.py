
f = open("input/day12state.txt")
state = { i[0] for i in enumerate(f.readlines()[0][0:-1]) if i[1] == '#'}

f = open("input/day12rules.txt")
rules = [ x for x in f.readlines() ]
rules = [ rule.split('=>') for rule in rules ]
rules = [ [ x.replace('\n',"").replace(" ","") for x in rule] for rule in rules ]
rules = [ rule[0] for rule in rules if rule[1] == '#' ]
rules = [ {i[0] for i in enumerate(r) if i[1] == '#'} for r in rules ]

def notalive(rule,x):
  return {x+r for r in xrange(-2,3)}-{x+r-2 for r in rule}

def alive(rule,x):
  return {x+r-2 for r in rule}

def next(state, rules):
  return { x for x in xrange(min(state) -3, max(state) +4) 
           if any([ alive(r, x).issubset(state) 
                    and 
                    notalive(r, x).intersection(state)==set([]) 
                    for r in rules ]) }

prev_states = {}
taken = 0
prev_states[frozenset(state)] = (len(prev_states), taken)
for x in xrange(105):
  state = next(state, rules)

  m = min(state)
  taken += m

  state = { x - m for x in state } 

  if frozenset(state) in prev_states:
    print "saw before", prev_states[frozenset(state)]
    break

  prev_states[frozenset(state)] = (len(prev_states), taken)


print len(prev_states)
print taken
print state
print sum([ x + taken + (50000000000-len(prev_states)) for x in state ])
