# -*- coding: utf8 -*-
import os
import re
import json
from pprint import pprint
import pymongo
from pymongo import MongoClient
import urllib.parse
import config

#######################################################################################################
# Script generating mongo commands to recreate the database and insert data from META_XXXX.json files #
#######################################################################################################

### fonctions ###
def get_json(filename):
	with open(filename, 'r') as f:
		datas = json.load(f)
		return datas

def scan_dir(dir):
	listedirpgd = os.listdir(dir)
	if '#snapshot' in listedirpgd: listedirpgd.remove('#snapshot')
	for name in listedirpgd:
		path = os.path.join(dir, name)
		if os.path.islink(path):
			continue
		if os.path.isfile(path):
			if p.match(name):
				filename.write("\t{")
				mydata = get_json(path)
				for key,value in mydata.items():
					if isinstance(value,list):
						field = "\""+key+"\":"+json.dumps(list(value), ensure_ascii=False)+","
						filename.write(field)
					else:
						value = regex.sub("", value)
						value = value.replace('"','\\"')
						field = "\""+key+"\":\""+value+"\","
						filename.write(field)
				field = "\"chemin\":\""+path+"\""
				filename.write(field)
				field = "},\n"
				filename.write(field)
		else:
			scan_dir(path)

############
### Prog ###
############
p = re.compile('META_.*\.json')
regex = re.compile(r'[\n\r\t]')

## Données à insérer dans la base
db_commands = 'DB_commands.json' # Fichier contenant commandes insertion de données
filename = open(db_commands, "w")
filename.write("[\n")

# Scan du répertoire
# Development local
scan_dir("/pgd_data")

filename.write("]\n")
filename.close()

# Fichier contenant commandes insertion de données
db_final_commands = 'DB_final_commands.json'
final_command_file = open(db_final_commands,"w")

filename = open(db_commands, "r")
tempfile = filename.read()
chaineTriee = re.sub(r'(.*)},\n]', r'\1}\n]', tempfile)
final_command_file.write(chaineTriee)
final_command_file.close()

if os.path.getsize(db_final_commands)>10:
    # Get json du fichier à insérer dans la base
    final_file_dict = get_json(db_final_commands)

    # Creation base + insertion des données
    database = urllib.parse.quote_plus('pgd-db')
    username = urllib.parse.quote_plus(config.username)
    password = urllib.parse.quote_plus(config.password)
    dbserver = urllib.parse.quote_plus(config.dbserver)
    dbport = str(config.dbport)
    client = MongoClient("mongodb://"+username+":"+password+"@"+dbserver+":"+dbport+"/"+database)
    db = client['pgd-db']
    collection_metadata = db['metadata']
    collection_metadata.drop()
    collection_metadata.insert_many(final_file_dict)

    # Fermeture client Mongo
    client.close()

