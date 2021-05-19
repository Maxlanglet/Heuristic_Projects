import subprocess
import time

# makefile command
def make_compile():
        subprocess.check_call(["make"])
make_compile()

# CHANGE THESE FILES ACCORDING TO THE TESTING DONE

#list of the files we want to test
files = ["50_20_"]#, 100_20_ #segmented since we split the comparisons this time

f = "IGA"# or "Tabu"
s = "srz"#only srz tested in our case
results = open(f+"_"+s+"_50"+".txt", "w")#or "_100" file name not to confuse files 


start = time.time()

for filename in files:
	for i in range(1,31):#loop over all files
		if i < 10:
			print("./flowshopWCT"+"../instances/"+filename+" 0 "+str(i)+f+s)
			proc = subprocess.call(["./flowshopWCT","../instances/"+filename+"0"+str(i),f,s], stdout=results)
		else:
			print("./flowshopWCT"+"../instances/"+filename+str(i)+f+s)
			proc = subprocess.call(["./flowshopWCT","../instances/"+filename+str(i),f,s], stdout=results)
	end = time.time()
	print(end - start)
	start = time.time()
