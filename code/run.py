import argparse
import itertools
import os
from multiprocessing import Pool

def run(instr):
    os.system(instr)

def main(args):

    if args.verbose == 0:
        file_name = 'run_all.bash'
        f = open(file_name, 'w')
    
        f.write('#!/bin/bash\n')

    L1D_size = ['32kB', '64kB']
    L1I_size = ['32kB', '64kB']
    L2_size = ['128kB', '256kB', '512kB']
    L1_assoc = [2, 4, 8]
    L2_assoc = [4, 8]
    bp_type = ['TournamentBP', 'BiModeBP', 'LocalBP']
    LQEntries = args.lqentries
    SQEntries = args.sqentries
    ROBEntries = [128, 192]
    numIQEntries = args.numiqentries

    
    i = 0
    instrs = []
    for l1d_size, l1i_size, l2_size, l1_assoc, l2_assoc, bp, lqent, sqent, robent, numiq in itertools.product(L1D_size, L1I_size, L2_size, L1_assoc, L2_assoc, bp_type, LQEntries, SQEntries, ROBEntries, numIQEntries):
        if i > 10:
            break
        # print(l1d_size, l1i_size, l2_size, l1_assoc, l2_assoc, bp, lqent, sqent, robent, numiq)

        instr = 'build/X86/gem5.opt -d "Stats/Assign1/." --stats-file='
        
        stat_file_name = f'out_{l1d_size}_{l1i_size}_{l2_size}_{l1_assoc}_{l2_assoc}_{bp}_{lqent}_{sqent}_{robent}_{numiq}.txt'
        instr += stat_file_name
        instr += f' configs/example/se_modified.py -c ./tests/qsort5 --l1d_size={l1d_size} --l1i_size={l1i_size} --l2_size={l2_size} --l1d_assoc={l1_assoc} --l1i_assoc={l1_assoc} --l2_assoc={l2_assoc} --bp-type={bp} --lqentries={lqent}  --sqentries={sqent} --robentries={robent} --numiqentries={numiq} --caches'

        if args.verbose == 0:
            f.write(instr)
        elif args.verbose == 1:
            os.system(instr)

        elif args.verbose==2:
            instrs.append(instr)
        # break
        i+=1
    print(args.num_processes)
    with Pool(args.num_processes) as p:
        p.map(run, instrs)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose', default=1, help='0 for generating bash script, 1 for running straight from python script', type=int)
    parser.add_argument('-lq', '--lqentries', nargs='+', default=['16', '32', '64'])
    parser.add_argument('-sq', '--sqentries', nargs='+', default=['16', '32', '64'])
    parser.add_argument('-numiq', '--numiqentries', nargs='+', default=['16', '32', '64'])
    parser.add_argument('-n', '--num_processes', type=int, default=1)
    args = parser.parse_args()
    main(args)