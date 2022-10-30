import json
import CreateClass as creation
#import requests

def find_class(id, org, path):
        #f = requests.get(path)
        classFlag = False
        orgFlag = False
        f = open("..\data.json")
        data = json.load(f)
        f.close()
        if data == None:
            return None;
        for c in data["class_data"]:
            if c['classname'] == id and c['org'] == org:
                return c
            elif c['classname'] == id:
                classFlag = True
            elif c['org'] == org:
                orgFlag = True
        if classFlag:
            return -1
        elif orgFlag:
            return -2
        else:
            return None

if __name__ == "__main__":
    c = find_class("How2Monke", "Monkey U", "..\data.json")

    if c is None:
        print("Database empty")
        quit()
    elif c == -1:
        print("Class does not exist")
        quit()
    elif c == -2:
        print("Organization does not exist")
        quit()
    creation.CreateClass("..\\input.json")
    c = find_class("How 2 Monkey II", "Monkey U", "..\data.json")
    print(c['org'])
    print(c['classname'])
    print(c['events'])
    print(c['members'])