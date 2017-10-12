import math,os,time
heap=[]
tstat=0
tablesize=1573
table = [None] * tablesize
def put_to_heap(key):
    length=len(heap)
    if tstat==1:
        inserttohash(key)
    if length==0:
        heap.append([key])
    elif len(heap[length-1])==2:
        heap.append([key])
        return upheapify(length+1,0)
    elif len(heap[length-1])==1:
        temp=heap.pop(length-1)
        if key<temp[0]:
            temp.insert(0,key)
            heap.append(temp)
            return upheapify(length,0)
        else:
            temp.append(key)
            heap.append(temp)
            return upheapify(length,1)
def upheapify(i,position):
    if i==1:
        return
    parent=int(math.floor(i/2))
    if heap[parent-1][0]<=heap[i-1][position] and heap[parent-1][1]>=heap[i-1][position]:
        return
    elif heap[parent-1][0]> heap[i-1][position]:
        temp=heap[parent-1][0]
        heap[parent-1][0]=heap[i-1][position]
        heap[i-1][position]=temp
        return upheapify(parent,0)
    elif heap[parent-1][1]< heap[i-1][position]:
        temp=heap[parent-1][1]
        heap[parent-1][1]=heap[i-1][position]
        heap[i-1][position]=temp
        return upheapify(parent,1)
def downheapify(i):
    length=len(heap)
    left=int(2*i)
    right=int(2*i+1)
    l=len(heap[i-1])
    if l==2:
        if heap[i-1][0]>heap[i-1][1]:
            temp=heap[i-1][0]
            heap[i-1][0]=heap[i-1][1]
            heap[i-1][1]=temp
    if left>length:
        return
    if length==left:
        l=len(heap[left-1])
        if l==1:
            if heap[left-1][0]<=heap[i-1][1] and heap[left-1][0]>=heap[i-1][0]:
                return
            if heap[left-1][0]>heap[i-1][1]:
                temp=heap[left-1][0]
                heap[left-1][0]=heap[i-1][1]
                heap[i-1][1]=temp
            if heap[left-1][0]<heap[i-1][0]:
                temp=heap[left-1][0]
                heap[left-1][0]=heap[i-1][0]
                heap[i-1][0]=temp
        elif l==2:
            if heap[left-1][0]>heap[left-1][1]:
                temp=heap[left-1][0]
                heap[left-1][0]=heap[left-1][1]
                heap[left-1][1]=temp
            if heap[left-1][1]<=heap[i-1][1] and heap[left-1][0]>=heap[i-1][0]:
                return
            if heap[left-1][1]>heap[i-1][1]:
                temp=heap[left-1][1]
                heap[left-1][1]=heap[i-1][1]
                heap[i-1][1]=temp
            if heap[left-1][0]<heap[i-1][0]:
                temp=heap[left-1][0]
                heap[left-1][0]=heap[i-1][0]
                heap[i-1][0]=temp
    if right<=length:
        ll=len(heap[left-1])
        lr=len(heap[right-1])
        point=0
        if ll==2 and lr==2:
            if heap[left-1][0]>heap[left-1][1]:
                temp=heap[left-1][0]
                heap[left-1][0]=heap[left-1][1]
                heap[left-1][1]=temp
            if heap[right-1][0]>heap[right-1][1]:
                temp=heap[right-1][0]
                heap[right-1][0]=heap[right-1][1]
                heap[right-1][1]=temp
            if heap[left-1][1]<=heap[i-1][1] and heap[left-1][0]>=heap[i-1][0] and heap[right-1][1]<=heap[i-1][1] and heap[right-1][0]>=heap[i-1][0]:
                return
            if heap[left-1][0]<heap[right-1][0]:
               point=left
            else:
                point=right
            if heap[point-1][0]<heap[i-1][0]:
                temp=heap[point-1][0]
                heap[point-1][0]=heap[i-1][0]
                heap[i-1][0]=temp
                downheapify(point)
            if heap[left-1][1]>heap[right-1][1]:
               point=left
            else:
                point=right
            if heap[point-1][1]>heap[i-1][1]:
                temp=heap[point-1][1]
                heap[point-1][1]=heap[i-1][1]
                heap[i-1][1]=temp
                downheapify(point)    
        elif ll==2 and lr==1:
            if heap[left-1][0]>heap[left-1][1]:
                temp=heap[left-1][0]
                heap[left-1][0]=heap[left-1][1]
                heap[left-1][1]=temp
            if heap[left-1][1]<=heap[i-1][1] and heap[left-1][0]>=heap[i-1][0] and heap[right-1][0]<=heap[i-1][1] and heap[right-1][0]>=heap[i-1][0]:
                return
            if heap[left-1][0]<heap[right-1][0]:
               point=left
            else:
                point=right
            if heap[point-1][0]<heap[i-1][0]:
                temp=heap[point-1][0]
                heap[point-1][0]=heap[i-1][0]
                heap[i-1][0]=temp
                downheapify(point)
            if heap[left-1][1]>heap[right-1][0]:
               point=left
               position=1
            else:
                point=right
                position=0
            if heap[point-1][position]>heap[i-1][1]:
                temp=heap[point-1][position]
                heap[point-1][position]=heap[i-1][1]
                heap[i-1][1]=temp
                downheapify(point)
