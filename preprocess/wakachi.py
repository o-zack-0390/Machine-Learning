# coding: utf-8

def wakachi(text):

#	分かち書き : '-O wakati' , 
	tagger = MeCab.Tagger(r'-Owakati -d "/usr/lib/x86_64-linux-gnu/mecab/dic/mecab-ipadic-neologd"')
	
	try:
		res = tagger.parse(text.strip())
		
	except:
		return []
	
	return res



f1 = open("/content/drive/MyDrive/express/test.csv", 'r', encoding = "utf-8", newline = '')
f2 = open("/content/drive/MyDrive/express/wakachi.csv", 'w', encoding = "utf-8", newline = '')
line = f1.readline()

while line:
	line += line.strip()  
	line = line.replace(',', '、')  
	line = line.replace('\t', '') 
	line = wakachi(line)
	line = line.replace("\n",'')
	f2.write(line)
	line = f1.readline()
		
f1.close()
f2.close()

