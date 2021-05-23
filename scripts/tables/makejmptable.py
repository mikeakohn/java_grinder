#!/usr/bin/env python

print "#include <stdio.h>"
print "#include <stdlib.h>\n"
print "#include \"java_class.h\"\n"

print "int java_execute_method(struct java_class_t *java_class, int method_index)"
print "{"
print "struct methods_t *method=((void *)java_class->methods_heap)+java_class->methods[method_index];"
print "unsigned char *bytes=method->attributes[0].info;"
print "int pc=0;"
print ""
print "  while(1)"
print "  {"
print "    switch(bytes[pc])"
print "    {"

fp = open("opcodes.txt","r")

last_opcode = -1

while 1:
  line = fp.readline()
  if not line: break
  line = line.strip()
  if not line: continue
  if line[0] == '#': continue

  tokens = line.split()
  opcode = int(tokens[0])
  if opcode-1 != last_opcode:
    for t in range(last_opcode+1, opcode):
      print "      case "+str(t)+": // "+tokens[2]+" "+tokens[1]
      #print "        printf(\""+tokens[1]+" "+tokens[2]+"\\n\");"
    #print "        break;\n"

  print "      case "+str(opcode)+": // "+tokens[2]+" "+tokens[1]
  #print "        printf(\""+tokens[1]+" "+tokens[2]+"\\n\");"
  #print "        printf(\"Opcode "+tokens[1]+" "+tokens[2]+" unimplemented\\n\");"
  #print "        break;\n"

  last_opcode = opcode

print "    }"
print "    pc++;"
print "    if (pc==10) break;"
print "  }"
print ""
print "  return 0;"
print "}"

fp.close()

# I was trying to make a goto/label jump table.  Realized it wasn't
# possible, so I did the above code with a switch/case and found it
# seems to actually do the jump table anyway.  Big Oh (Constant) :)

"""
print "#include <stdio.h>"
print "#include <stdlib.h>\n"
print "#include \"java_class.h\"\n"

print "int jvm_execute(struct java_class_t *java_class)"
print "{"
print "  while(1)"
print "  {"
print "    opcode=next_opcode();"
print "    goto opcode_jump_table[opcode];"

fp = open("opcodes.txt","r")

#last_opcode = -1

while 1:
  line = fp.readline()
  if not line: break
  line = line.strip()
  if not line: continue
  if line[0] == '#': continue

  tokens = line.split()
  opcode = int(tokens[0])
  #if opcode-1 != last_opcode:
  #  for t in range(last_opcode+1, opcode):
  #    print "    unused_"+str(t)+":"
  #else:

  print "    "+tokens[2]+"_"+str(opcode)+":"
  print "      goto opcode_done;\n"

  last_opcode = opcode

print "    unused_opcode:\n"
print "    opcode_done:\n"
print "  }"
print "}"

fp.close()

fp = open("opcodes.txt","r")

print "void *opcode_jump_table[255] ="
print "{"

last_opcode = -1

while 1:
  line = fp.readline()
  if not line: break
  line = line.strip()
  if not line: continue
  if line[0] == '#': continue

  tokens = line.split()
  opcode = int(tokens[0])
  if opcode-1 != last_opcode:
    for t in range(last_opcode+1, opcode):
      print "  unused_opcode,  // "+str(t)
  else:
    print "  "+tokens[2]+"_"+str(opcode)+","

  last_opcode = opcode

print "};"

fp.close()
"""




