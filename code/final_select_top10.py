import os
import numpy as np

stat_dir = './Assign1'
list_files = os.listdir(stat_dir)

file_name_list = []
simticks = []
inst_list = []
bp1 = 'TournamentBP'
bp2 = 'BiModeBP'
bp3 = 'LocalBP'
for file_name in list_files:
    if file_name == 'config.json' or file_name == 'config.dot' or file_name == 'config.ini' or file_name == 'fs':
        continue
    # print(file_name)
    fp = os.path.join(stat_dir, file_name)
    f = open(fp)
    lines = f.readlines()
    ticks_list = lines[197].split(' ')
    #print(ticks_list)
    ticks_list = list(filter(('').__ne__, ticks_list))
    #print(ticks_list)
    file_name_list.append(file_name)
    l1d_size = file_name[4:8]
    l1i_size = file_name[9:13]
    l2_size = file_name[14:19]
    l1_assoc = file_name[20:21]
    l2_assoc = file_name[22:23]
    if (file_name.find(bp1) != -1):
         bp=bp1
    elif (file_name.find(bp2) !=-1):
         bp=bp2
    else:
         bp=bp3	    
    lqent = file_name[-16:-14]
    sqent = file_name[-13:-11]
    robent = file_name[-10:-7]
    numiq = file_name[-6:-4]
## out_64kB_64kB_512kB_8_4_LocalBP_16_64_192_64.txt
    inst = 'build/X86/gem5.opt -d "Stats/Assign1/." --stats-file='
    inst += file_name
    inst += f' configs/example/se_modified.py -c ./tests/qsort5 --l1d_size={l1d_size} --l1i_size={l1i_size} --l2_size={l2_size} --l1d_assoc={l1_assoc} --l1i_assoc={l1_assoc} --l2_assoc={l2_assoc} --bp-type={bp} --lqentries={lqent}  --sqentries={sqent} --robentries={robent} --numiqentries={numiq} --caches'
    simticks.append(int(ticks_list[1]))
    inst_list.append(inst)

simticks = np.array(simticks)
# print(simticks.shape)
top_10_files_indices = np.argsort(simticks)[0:10]
# print(top_10_files_indices)
for idx in top_10_files_indices:
    print(inst_list[idx])
        
