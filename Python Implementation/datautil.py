import csv
import os
import re
import numpy as np
import matplotlib.pyplot as plt
from neuralnet import *

'''Return a list of nodes'''
def get_nodes(nodes_file):
    upload = open(nodes_file,"rU")
    reader = csv.reader(upload)
    nodes = [str(x[0]) for x in reader]
    return nodes

'''Basic dataset object for neural network'''
class Dataset(object):
    def __init__(self,nodes_list, path_to_inputs,path_to_targets,startdate = 0):
        self.nodes = nodes_list
        self.date = 0
        '''num of neurons required in input and output layer = #nodes + 24 hrs + 1 datetime'''
        self.ionum = len(nodes_list) + 24 + 1

        '''clean data includes original csv data + node'''
        self.cleaninputs = self.__extract(self.nodes,path_to_inputs,startdate)
        self.cleantargets = self.__extract(self.nodes,path_to_targets,startdate)
        assert len(self.cleaninputs) == len(self.cleantargets)

        '''Data for NN'''
        self.__processed_data = self.process_data()
        self.inputs = self.__processed_data[0]
        self.targets = self.__processed_data[1]

    def __retrieve(self, path):
        allfiles = os.listdir(path)
        return allfiles

    def __extract(self,nodes,path,startdate):
        files = self.__retrieve(path)
        fullpath = [os.path.join(path,x) for x in files if re.search("csv",x)]
        csv_objects = [csv.reader(open(x,"rU")) for x in fullpath]
        cleandata = []
        date_index = startdate
        for x in csv_objects:
            date_index = date_index+1
            for row in x:
                if row != []:
                    for i in range(len(nodes)):
                        node = np.zeros(len(nodes))
                        node[i] = 100
                        if str(row[0]) == nodes[i]:
                            if str(row[2]) == "LMP":
                                data_entry = row
                                data_entry.append(date_index)
                                data_entry.extend(node)
                                cleandata.append([data_entry,nodes[i],date_index])
        self.date = date_index
        return cleandata

    '''convert clean data into data readable by NN'''
    '''data can be filtered by node'''
    def process_data(self,nodes = None):
        cleaninputs = self.cleaninputs
        cleantargets = self.cleantargets
        
        if nodes != None:
            pre_inputs = [x[0][3:] for x in cleaninputs if x[1] in nodes]
            pre_targets = [x[0][3:] for x in cleantargets if x[1] in nodes]

        else:
            pre_inputs = [x[0][3:] for x in cleaninputs]
            pre_targets = [x[0][3:] for x in cleantargets]
        inputs = []
        for x in pre_inputs:
            data_entry = [float(y)/100. for y in x]
            inputs.append(data_entry)
        targets = []
        for x in pre_targets:
            data_entry = [float(y)/100. for y in x]
            targets.append(data_entry)
            
        return inputs, targets

'''Dataset object when targets are unknown (testing set only)'''
class Forecast(Dataset):
    def __init__(self,nodes_list, path_to_inputs,startdate = 0):
        self.nodes = nodes_list
        self.date = 0
        self.ionum = len(nodes_list) + 24 + 1

        self.cleaninputs = self.__extract(self.nodes,path_to_inputs,startdate)
       
        self.inputs = self.process_data()

    def process_data(self,nodes = None):
        cleaninputs = self.cleaninputs
        
        if nodes != None:
            pre_inputs = [x[0][3:] for x in cleaninputs if x[1] in nodes]
        else:
            pre_inputs = [x[0][3:] for x in cleaninputs]
        inputs = []
        for x in pre_inputs:
            data_entry = [float(y)/100. for y in x]
            inputs.append(data_entry)
 
        return inputs

'''Output class for the NN'''
class Output(object):
    def __init__(self,dataset,net):
        self.net = net
        self.ds = dataset
        self.nodes = dataset.nodes
        self.output = None
        self.outputplus = None

    def __get_output(self,inputs = None):
        if inputs == None:
            inputs = self.ds.inputs
        output = []
        for x in inputs:
            self.net.feedforward(x)
            output.append(self.net.output)
        self.output = output

    def __organize_results(self,inputs = None,targets = None, output = None):
        if inputs == None:
            inputs = self.ds.inputs
            targets = self.ds.targets
            output = self.output
        organize = []
        for i in range(len(inputs)):
            organize.append(zip(inputs[i],targets[i],output[i]))
        data = []
        for x in organize:
            for i in range(24):
                data.append(x[i])
        self.outputplus = data
            
    def __PL_analyzer(self):
        Daily_PL = []
        for x in self.outputplus:
            if x[2] >= x[0]:
                PL = x[1] - x[0]
            else:
                PL = x[0] - x[1]
            Daily_PL.append(PL)
        Overall_PL = [0]
        for i in range(len(Daily_PL)):
            Overall_PL.append(Daily_PL[i]+Overall_PL[i])
        Overall_PL = [x*100 for x in Overall_PL]
        self.Overall_PL = Overall_PL

    def __write(self, Node = None, new = True):
        if new:
            out = open("out.txt", "w")
        else:
            out = open("out.txt","a")
        if Node != None:
            out.write(str(Node) + "\n")
        out.write("Day Ahead, Real Time, NN Output\n")
        hour = 0
        day = 1
        for x in self.outputplus:
            if hour%24 == 0:
                out.write("\nDay" + str(day) +"\n")
                day = day + 1
            out.write(str(x[0]*100) + ", " + str(x[1]*100) + ", " + str(x[2]*100) + "\n")
            hour = hour + 1
        

    def __plot_PL(self):
        plt.plot(self.Overall_PL)
        plt.title("Total Profit/Loss Return over Testing Period")
        plt.xlabel("Time")
        plt.ylabel("Net Return")
        plt.show()

    def display(self):
        self.__get_output()
        self.__organize_results()
        self.__PL_analyzer()
        self.__plot_PL()

    def analyze_by_node(self, nodes = None,Overall = False):
        nodes = self.nodes
        newfile = True
        PL_tracker = []
        for x in nodes:
            filtered_data = self.ds.process_data([x])
            tempinputs = filtered_data[0]
            temptargets = filtered_data[1]
            self.__get_output(tempinputs)
            self.__organize_results(tempinputs,temptargets,self.output)
            self.__PL_analyzer()
            self.__write(x, new = newfile)
            newfile = False
            PL_tracker.append(self.Overall_PL)
            plt.plot(self.Overall_PL, label = x)
        if Overall and PL_tracker != []:
            PL = []
            for i in range(len(PL_tracker[0])):
                hourly_total_PL = 0
                for x in PL_tracker:
                    hourly_total_PL = hourly_total_PL + x[i]    #potential error: nodes missing data -> lists of diff lens
                PL.append(hourly_total_PL)
            plt.plot(PL, label = "AGGREGATE")
        plt.legend(loc = 4, prop={'size':10})
        plt.title("Total Profit/Loss Return over Testing Period")
        plt.xlabel("Time")
        plt.ylabel("Net Return")
        plt.show()
            
        
            
