import json, os;

with open('JECTemp.js') as json_file:
   Data = json.load(json_file)
   os.makedirs('RecoveredFragment', 0o777, True)
   for Key in Data:
      print(Key)
      with open('RecoveredFragment/' + Key + '.txt', 'w') as output:
         output.write("\""+Key+"\":")
         json.dump(Data[Key], output)
         output.write(",")
         output.close()


