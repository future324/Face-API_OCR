import json
import re
pattern = re.compile(r"type':\s'textline',(.*?)}")
with open('result.json', encoding='utf-8') as json_file:
     data = str(json.load(json_file))
     result=re.findall(pattern,data)
     print(result)
input("enter")

