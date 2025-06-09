# -*- coding: utf8 -*-
import os
import re
import json
from pprint import pprint
from pprint import pformat

### functions ###

def is_valid_json_file(filename):
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            content = json.load(f)
        return True
    except json.JSONDecodeError as e:
        return False

def get_json(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        content = json.load(f)
    return content

# Transforms a key/set dictionary into a key/list
def set_to_list_dic(mydicset):
    mydiclist = dict()
    for key,value in mydicset.items():
        mydiclist[key] = sorted(value)
    return mydiclist

# loads the dictionary of active options and returns the key/set dictionary
def load_active_dic(masterdic):
    activedic = dict()
    for key,value in masterdic['chkbxdico'].items():
        activedic[key] = set(value)
    for key,value in masterdic['listdico'].items():
        activedic[key] = set(value)
    return activedic

# Creates an empty key/set dictionary based on a template dictionary
def init_dic(modeldic):
    maindic = dict()
    for key,value in modeldic.items():
        maindic[key] = set()
    return maindic

# Merges the dictionary from the scan with that of the active options
def merge_dic(activedic, maindic):
    mergeddic = dict()
    for key, value in activedic.items():
        if key in maindic:
            mergeddic[key] = maindic[key].union(set(activedic[key]))
        else:
            mergeddic[key] = set(activedic[key])
    return mergeddic

# Removes empty elements from a key/set dictionary
def clean_dic(dico):
    for key, value in dico.items():
        dico[key].discard('')

# Traverses the tree and add json data into a single dictionary
def scan_dir(dir,dico):
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
            mydata = get_json(path)
            for key,value in mydata.items():
                #print(type(value))
                if isinstance(value,list):
                    if key != 'resources' and key in dico:
                        dico[key].update(set(mydata[key]))
        else:
            scan_dir(path,dico)

# Reorganizes the data in the master file format (i.e. chkbxdico and listdico) and returns the dictionary
def format_dico(mydico,master):
    formated = dict()
    for key in master:
        formated[key] = {}
        for subkey in master[key]:
            formated[key][subkey] = list(sorted(mydico[subkey]))
    return formated

### Prog ###
p = re.compile('META_.*\.json')
master_json = '../json/config_terms.json'
outputfile = '../json/options.json'

if is_valid_json_file(master_json) is True:
    masterdic = get_json(master_json)
    str_masterdic = '{ "chkbxdico": '+str(masterdic['chkbxdico'])+', "listdico": '+str(masterdic['listdico'])+'}'
    masterdic = json.loads(str_masterdic.replace("'",'"'))

    activedic = load_active_dic(masterdic)
    maindic = init_dic(activedic)

    scan_dir("/pgd_data/", maindic)

    alldata = merge_dic(activedic,maindic)
    clean_dic(alldata)
    formated = format_dico(alldata,masterdic)

    param = get_json(master_json)
    param['chkbxdico']=formated['chkbxdico'];
    param['listdico']=formated['listdico'];

    fo = open(outputfile, 'w')
    fo.write(str(param).replace("'","\"").replace('True','true').replace('False','false'))
    fo.close()

else:
    print("ERROR: "+master_json+" is not a valid JSON file")

