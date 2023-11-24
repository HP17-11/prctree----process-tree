# prctree----process-tree
---> prctree act as a commad to check status of process and get details of its children

It is a C program that searches for processes in the process tree (rooted at a specified
process) and outputs the requested information based on the input parameters.
Synopsis :
prctree [root_process] [process_id1] [process_id2]… [process_id(n)] [OPTION]
. //up to 6 such processes can be listed

It Lists the PID and PPID of each process_id(n) if process_id(n) belongs to the process
tree rooted at root_process

OPTION
-dn additionally lists the PIDs of all the non-direct descendants of process_id1 (only)
-id additionally lists the PIDs of all the immediate descendants of process_id1
-lp additionally lists the PIDs of all the sibling processes of process_id1
- sz additionally Lists the PIDs of all sibling processes of process_id1 that are defunct
- gp additionally lists the PIDs of all the grandchildren of process_id1
- zz additionally prints the status of process_id1 (Defunct/ Not Defunct)
- zc additionally lists the PIDs of all the direct descendants of process_id1 that are
currently in the defunct state
-zx additionally lists the PIDs of the direct descendants of process_id1..process_id[n]
that are currently in the defunct state
