from copy import copy

def read_file(path):
    f     = open(path, 'r', encoding="utf-8", newline="")
    line  = f.readline()
    dict  = {}
    index = 1
    
    while line:
        dict[line] = index
        line       = f.readline()
        line       = line_replace(line)
        index     += 1
        
    f.close()
    return dict
    

def get_compare(r_path, w_path):
    f1    = open(r_path, 'r', encoding="utf-8", newline="")
    f2    = open(w_path, 'w', encoding="utf-8")
    line  = f1.readline()
    index = 1
    
    while line:
    
        if t_list.get(line) != None:
            if index < t_list[line]:
                f2.write("{}  {}:{}  {}\n".format(t_list[line]-index, index, t_list[line], line))
            
        else:
            f2.write("∞  {}:100↓  {}\n".format(index, line))
            
        line   = f1.readline()
        line   = line_replace(line)
        index += 1
        
    f1.close()
    f2.close()
    
    
def line_replace(line):
    line = line\
    .replace("\n", '')\
    .replace("\r", '')\
    .replace(' ','')
    
    return line


path1  = "data/important_word_1.txt"
path2  = "data/important_word_10.txt"
path3  = "data/compare.txt"
t_list = copy(read_file(path1))
t_size = len(t_list)
get_compare(path2, path3)