#Mark Boady
#August 2024

#This python script can be used to test the puzzle
#It is just a sanity check, it does not check every possible edge case.

#Get the file name from the command line.
import sys
if len(sys.argv)!=2:
    print("This program takes 1 command line arg. The file to open and parse.")
    sys.exit(0)
filename = sys.argv[1]

#Status of the word
#Everyone has to wake up once
people_who_woke_up = []
#The bar where -1 means seat is empty
the_bar = [-1, -1, -1, -1, -1]
#The line outside the bar
looking = []
#Make sure everyone ate
people_who_ate = []

#Read the file and check if everything makes sense.
line_counter = 0
file = open(filename,"r")
for line in file:
    line_counter+=1
    print("LINE",line_counter,":",line,end="")
    #Not a line we understand, skip
    if len(line) < 11 or line[0:6]!="Person":
        print("\tCOMMENTS: Skipped this line. It makes no sense.")
        continue
    #If you didn't follow the format, this will probably crash
    #Oh well ¯\_(ツ)_/¯
    #Get the person's ID number
    first_space = line.find(" ")
    second_space = line.find(" ",first_space+1)
    person_id = int(line[first_space:second_space+1])
    #Did someone wake up? Track that they exist now.
    if line.find("nap")>=0:
        people_who_woke_up.append(person_id)
    #Did someone get to the bar? See if they get a seat or in line
    if line.find("got")>=0:
        #Looking for a seat
        looking.append(person_id);
        print("\tCOMMENTS: This person is looking for a seat.")
    #If someone is eating, they either need to have a seat or have gotten out of line
    if line.find("started")>=0:
        #Stopped looking and hop on
        if looking.count(person_id)==1 and the_bar.count(-1) > 0:
            #Sit down!
            x = the_bar.index(-1)
            the_bar[x] = person_id
            looking.remove(person_id)
            print("\tCOMMENTS: This person stopped looking and sat down.")
        #Are you sitting at the bar?
        if(the_bar.count(person_id)==1):
            seat = the_bar.index(person_id)
            print("\tCOMMENTS: This person is sitting at",seat,".")
        else:
            print("\t**ERROR**: You said this person was eating, but they didn't seem to have a seat?")
            break#Kill the program
    #Someone Leaves
    if line.find("finished")>=0:
        #They give up their seat
        seat = the_bar.index(person_id)
        the_bar[seat]=-1
        people_who_ate.append(person_id)
        print("\tCOMMENTS: This person is giving up seat",seat,".")
    #Give a Summary
    print("\tSummary: The Bar looks like",the_bar,"(-1 means empty)")
    print("\tSummary: People looking for seats",looking)
    
file.close()

#Did Everyone Wake up?
people_who_woke_up.sort()
print("Lowest Person ID: ",people_who_woke_up[0])
print("Highest Person ID: ",people_who_woke_up[-1])
missing=False
for i in range(1,len(people_who_woke_up)-1):
    if people_who_woke_up[i]+1 != people_who_woke_up[i+1]:
        print("Missing People:",people_who_woke_up[i],"followed by",people_who_woke_up[-i])
        missing=True
if not missing:
    print("No People in Range Missing")
#Who Ate?
#Check if everyone who woke up ate
eat_error = False
for x in people_who_woke_up:
    if people_who_ate.count(x)!=1:
        print("Person",x,"woke up but didn't eat exactly once.")
        eat_error = True
if not eat_error:
    print("Everyone ate.")
print("The End")
