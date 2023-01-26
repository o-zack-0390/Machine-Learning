# 間違えたファイルを記録
def process1(path1):
    global miss_list
    
    f    = open(path1, 'r', encoding="utf-8")
    line = f.readline()
    
    while line:
        miss_list.append(line.split(' '))
        line = f.readline()
    
    f.close()


# 間違えたファイルを別のカテゴリーにする
def process2(path2, path3):
    global miss_list
    
    f1   = open(path2, 'r', encoding="utf-8")
    f2   = open(path3, 'w', encoding="utf-8", newline="")
    size = len(miss_list)
    line = f1.readline()
    
    while line:
        line = line.split(' ')
        
        for i in range(size):
            if line[1] in miss_list[i][1]:
                line[0] = str(int(line[0]) + 9)
                break
        
        f2.write(' '.join(line))
        line = f1.readline()
        
    f1.close()
    f2.close()
    
    
miss_list = []
path1     = 'data/miss_uid.txt'
path2     = 'data/uid.txt'
path3     = 'data/new_uid.txt'

process1(path1)
process2(path2, path3)
