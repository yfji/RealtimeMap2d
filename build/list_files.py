import os
import os.path as op


if __name__=='__main__':
    img_dir='./rgb'
    files=os.listdir(img_dir)
    #files=sorted(files, reverse=True)[200:]
    files=sorted(files)
    with open('file_lst.txt','w') as f:
        for line in files:
            if len(line)==0:
                continue
            f.write(op.join(img_dir, line)+'\n')