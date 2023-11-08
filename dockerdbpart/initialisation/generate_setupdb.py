import json

with open('/json/config_terms.json') as f:
   param = json.load(f)

str_fields = []

txtboite =  param['txtboite']
for key in txtboite:
    str_fields.append('{ "'+key+'": { $type: "string" } }')


checkboite =  param['checkboite']
for key in checkboite:
    str_fields.append('{ "'+key+'": { $type: "array" } }')

listboite =  param['listboite']
for key in listboite:
    str_fields.append('{ "'+key+'": { $type: "string" } }')

dateboite =  param['dateboite']
for key in dateboite:
    str_fields.append('{ "'+key+'": { $type: "string" } }')

areaboite =  param['areaboite']
for key in areaboite:
    str_fields.append('{ "'+key+'": { $type: "string" } }')

str_fields.append('{ "chemin": { $type: "string" } }')

s = ",\n"
list_fields = s.join( str_fields )
#print (list_fields)


template = 'setupdb-js.template'
outputfile = 'setupdb.js'

with open(template, 'r') as f1, open(outputfile, 'w') as f2:
   line = f1.readline()
   while line:
       if '@@@@@@' in line:
           f2.write(list_fields+"\n")
       else:
           f2.write(line)
       line = f1.readline()

f1.close()
f2.close()



