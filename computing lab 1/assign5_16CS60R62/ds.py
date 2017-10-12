import math
heap=[]
def heappush(heap, val):
    cur = len(heap)
    heap.append(val)
    while cur > 0:
        parent = (cur - 1) // 2
        if heap[parent] <= heap[cur]: break
        heap[cur], heap[parent] = heap[parent], heap[cur]
        cur = parent
        pass
    pass
def heappop(heap):
    ret = heap[0]
    last = heap.pop()
    size = len(heap)
    if size == 0:
        return ret
    heap[0] = last
    cur = 0
    while True:
        ch1 = 2 * cur + 1
        if ch1 >= size: return ret
        ch2 = ch1 + 1
        child = ch2 if ch2 < size and heap[ch2] < heap[ch1] else ch1
        if heap[cur] <= heap[child]: return ret
        heap[child], heap[cur] = heap[cur], heap[child]
        cur = child
        pass
    pass
def put_to_heap(key):
    length=len(heap)
    print key+"   "+str(length)    
    if length==0:
        heap.append([key])
    elif len(heap[length-1])==2:
        heap.append([key])
        return upheapify(length,0)
    elif len(heap[length-1])==1:
        temp=heap.pop(length-1)
        if key<temp[0]:
            temp.insert(0,key)
            heap.append(temp)
            return upheapify(length-1,0)
        else:
            temp.append(key)
            heap.append(temp)
            return upheapify(length-1,1)
def upheapify(i,position):
    parent=int(math.floor(i/2))
    if heap[parent][0]<=heap[i][position] and heap[parent][1]>=heap[i][position]:
        return
    elif heap[parent][0]> heap[i][position]:
        temp=heap[parent][0]
        heap[parent][0]=heap[i][position]
        heap[i][position]=temp
        return upheapify(parent,0)
    elif heap[parent][1]< heap[i][position]:
        temp=heap[parent][1]
        heap[parent][1]=heap[i][position]
        heap[i][position]=temp
        return upheapify(parent,1)
def downheapify(i,position):
    left=int(2*i)
    left=int(2*i+1)
    if heap[i][0]<=heap[i][position] and heap[i][1]>=heap[i][position]:
        return
    elif heap[i][0]> heap[i][position]:
        temp=heap[parent][0]
        heap[parent][0]=heap[i][position]
        heap[i][position]=temp
        return upheapify(parent,0)
    elif heap[parent][1]< heap[i][position]:
        temp=heap[parent][1]
        heap[parent][1]=heap[i][position]
        heap[i][position]=temp
        return upheapify(parent,1)
def removemin():
    length=len(heap)
    if length==0:
        return None
    else:
        x=heap[0][0]
        heap[0][0]=heap[length-1][0]
        heap.pop(length-1)
        downheapify(0,0)
        return x
def removemax():
    length=len(heap)
    if length==0:
        return None
    elif len(heap[length-1])==2:
        x=heap[0][1]
        heap[0][1]=heap[length-1][1]
        heap[length-1].pop(1)
        downheapify(0,1)
        return x
    elif len(heap[length-1])==1:
        x=heap[0][1]
        heap[0][1]=heap[length-1][0]
        heap.pop(length-1)
        downheapify(0,1)
        return x
def size_of_heap():
    length=len(heap)
    if length==0:
        return 0
    elif len(heap[length-1])==2:
        return 2*length
    elif len(heap[length-1])==1:
        return 2*length-1
def builddepq():
    return 0
def getmin():
    length=len(heap)
    if length==0:
        return 'no element in heap'
    return heap[0][0]
def getmax():
    length=len(heap)
    if length==0:
        return 'no element in heap'
    return heap[0][1]
def isempty():
    length=len(heap)
    if length==0:
        return True
    else:
        return False
def quicksort(filename='wl.txt',index=1):
    file=open(filename,"r")
    for lines in file:
	if size_of_heap()<=100 :
	    put_to_heap(lines)
	else:
	    x=getmin()
	    y=getmax()
	    if lines<x:
		leftfile=open(str(2*index)+".txt","w")
		leftfile.write(lines)
		leftfile.close
	    elif lines>y:
		rightfile=open(str(2*index+1)+".txt","w")
		rightfile.write(lines)
		rightfile.close
	    else:
		z=removemin()
		put_to_heap(lines)
		leftfile=open(str(2*index)+".txt","w")
		leftfile.write(z)
		leftfile.close
    file.close
    quicksort(str(2*index)+".txt",2*index)
    quicksort(str(2*index+1)+".txt",2*index+1)
    file=open(filename,"w")
    file1=open(str(2*index)+".txt","r")
    for lines in file1:
        file.write(lines)
    file1.close
    ###### remove min till priority queue is empty written in file
    file1=open(str(2*index+1)+".txt","r")
    for lines in file1:
        file.write(lines)
    file1.close
    file.close
heap.append(['abc','abd'])
while 1:
    print 'Enter Your choice:\n'
    print '0 for External quicksort Using DEPQ'
    print '1 for IsEmpty'
    print '2 for Size'
    print '3 for GetMin'
    print '4 for GetMax'
    print '5 for Put(x)'
    print '6 for BuildDEPQ'
    print '7 for RemoveMin'
    print '8 for RemoveMax'
    print '9 for IsContain'
    print '10 for display heap'
    print '-1 for EXIT-1'
    choice=input("\t::")
    if int(choice)==-1:
        break
    elif int(choice)==0:
        filelink=raw_input("Enter Input Filename : ")
        a=1
        quicksort(filelink,a)
        del heap[:]
    elif int(choice)==1:
        print "heap is empty "+str(isempty())
    elif int(choice)==2:
        print 'heap size :'+str(size_of_heap())        
    elif int(choice)==3:
        print 'minimum value in heap : '+str(getmin())
    elif int(choice)==4:
        print 'maximum value in heap : '+str(getmax())
    elif int(choice)==5:
        print "enter word that you want to insert"
        key=raw_input("\t:")
        put_to_heap(key)
    elif int(choice)==6:
        filelink=raw_input("Enter Input Filename : ")
        builddepq()
    elif int(choice)==7:
        print "removed max element "+str(removemin())
    elif int(choice)==8:
        print "removed max element "+removemax()
    elif int(choice)==9:
        print "iscontain"
    elif int(choice)==10:
        print "display"
    else:
        print "YOUR ENTER INVALID CHOICE"
