# -*- coding: utf8 -*-
import os
import re
import json
from pprint import pprint
import pymongo
from pymongo import MongoClient
import urllib.parse
import config
import importlib.util

try:
    import requests
except ImportError as e:
    pass  # module doesn't exist

#######################################################################################################
# Script generating mongo commands to recreate the database and insert data from META_XXXX.json files #
#######################################################################################################

### functions ###

def is_valid_json_file(filename):
    path_str = filename.replace("/pgd_data/", "")
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            content = json.load(f)
        return True
    except json.JSONDecodeError as e:
        print("ERROR: "+path_str+f" is not a valid JSON file : {e}")
        return False


def get_json(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        content = json.load(f)
    return content


def isJSONValid(path):
    # load the JSON schema
    json_schema = get_json('/json/maggot-schema.json')

    # Load metadata file
    json_data = get_json(path)

    # URL & header : see https://assertible.com/json-schema-validation
    url = "https://assertible.com/json"
    headers = {"Content-Type":"application/json; charset=utf-8"}

    # Payload
    payload = {
        'schema': json_schema,
        'json': json_data
    }

    # Default return value
    ret = False
    path_str = path.replace("/pgd_data/", "")

    try:
        # Send the POST request
        session = requests.Session()
        response = session.post(url, headers=headers, json=payload)
        #response.raise_for_status()

        # Response
        result = response.json()
        if result.get("valid", False):
            print("SUCCESS: "+path_str+"  is valid")
            ret = True
        else:
            print("ERROR: "+path_str+" is not a valid Maggot JSON file")
            for error in result.get("errors", []):
                print(f"- {error}")

    except requests.exceptions.Timeout:
        print("Timeout is reached")
    except requests.exceptions.RequestException as e:
        print(f"❗ An error occurs for "+path_str)

    return ret


def scan_dir(dir):
    listedirpgd = os.listdir(dir)
    if '#snapshot' in listedirpgd: listedirpgd.remove('#snapshot')
    for name in listedirpgd: 
        path = os.path.join(dir, name)
        if os.path.islink(path):
            continue
        if os.path.isfile(path):
            if not p.match(name):
                continue
            if is_valid_json_file(path) is False:
                continue
            spec = importlib.util.find_spec('requests')
            if spec is not None and isJSONValid(path) is False:
                continue
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

## Data to be inserted into the database
db_commands = 'DB_commands.json' # File containing data insertion commands
filename = open(db_commands, "w")
filename.write("[\n")

# Directory scan
scan_dir("/pgd_data")

filename.write("]\n")
filename.close()

# File containing data insertion commands
db_final_commands = 'DB_final_commands.json'
final_command_file = open(db_final_commands,"w")

filename = open(db_commands, "r")
tempfile = filename.read()
chaineTriee = re.sub(r'(.*)},\n]', r'\1}\n]', tempfile)
final_command_file.write(chaineTriee)
final_command_file.close()

if os.path.getsize(db_final_commands)>10:
    # Load JSON file to insert into the database
    final_file_dict = get_json(db_final_commands)

    # data insertion
    database = urllib.parse.quote_plus(config.database)
    username = urllib.parse.quote_plus(config.username)
    password = urllib.parse.quote_plus(config.password)
    dbserver = urllib.parse.quote_plus(config.dbserver)
    dbport = str(config.dbport)
    client = MongoClient("mongodb://"+username+":"+password+"@"+dbserver+":"+dbport+"/"+database)
    db = client[config.database]
    collection_metadata = db['metadata']
    collection_metadata.drop()
    collection_metadata.insert_many(final_file_dict)

    # Mongo Client Closure
    client.close()

