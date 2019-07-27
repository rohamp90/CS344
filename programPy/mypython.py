import os       #for the os.system function
import string   #to get string functions
import random   #to get random functions

# Empty String Variable
filename1 = ""
filename2 = ""
filename3 = ""

# Generate Random String of length 10
#source https://pynative.com/python-generate-random-string/
def randomString(stringLength=10):
    """Generate a random string of fixed length """
    letters = string.ascii_lowercase
    return ''.join(random.choice(letters) for i in range(stringLength))

filename1 = randomString(10)
filename2 = randomString(10)
filename3 = randomString(10) 

#echo names of files to console
cmd1 = "echo " + filename1
cmd2 = "echo " + filename2
cmd3 = "echo " + filename3
os.system(cmd1)
os.system(cmd2)
os.system(cmd3)

# create 3 files
cmd1 = cmd1 + " > " + filename1
cmd2 = cmd2 + " > " + filename2
cmd3 = cmd3 + " > " + filename3
os.system(cmd1)
os.system(cmd2)
os.system(cmd3)

# Generate Random Numbers
num1 = random.randint(1,42)
num2 = random.randint(1,42)

# Print out random num to console
cmd4 = "echo " + str(num1)  #cast int to string
os.system(cmd4)
cmd4 = "echo " + str(num2)
os.system(cmd4)

product = num1 * num2 # mult num1 and num2

cmd4 = "echo " + str(product)
os.system(cmd4)