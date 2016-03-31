import os
import shutil

if __name__ == "__main__":
    path = os.getcwd()
    path = path + "/shadow"

    if os.path.exists(path):
        shutil.rmtree(path)
    os.makedirs(path)
        
    os.chdir(path)
    os.system("qmake ../src -tp vc -r")

    #copy dll files
    shutil.copyfile("../src/QtService/QtSolutions_Service-head.dll", "./bridger/QtSolutions_Service-head.dll")
    shutil.copyfile("../src/QtService/QtSolutions_Service-headd.dll", "./bridger/QtSolutions_Service-headd.dll")
    shutil.copyfile("../src/QtService/QtSolutions_Service-head.dll", "./controller/QtSolutions_Service-head.dll")
    shutil.copyfile("../src/QtService/QtSolutions_Service-headd.dll", "./controller/QtSolutions_Service-headd.dll")