def removemin():
    length=len(heap)
    if length==0:
        return None
    elif len(heap[length-1])==2:
        x=heap[0][0]
        heap[0][0]=heap[length-1][0]
        heap[length-1].pop(0)
        if size_of_heap()>2:        
            downheapify(1)
        if tstat==1:
            removefromhash(x)
        return x
    elif len(heap[length-1])==1:
        x=heap[0][0]
        heap[0][0]=heap[length-1][0]
        heap.pop(length-1)
        if size_of_heap()>2:        
            downheapify(1)
        if tstat==1:
            removefromhash(x)
        return x
def removemax():
    length=len(heap)
    if length==0:
        return None
    elif len(heap[length-1])==2:
        x=heap[0][1]
        heap[0][1]=heap[length-1][1]
        heap[length-1].pop(1)
        if size_of_heap()>2:        
            downheapify(1)
        if tstat==1:
            removefromhash(x)
        return x
    elif len(heap[length-1])==1:
        x=heap[0][0]
        heap.pop(length-1)
        if size_of_heap()>2:       
            downheapify(1)
        if tstat==1:
            removefromhash(x)
        return x
def size_of_heap():
    length=len(heap)
    if length==0:
        return 0
    elif len(heap[length-1])==2:
        return 2*length
    elif len(heap[length-1])==1:
        return 2*length-1
def getmin():
    length=len(heap)
    if length==0:
        return 'no element in heap'
    return heap[0][0]
def getmax():
    length=len(heap)
    if length==0:
        return 'no element in heap'
    else:
        l=len(heap[0])
        if l==2:
            return heap[0][1]
        elif l==1:
            return heap[0][0]
def isempty():
    length=len(heap)
    if length==0:
        return True
    else:
        return False
def builddepq(filename):
    file=open(filename,"r")
    for line in file:
        l=len(line)
        line=line[:l-1]
        if tstat==1:
            inserttohash(line)
        length=len(heap)
        if length==0:
            heap.append([line])
        elif len(heap[length-1])==2:
            heap.append([line])
        elif len(heap[length-1])==1:
            if heap[length-1][0]<line:
                heap[length-1].append(line)
            else:
                heap[length-1].insert(0,line)
    file.close
    length=len(heap)
    while (length >0):
        downheapify(length)
        length=length-1
def hashfunction1(string):
    hash = 5381;
    for c in string:
        x=int(ord(c))
        hash =((hash*37+hash+hash*41+x)+(hash*37+hash+hash*41+x)*65336+(hash*37+hash+hash*41+x)/65336)%tablesize
    return hash
def hashfunction2(string):
    hash = 5381;
    for c in string:
        x=int(ord(c))
        hash =((hash*577+hash+hash*37+x)+(hash*113+hash+hash*71+x)*4096+(hash*37+hash+hash*41+x)/65336)%tablesize
    return int(hash)
def inserttohash(line):
    key=hashfunction1(line)
    if table[key]==None:
        table.pop(key)
        table.insert(key,[line])
    else:
        table[key].append(line)
