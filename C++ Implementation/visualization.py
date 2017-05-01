import csv
import matplotlib.pyplot as plt

def error(errorlog):
    upload = open(errorlog, "rU")
    reader = csv.reader(upload)
    error = [float(x[0]) for x in reader]
    plt.plot(error)
    plt.title("Training Iterations vs. Error Rate")
    plt.xlabel("Iteration")
    plt.ylabel("MSE (Mean Squared Error)")
    plt.show()
    return error

def output(nodefile,outputfile, overall = True):
    f = open(nodefile,"rU")
    node_reader = csv.reader(f)
    nodes = [str(x[0]) for x in node_reader if x]
    print nodes
    output_file = open(outputfile, "rU")
    output_reader = csv.reader(output_file)
    results = []
    sumtotal = 0
    for x in output_reader:
        if x:
            if str(x[0]) in nodes:
                if not results:
                    i = 0
                else:
                    i = i +1
                results.append([])
                sumtotal = 0
        if len(x) == 4:
            try:
                sumtotal = sumtotal + float(x[3])
                results[i].append(sumtotal)
            except:
                None

    for x in zip(results,nodes):
        plt.plot(x[0], label = x[1])
    plt.title("Net Profit/Loss by Node over Testing Period")
    plt.xlabel("Time (Hours)")
    plt.ylabel("Net Profit/Loss")

    if overall:
        pl = []
        for i in range(len(results[0])):
            temp = 0
            for x in results:
                temp = temp + x[i]
            pl.append(temp)
        plt.plot(pl, label = "AGGREGATE")
    plt.legend(loc = 4, prop={'size':10})


    plt.show()

def sumof_list(data):
    total = 0
    for x in data:
        total = total + x
    return total
                              
           
o = output("node.txt","Output/output.csv")
