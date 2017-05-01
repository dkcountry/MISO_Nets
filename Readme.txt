# CS Project for AWS Big Data class at UChicago. Written by David Kang and Qizhen He, 5/1/2013
This is basic feed forward neural net implemention designed to making trading decisions on the MISO electricity data. Our goal was to forecast future price changes based on a variety of current-time inputs - including current day prices, relationships between locations (nodes), and time. Due to the volume of data, we were eventually required to re-implement in C++ and parallelize the program in AWS 



#The following are the instructions for compiling the C++ implementation.

Separate instructions for running the python version can be found in the prototype folder.

To compile:
1) Inside the C++ folder, type $ make

2) ./main -d node.txt -i ../SAMPLEDATA/training -t ../SAMPLEDATA/testing

where
	-d specifies the location for the node file
	-i specifies the location of the input/training set
	-t specifices the location of the testing set
[OPTIONAL]
	-k (int) specifies the number of neurons (default = 10)
	-n (int) specifices the of training sessions (default = 10)
	-l (double) specifies the learning rate (default = .05)

3) The results and errorlog are saved into the Output folder. To visualize, run $python visualization.py

(We only included a subset of the data since the entire dataset is too large to load to github. The full dataset can be accessed at https://www.misoenergy.org/Library/MarketReports/Pages/MarketReports.aspx)

