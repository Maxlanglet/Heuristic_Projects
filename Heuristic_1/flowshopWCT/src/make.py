import subprocess
import time

# makefile command
def make_compile():
        subprocess.check_call(["make"])
make_compile()

# CHANGE THESE FILES ACCORDING TO YOUR COMPUTER
#list of the files we want to test
files = ["100_20_", "50_20_"]
first = ["first", "best"]
second = ["transpose", "exchange", "insert"]
third = ["random", "srz"]

#for every file, we run the code for every buffer size
# for filename in files:
# 	for t in third:
# 		for s in second:
# 			for f in first:
# 				for i in range(1,31):
# 					if i < 10:
# 						print("./flowshopWCT"+"../instances/"+filename+" 0 "+str(i)+f+s+t)
# 						proc = subprocess.call(["./flowshopWCT","../instances/"+filename+"0"+str(i),f,s,t], stdout=results)
# 					else:
# 						print("./flowshopWCT"+"../instances/"+filename+str(i)+f+s+t)
# 						proc = subprocess.call(["./flowshopWCT","../instances/"+filename+str(i),f,s,t], stdout=results)
average=0
f = "best"
s = "exchange"
t = "random"
#results = open(f+"_"+s+"_"+t+".txt", "w")
results = open("VND_tei_2.txt", "w")
start = time.time()
for filename in files:
	for i in range(1,31):
		if i < 10:
			print("./flowshopWCT"+"../instances/"+filename+" 0 "+str(i)+f+s+t)
			proc = subprocess.call(["./flowshopWCT","../instances/"+filename+"0"+str(i),f,s,t], stdout=results)
		else:
			print("./flowshopWCT"+"../instances/"+filename+str(i)+f+s+t)
			proc = subprocess.call(["./flowshopWCT","../instances/"+filename+str(i),f,s,t], stdout=results)
	end = time.time()
	print(end - start)
	start = time.time()
# end = time.time()
# print(end - start)