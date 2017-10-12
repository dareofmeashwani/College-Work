import csv
import time
import os
class node:
    leaf=0
    parent=None
    left=None
    right=None
    key=None
    label=[]
    height=0
interval=[]
info=[]
over=[]
songlist=[]
clique=[]
mylist=[]
tree=None
matrix=None
def insertinterval():
    com_name=raw_input("Enter Composer Name  : ")
    com_interval=raw_input("Enter Composer lifespan In this format XXXX-ZZZZ  : ")
    temp=[]
    l=len(info)
    temp.append(str(l+1))
    temp.append(com_name.replace(' ', ''))
    temp.append(com_interval.replace(' ', ''))
    info.append(temp)
    com_interval = com_interval.replace(' ', '')
    position=com_interval.index('-')
    start=com_interval[:position]
    end=com_interval[position+1:]
    if int(start) not in interval:
        insertion(int(start))
    if int(end) not in interval:
        insertion(int(end))
    temptree=None
    l=len(interval)
    temp1=interval[:(l//2)]
    temp2=interval[(l//2):]
    for i in range(len(temp1)):
        temptree=insert(temptree,temp1[i])
    for i in range(len(temp2)):
        temptree=insert(temptree,temp2[i])
    constructleafnode(temptree)
    global tree
    tree=temptree
    for i in range(len(info)):
        com_interval=info[i][2]
        position=str(com_interval).index('-')
        start=int(com_interval[:position])
        end=int(com_interval[position+1:])
        addInterval( str(info[i][1]),start,end,-99999999,99999999, tree )
    generatematrix()    
def builtit(filename):
    i=0
    f = open( filename, 'rt' )
    reader = csv.reader(f)
    for row in reader:
        i=i+1
        if i==1:
            continue
        r0=row[0]
        r1=row[1]
        r2=row[2]
        temp=[]
        temp.append(r0.replace(' ', ''))
        temp.append(r1.replace(' ', ''))
        temp.append(r2.replace(' ', ''))
        info.append(temp)        
        contructinterval(r2)
    f.close
    global tree
    l=len(interval)
    temp1=interval[:(l//2)]
    temp2=interval[(l//2):]
    for i in range(len(temp1)):
        tree=insert(tree,temp1[i])
    for i in range(len(temp2)):
        tree=insert(tree,temp2[i])
    constructleafnode(tree)
    for i in range(len(info)):
        com_interval=info[i][2]
        position=str(com_interval).index('-')
        start=int(com_interval[:position])
        end=int(com_interval[position+1:])
        addInterval( str(info[i][1]),start,end,-99999999,99999999, tree )
    resolvesonglist(filename)
    insertsongsintree(filename)
    generatematrix()    
def findoverlaps2():
    global mylist
    del mylist[:]
    com_interval=raw_input("Enter Composer Interval In this format XXXX-ZZZZ  : ")
    com_interval=com_interval.replace(' ', '')
    position=com_interval.index('-')
    start=int(com_interval[:position])
    end=int(com_interval[position+1:])
    k=-1
    for i in range(len(interval)):
        if interval[i]>end:
            k=i
            break
        if interval[i]==end:
            k=i+1
            break
    findinterval(start,interval[k],-99999999,99999999, tree )
    mylist=uniq(mylist)[:]
    print mylist
def findoverlaps(choice=0):
    global mylist
    del mylist[:]
    com_interval=raw_input("Enter Composer Interval In this format XXXX-ZZZZ  : ")
    com_interval=com_interval.replace(' ', '')
    position=com_interval.index('-')
    starti=int(com_interval[:position])
    endi=int(com_interval[position+1:])
    l=len(info)
    temp=[]
    for i in range(l):
	com_interval=info[i][2]
        position=str(com_interval).index('-')
        start=int(com_interval[:position])
        end=int(com_interval[position+1:])
	if ((starti-end)<0 and starti>start) or ((start-endi)<0 and endi<end) or (starti>=start and endi<=end):
	    temp.append(info[i][1])
    mylist=uniq(temp)[:]
    for i in range(len(mylist)):
	for j in range(l):
	    if mylist[i]==info[j][1] and choice==0:
		print info[j][1],info[j][2]
def maxoverlaps():
    l=len(info)
    ar=[0]*l
    global mylist
    maximum=0
    for i in range(l):
        com_interval=info[i][2]
        position=str(com_interval).index('-')
        start=int(com_interval[:position])
        end=int(com_interval[position+1:])
        #listIntervals((start+end)//2,tree,0)
        del mylist[:]
        findinterval(start,end,-99999999,99999999, tree )
        mylist=uniq(mylist)[:]
        for j in range(l):
             if info[j][1] in mylist:
                 ar[j]=ar[j]+1
    for i in range(l):
        if ar[maximum]<ar[i]:
             maximum=i
    print info[maximum]
def findsong():
    del over[:]
    com_interval=raw_input("Enter Composer Interval In this format XXXX-ZZZZ  : ")
    com_interval=com_interval.replace(' ', '')
    position=com_interval.index('-')
    start=int(com_interval[:position])
    end=int(com_interval[position+1:])
    listIntervals((start+end)//2,tree,0)
    for i in range(len(over)):
        print "Composer Name :  "+str(over[i])
        for j in range(len(info)):
            if info[j][1].replace(" ","")==over[i].replace(" ",""):
                for k in range(len(songlist[j])):
                    print songlist[j][k]
def builtig():
    temptree=centeredtree(interval)
    constructleafnode(temptree)
    for i in range(len(info)):
        com_interval=info[i][2]
        position=str(com_interval).index('-')
        start=int(com_interval[:position])
        end=int(com_interval[position+1:])
        addInterval( str(info[i][1]),start,end,-99999999,99999999, temptree )
    fg=open("treegraphic.dot","w")
    fg.write("digraph Tree { \n")
    generatetreegraphic(temptree,fg)
    fg.write("}\n")
    fg.close
    fg=open("intervalgraph.dot","w")
    fg.write("graph intervalgraph { \n")
    generategraphgraphic(fg)
    fg.write("}\n")
    fg.close            
def biggestteam():
    temp=[]
    for i in range(len(info)):
        temp.append(info[i][1].replace(" ",""))
    BronKerbosch1([],temp,[])
    l=len(clique)
    status=[]
    j=0	
    while j<l:
	gli=len(clique[j])
	status.append(gli)
	j=j+1
    temp=clique[status.index(max(status))]
    print "Biggest Team : ",temp
    del clique[:]
    l=len(info)
    start=-1
    end=-1
    for i in range(len(temp)):
	for j in range(l):
	    if info[j][1]==temp[i]:
		break
	com_interval=info[j][2]
	position=com_interval.index('-')
	starti=int(com_interval[:position])
	endi=int(com_interval[position+1:])
	if i==0:
	    start=starti
	    end=endi
	    continue
	if (starti-end)<0 and starti>start:
	    start=starti
	elif (start-endi)<0 and endi<end:
	    end=endi
	elif (starti>=start and endi<=end):
	    start=starti
	    end=endi
    print str(start)+"-"+str(end)	
def findgroup():
    l=len(info)
    status=[0]*l
    group=[]
    for i in range(l):
        if status[i]==0:
            status[i]=i+1
            temp=adajency_dfs(status,i)
            status=temp[:]
    for i in range(l):
        temp=[]
        for j in range(l):
            if status[j]==i:
                temp=temp[:]+[info[j][1]]
        group.append(temp)
    k=l-1
    while k>=0:
	gl=len(group[k])
	if gl==0:
	    group.pop(k)
	k=k-1
    l=len(group)
    status=[]
    j=0	
    while j<l:
	gli=len(group[j])
	status.append(gli)
	j=j+1
    print "Maximum Group : ",group[status.index(max(status))]
    print "Minimum Group : ",group[status.index(min(status))]
def centeredtree(mlist):
	length=len(mlist)
	if length==0:
		return
	if length==1:
		temp=node()
		temp.key=mlist[0]
		return temp
	else:
		element=mlist[length//2]
		temp1=mlist[:(length//2)]
		temp2=mlist[(length//2)+1:]
		add1=None
		add2=None
		if len(temp1)>0:
			add1=centeredtree(temp1)
		if len(temp2)>0:
			add2=centeredtree(temp2)	
		temp=node()
		temp.key=element
		temp.left=add1
		temp.right=add2
		if add1!=None: add1.parent=temp
		if add2!=None: add2.parent=temp
		return temp            
def uniq(mylist):
  output = []
  for x in mylist:
    if x not in output:
      output.append(x)
  return output    
def contructinterval(element):
    element = element.replace(' ', '')
    position=element.index('-')
    start=element[:position]
    end=element[position+1:]
    insertion(int(start))
    insertion(int(end))
def insertion(element):
    position=len(interval)
    if position!=0:
        i=position-1
        while i>=0:
            if interval[i]>element:
                position=i
            elif interval[i]==element:
                interval.pop(i)
                position=i
                break
            i=i-1
    interval.insert(position,element)
def insert(root,key):
    if root == None:
        temp=node()
        temp.key=key
        return temp
    if key < root.key:
        root.left  = insert(root.left,key)
        root.left.parent=root
    else:
        root.right = insert(root.right,key)
        root.right.parent=root
    root.height = maximum(height(root.left), height(root.right)) + 1
    balance = getBalance(root)
    if balance>1 and key < root.left.key:
        return rightRotate(root)
    if balance < -1 and key > root.right.key:
        return leftRotate(root)
    if balance > 1 and key > root.left.key:
        root.left =  leftRotate(root.left)
        return rightRotate(root)
    if balance < -1 and key < root.right.key:
        root.right = rightRotate(root.right)
        return leftRotate(root)
    return root
def getBalance(root):
    if root==None:
        return 0
    return height(root.left) - height(root.right)
def leftRotate(root):
    if root!=None:
        temp=root.parent
        y = root.right
        T2 = y.left
        y.left = root
        root.parent=y
        root.right = T2
        if T2!=None: T2.parent=root
        root.height = maximum(height(root.left), height(root.right))+1
        y.height = maximum(height(y.left), height(y.right))+1
        y.parent=temp
        return y
def rightRotate(y):
    if y!=None:
        temp=y.parent
        x = y.left
        T2 = x.right
        x.right = y
        y.parent=x
        y.left = T2
        if T2!=None: T2.parent=y
        y.height = maximum(height(y.left), height(y.right))+1
        x.height = maximum(height(x.left), height(x.right))+1
        x.parent=temp
        return x
def height(root):
    if root == None:
        return 0
    return root.height
def maximum(a,b):
    if a>b:
        return a
    else:
        return b
def displaytree(root):
    if root!=None:
        displaytree(root.left)
        print str(root.key)+"  parent:"+str(root.parent)+"  root:"+str(root)+"  right:"+str(root.right)+"  left:"+str(root.left) +"  key:"+str(root.key)+"  height:"+str(root.height)
        print root.label
        displaytree(root.right)
def inordersuccessor(root,n):
    if n.right!=None:
        return minValue(n.right)
    p = n.parent
    while p != None and n == p.right:
        n = p
        p = p.parent
    return p
def minValue(root):
    current =root
    while current.left != None:
        current = current.left
    return current
def subinterval(start,end):
    l=len(interval)
    startpos=-1
    endpos=-1
    for i in range(l):
        if start>=interval[0] and end<=interval[l-1]:
            if i==0:
                s=1
            else:
                s=i
            if i==l:
                t=l-1
            else:
                t=i
            if start>=interval[i] and start<interval[t+1]:
                startpos=i
            if end>interval[s-1] and end<=interval[i]:
                endpos=i
        elif start<interval[0] and end<=interval[l-1]:
            if i==0:
                s=1
            else:
                s=i
            if end>interval[s-1] and end<=interval[i]:
                endpos=i
        elif start>=interval[0] and end>interval[l-1]:
            if i==l:
                t=l-1
            else:
                t=i
            if start>=interval[i] and start<interval[t+1]:
                startpos=i
    inter=None
    if endpos==-1  and startpos==-1:
        inter=[int(interval[0])-1]+interval+[int(interval[l-1])+1]
    elif endpos!=-1 and startpos==-1:
        inter=[int(interval[0])-1]+interval[:endpos+1]
    elif endpos==-1 and startpos!=-1:
        inter=interval[startpos+1:]+[int(interval[l-1])+1]
    else:
        inter=interval[startpos:endpos+1]
    return inter
def constructleafnode(root):
    if root==None:
        return
    if root.leaf==0:
        if root.left==None:
            left=node()
            left.leaf=1
            left.parent=root
            root.left=left
        if root.right==None:
            right=node()
            right.leaf=1  
            right.parent=root
            root.right=right
        constructleafnode(root.left)
        constructleafnode(root.right)
def listIntervals( k,root,mode):
    global over
    while root != None:
        if len(root.label)!=0:
            if len(over)==0:
                over=root.label[:]
            else:
                over=over+root.label[:]
            if mode==1:
                print root.label
        if k < root.key:
            root =root.left
        else:
            root = root.right
def findinterval(a, b, minimum,maximum, x ):
    if x==None:
        return
    if a <= minimum and maximum <= b:
        global mylist
        mylist=mylist+x.label[:]
    if a < x.key and x.leaf==0:
            findinterval(a, b, minimum,x.key,x.left )
    if x.key < b and x.leaf==0:
            findinterval(a, b, x.key, maximum, x.right )
def addInterval( I, a, b, minimum,maximum, x ):
    if a <= minimum and maximum <= b:
        l=len(x.label)
        if l==0:
            x.label=[I]
        else:
            x.label=[I]+x.label
    else:
        if a < x.key and x.leaf==0:
            addInterval( I, a, b, minimum,x.key,x.left )
        if x.key < b and x.leaf==0:
            addInterval( I, a, b, x.key, maximum, x.right )
def insertsongsintree(filename):
    f = open( filename, 'rt' )
    reader = csv.reader(f)
    i=0
    global songlist
    for row in reader:
        i=i+1
        if i==1:
            continue
        temp=row[3:]
        temp1=[]
        for i in range(len(temp)):
            temp1=temp1+[["genre "+str(i+1)]+[temp[i]]]
        songlist=songlist+[temp1]
def resolvesonglist(filename):
    fr=open(filename,'r')
    fw=open("temp.csv",'w')
    for line in fr:
        temp=line.replace(' "','"')
        fw.write(temp)
    fw.close
    fr.close
    os.remove(filename)
    os.rename("temp.csv",filename)
def generatetreegraphic(root,fg):
    if root==None:
        return
    if root.leaf==0:        
        temproot=str(root)[29:-2]
        fg.write(temproot+' [label="'+str(root.key)+" "+str(root.label)+'"'+' shape=circle ]')
        fg.write("\n")
        if root.left!=None:
            templeft=str(root.left)[29:-2]
            fg.write(str(temproot)+"->"+str(templeft))
            fg.write("\n")           
        if root.right!=None:
            tempright=str(root.right)[29:-2]
            fg.write(str(temproot)+"->"+str(tempright))
            fg.write("\n")
    if root.leaf==1:
        temproot=str(root)[29:-2]
        fg.write(temproot+' [label="'+str(root.label)+'"'+' shape=box ]')
        fg.write("\n")
    generatetreegraphic(root.left,fg)
    generatetreegraphic(root.right,fg)
def generatematrix():
    l=len(info)
    global matrix
    if matrix==None: matrix=[]
    del matrix[:]
    for i in range(l):
        global mylist
        table=[0]*l
        com_interval=info[i][2]
        position=str(com_interval).index('-')
        start1=int(com_interval[:position])
        end1=int(com_interval[position+1:])
        del mylist[:]
        findinterval(start1,end1,-99999999,99999999, tree )
        mylist=uniq(mylist)[:]
        j=0
        while j <l:
            k=0
            while k <len(mylist):
                if info[j][1].replace(" ","")==mylist[k].replace(" ",""):
                    table[j]=1
                k=k+1
            if i==j:
                table[j]=0
            j=j+1
        matrix.append(table)
    tempmatrix=[]
    for i in range(l):
        table=[0]*l
        for j in range(l):
	    if (matrix[i][j]==1 or matrix[j][i]==1):
			table[j]=1
	tempmatrix.append(table)
    matrix=tempmatrix[:]
def generategraphgraphic(fg):
    global matrix
    l=len(info)
    for i in range(l):
        fg.write(str(info[i][1])+' [label="'+str(info[i][1])+'"'+' shape=circle ]')
        fg.write("\n")
        for j in range(l):
            if (matrix[i][j]==1 or matrix[j][i]==1) and i<j :
                fg.write(str(info[i][1])+"--"+str(info[j][1]))
                fg.write("\n")
def adajency_dfs(status,k):
    l=len(info)
    adaj=matrix[k]
    temp=None
    for i in range(l):
        if adaj[i]==1 and status[i]==0:
            status[i]=k+1
            temp=adajency_dfs(status,k)
            status=temp[:]
    return status
def BronKerbosch1(R, P, X):
    lp=len(P)
    lx=len(X)
    if lp==0 and lx==0:
        global clique
        clique.append(R)
        return
    while 1:
        if len(P)==0:
            break
        v=P[0]
        temp1=R+[v]
        temp1=uniq(sorted(temp1))[:]
        n=findneighbour(v)
        n=uniq(sorted(n))[:]
        BronKerbosch1(temp1, uniq(sorted(list(set(P) & set(n))))[:],uniq(sorted(list(set(X) & set(n))))[:])
        P.pop(0)
        X=uniq(sorted(X+[v]))[:]
def findneighbour(v):
    l=len(info)
    for i in range(l):
        if info[i][1].replace(" ","")==v:
            break
    temp=matrix[i]
    neighbour=[]
    for i in range(l):
        if temp[i]==1:
            neighbour.append(info[i][1])
    return neighbour
while 1:
    print 'Enter Your choice:\n'
    print '0  for Exit'
    print '1  for BuiltIT()'
    print '2  for Insert()'
    print '3  for FindOverlaps()'
    print '4  for MaxOverlaps()'
    print '5  for FindSongs()'
    print '6  for BuiltIG()'
    print '7  for BiggestTeam()'
    print '8  for FindGroup()'
    try:
        choice=input("\t::")
        if int(choice)==0:
            break
        elif int(choice)==1:
            filelink=raw_input("Enter Input CSV Filename : ")
            builtit(str(filelink))
        elif int(choice)==2:
            insertinterval()        
        elif int(choice)==3:
            findoverlaps()
        elif int(choice)==4:
            maxoverlaps()
        elif int(choice)==5:
            findsong()
        elif int(choice)==6:
            builtig()
        elif int(choice)==7:
            biggestteam()
        elif int(choice)==8:
            findgroup()
        elif int(choice)==9:
            print info
        elif int(choice)==10:
            displaytree(tree)
        elif int(choice)==11:
            print interval
        else:        
            print "YOUR ENTER INVALID CHOICE"
    except Exception:
        print "YOUR ENTER INVALID CHOICE"
    
