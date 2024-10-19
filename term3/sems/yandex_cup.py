from sys import stdout 
a = 1
b = 1
fl = 0
for i in range(200):
    print(b)
    stdout.flush() 
    inp = input()
    if inp == "wet":
        a = b
        b*=2
        continue
    elif inp == "ok":
        break
    else:
        fl = 1
        break
if (fl==0):
    for i in range(200):
        if(fl == 1):
            break
        m = (a+b)//2
        print(m)
        stdout.flush() 
        inp = input()
        if inp == "wet":
            a = m+1
        elif inp == "ok":
            b = m
            if(a == b):
                print("!",b)
                fl = 1
        else:
            break

    