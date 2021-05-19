# Project Implementation exercise 2 for the course Heuristic Optimization

In order to run this project on a single instance, firstly, one needs to be in the flowshopWCT folder within the terminal. Then, the following command will run the project on a single instance for example: 
		
		./flowshopWCT ../instances/50_20_01 first transpose random

An alternative way to run the project is also possible. A python script is available in the src folder. This script will run the project on all instances selected. Some parameters might have to be modified manually to segment the study and for my comprehension, for example the instance size and file name created. In any case, those parameters are explained in the file. In order to run the script, one can type the following command, still in the flowshopWCT folder : 

		python src/make.py

A file will then be created with the wct of all instances tested.