def removefromhash(line):
    key1=hashfunction1(line)  
    if table[key1]==None:
        return None
    else:
        x=-1
        try:
            x=table[key1].index(line)
        except Exception:
            x=-1
        if x>=0:
            table[key1].pop(x)
        else:
            print 'not found'
def iscontain(line):
    key1=hashfunction1(line) 
    if table[key1]==None:
        return False
    else:
        x=-1
        try:
            x=table[key1].index(line)          
        except Exception:
            x=-1
        if x>=0:
            return True
        else:
            return False
def quicksort(filename,index=1):
    rightfile=open("temp"+str(2*index+1)+".txt","w")    
    rightfile.close
    leftfile=open("temp"+str(2*index)+".txt","w")
    leftfile.close
    countl=0
    countr=0
    file=open(filename,"r")
    for lines in file:
        if size_of_heap()<10:
            put_to_heap(lines)
        else:
            x=getmin()
            y=getmax()
            if lines<=x:
                leftfile=open("temp"+str(2*index)+".txt","a+")
                leftfile.write(lines)
                leftfile.close
                countl=countl+1
            elif lines>=y:
                rightfile=open("temp"+str(2*index+1)+".txt","w")                
                rightfile.write(lines)
                rightfile.close
                countr=countr+1
            elif lines>x and lines<y:
                z=removemin()
                put_to_heap(lines)
                leftfile=open("temp"+str(2*index)+".txt","a+")
                leftfile.write(z)
                leftfile.close
                countl=countl+1  
    file.close
    #time.sleep(200)
    if countl>0:
        quicksort("temp"+str(2*index)+".txt",2*index)
    if countr>0:
        quicksort("temp"+str(2*index+1)+".txt",2*index+1)
    #print "countl "+str(countl)+" countr "+str(countr)
    file=open(filename,"w")
    if countl>0:
        leftfile=open("temp"+str(2*index)+".txt","r")
        for lines in leftfile:
            file.write(lines)
        leftfile.close
    os.remove("temp"+str(2*index)+".txt")  
    while 1: 
        x=removemin()
        if x==None:
            break
        file.write(str(x))
    if countr>0:
        rightfile=open("temp"+str(2*index+1)+".txt","r")
        for lines in rightfile:
            file.write(lines)
            rightfile.close
    os.remove("temp"+str(2*index+1)+".txt")
    file.close
while 1:
    print 'Enter Your choice:\n'
    print '0  for External quicksort Using DEPQ'
    print '1  for IsEmpty'
    print '2  for Size'
    print '3  for GetMin'
    print '4  for GetMax'
    print '5  for Put(x)'
    print '6  for BuildDEPQ'
    print '7  for RemoveMin'
    print '8  for RemoveMax'
    print '9  for IsContain'
    print '10 for display heap'
    print '11 for Enable HashTable For heap'
    print '-1 for EXIT'
    choice=input("\t::")
    if int(choice)==-1:
        break
    elif int(choice)==0:
        filelink=raw_input("Enter Input Filename : ")
        del heap[:]
        quicksort(filelink,1)
        del heap[:]
    elif int(choice)==1:
        print "heap is empty "+str(isempty())
    elif int(choice)==2:
        print 'heap size : '+str(size_of_heap())        
    elif int(choice)==3:
        print 'minimum value in heap : '+str(getmin())
    elif int(choice)==4:
        print 'maximum value in heap : '+str(getmax())
    elif int(choice)==5:
        print "enter word that you want to insert"
        key=raw_input("\t: ")
        put_to_heap(key)
    elif int(choice)==6:
        filelink=raw_input("Enter Input Filename : ")
        builddepq(filelink)
    elif int(choice)==7:
        print "removed max element "+str(removemin())
    elif int(choice)==8:
        print "removed max element "+str(removemax())
    elif int(choice)==9:
        print "enter word that you want to search"
        key=raw_input("\t: ")
        print "heap contion "+key+" is "+str(iscontain(key))
    elif int(choice)==10:
        print heap
    elif int(choice)==11:
        if tstat==0:
            x=raw_input(" 1 for yes \n 0 for no\n\t::")
            tstat=int(x)
    else:        
        print "YOUR ENTER INVALID CHOICE"
