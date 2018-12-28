from tkinter import *

op=open("hfmTree.txt",'r');
nodes=[]
for line in op:
    nodes.append(line.split())

#############SMALL GRAPH################
#window=Tk()
#height=1500
#width=2000
#canvas=Canvas(window,width=height,height=height)
########################################



#############BIG GRAPH##################
window=Tk()
height=1000
width=10000
xscrollbar = Scrollbar(window,orient=HORIZONTAL)
yscrollbar = Scrollbar(window)
canvas=Canvas(window,width=1920,height=1080,xscrollcommand=xscrollbar.set,yscrollcommand=yscrollbar.set)
xscrollbar.config(command=canvas.xview)
xscrollbar.pack(side=BOTTOM, fill=X)
yscrollbar.config(command=canvas.yview)
yscrollbar.pack(side=RIGHT, fill=Y)
canvas.pack()

f=Frame(canvas)

canvas.pack(side="top", fill="both", expand=True)

canvas.create_window(0,0,window=f, anchor='nw')

window.update()
canvas.config(scrollregion=(0,0,width+100,height+100))

########################################


x,y=width/2,0
queue=[]
tmp=nodes[len(nodes)-1]
tmp.append(x)
tmp.append(y)
tmp.append(1)
queue.append(tmp)
current_h=1
def tree_height(nodes):
    max_height=0
    for node in nodes:
        h=0
        if node[1]=='-1' and node[2]=='-1':
            while node[3]!='-1':
                node=nodes[eval(node[3])]
                h+=1
            if h>max_height:
                max_height=h
        else:
            break
    return max_height

x,y=width/2,40

def isempty(queue):
    cnt=0
    l=len(queue)
    for node in queue:
        if node[0]=='-1' and node[1]=='-1' and node[2]=='-1' and node[3]=='-1':
            cnt+=1
    if cnt!=l:
        return False
    else:
        return True
while isempty(queue)==False:
    node=queue.pop(0)
    if current_h!=node[-1]:
        current_h+=1
        x=width/2**current_h
        y=height/tree_height(nodes)*(current_h-1)
    elif node[-1]!=1:
        x+=width/2**current_h*2

    if node[0]=='-1' and node[1]=='-1' and node[2]=='-1' and node[3]=='-1':
        pass
    else:
        if node[-1]!=1:
            canvas.create_line(node[-3],node[-2],x,y)
        canvas.create_text(x,y,text="%.0lf"%eval(node[0]),font = "time 20 bold underline")


    if node[1]!='-1':
        tmp=nodes[eval(node[1])]
        tmp.append(x)
        tmp.append(y)
        tmp.append(node[-1]+1)
        queue.append(tmp)
    else:
        tmp=['-1','-1','-1','-1',x,y,node[-1]+1]
        queue.append(tmp)
    if node[2]!='-1':
        tmp=nodes[eval(node[2])]
        tmp.append(x)
        tmp.append(y)
        tmp.append(node[-1]+1)
        queue.append(tmp)
    else:
        tmp=['-1','-1','-1','-1',x,y,node[-1]+1]
        queue.append(tmp)
window.mainloop()
