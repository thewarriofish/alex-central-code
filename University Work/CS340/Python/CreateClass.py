import json
#import requests

def GetData(path):
    #f = requests.get(path)
    f = open(path)
    data = json.load(f)
    f.close()
    return data['class_input'][0]

def CreateClass(path):
    c = GetData(path)
    if c == None:
        print("ya dun goofed")
    with open("..\\data.json", "r+") as db:
        data = json.load(db);
        data['class_data'].append(c)
        db.seek(0)
        json.dump(data, db, indent = 2)

if __name__ == "__main__":
    CreateClass("..\\input.json")
