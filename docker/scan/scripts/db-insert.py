import os
import json
import pymongo
from pymongo import MongoClient
import urllib.parse
import config


def get_json(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        content = json.load(f)
    return content

############
### Prog ###
############


# File containing data insertion commands
db_final_commands = 'DB_final_commands.json'

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

